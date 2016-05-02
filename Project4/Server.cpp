/////////////////////////////////////////////////////////////////////////
// StringServer.cpp - Demonstrates simple one-way string messaging     //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////
/*
* This package implements a server that receives string messages
* from multiple concurrent clients and simply displays them.
*
* It's purpose is to provide a very simple illustration of how to use
* the Socket Package provided for Project #4.
*/
/*
* Required Files:
*   StringClient.cpp, StringServer.cpp
*   Sockets.h, Sockets.cpp, Cppll-BlockingQueue.h
*   Logger.h, Logger.cpp, Cpp11-BlockingQueue.h
*   Utilities.h, Utilities.cpp
*/
#include "../Sockets/Sockets.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Repository/Repository.h"
#include "../XMLResponseBodyGenerator/XMLResponseBodyGenerator.h"
#include "../Utilities/Utilities.h"
#include "../FileSystem/FileSystem.h"
#include <string>
#include <iostream>

using Show = StaticLogger<1>;
using namespace Utilities;
using namespace FileSystem;

using byte = std::string;
using Name = std::string;
using Value = std::string;
using Attribute = std::pair<Name, Value>;
using Attributes = std::vector<Attribute>;
using Body = std::vector<byte>;
using Message = std::string;




#pragma region FileTransferFunctions

//----------------Send file----------------------------------------//
bool sendFile(string filePath, SocketConnecter& si)
{
	const size_t BlockSize = 2048;
	const int bufferLen = 2000;
	char buffer[bufferLen];

	File file(filePath);
	file.open(File::in, File::binary);
	if (!file.isGood())
	{
		si.shutDownSend();
		return false;
	}
	while (true)
	{
		Block blk = file.getBlock(BlockSize);
		if (blk.size() == 0)
			break;
		for (size_t i = 0; i < blk.size(); ++i)
			buffer[i] = blk[i];
		si.send(blk.size(), buffer);
		if (!file.isGood())
			break;
	}
	file.close();

	return true;
}

//---------------receive file--------------------------------------//
bool receiveFile(string fileName, size_t fileLength, Socket& socket_)
{
	const size_t BlockSize = 2048;
	const int bufferLen = 2000;
	char buffer[bufferLen];
	
	//outputfile
	string socketUploadsDir = "../root/socket_uploads";
	string filePath = socketUploadsDir + "/" + fileName;
	File file(filePath);
	file.open(File::out, File::binary);

	size_t bytesToRead;
	while (true)
	{
		if (fileLength > BlockSize)
			bytesToRead = BlockSize;
		else bytesToRead = fileLength;

		socket_.recv(bytesToRead, buffer);

		if (socket_ == INVALID_SOCKET)
			return false;

		Block blk;
		for (size_t i = 0; i < bytesToRead; ++i)
			blk.push_back(buffer[i]);

		//To:do push block to file
		file.putBlock(blk);

		//-------check if need to read more
		if (fileLength < BlockSize)
			break;
		fileLength -= BlockSize;
	}
	file.close();
	return true;
}

#pragma endregion




/////////////////////////////////////////////////////////////////////
// Client handler (Runs on a new thread for every client)
//

class ClientHandler
{
public:
	void operator()(Socket& socket_);
	BlockingQueue<HttpMessage>& RecvQ();
private:
	void handleGetFiles(Socket& socket_, HttpMessage httpMessage);
	bool handleCheckIn(Socket& socket_, HttpMessage httpMessage);
	void handleCheckOut(Socket& socket_, HttpMessage httpMessage);
	void handleGetOpenCheckIn(Socket& socket_, HttpMessage httpMessage);
	void handleCloseOpenCheckIn(Socket& socket_, HttpMessage httpMessage);
	static BlockingQueue<HttpMessage> recvQ;
};

BlockingQueue<HttpMessage> ClientHandler::recvQ;

void ClientHandler::operator()(Socket& socket_)
{
	while (true)
	{		
		std::string msg = socket_.recvString();

		if (msg.size() == 0)
			break;

		//----------handle message --------------//
		HttpMessage httpMessage;
		httpMessage.parseMessage(msg);
		std::string command = httpMessage.findValue("Command");
		std::cout << "\nMessage Received From Client \n";
		httpMessage.printMessage();

		
		if (command == "GetFiles")
		{
			handleGetFiles(socket_,httpMessage);			
		}
		else if (command == "Check-In")
		{
			if (!handleCheckIn(socket_, httpMessage))
				break;
		}
		else if (command == "Check-Out")
		{
			handleCheckOut(socket_, httpMessage);
		}
		else if (command == "GetOpenCheck-In")
		{

		}
		else if (command == "CloseOpenCheck-In")
		{

		}
		else if (command == "quit")
		{			
			std::cout << "\nClient closed connection....\n";
		}
		else
		{
			std::cout << "\nCommand not recognized\n";
		}

		//enq msg on receiveQueue for main thread
		recvQ.enQ(httpMessage);
	}

	std::cout << "\nClosing connection with client\n";
	socket_.shutDown();
	socket_.close();
}

//------------Handle get files request------------//
void ClientHandler::handleGetFiles(Socket& socket_, HttpMessage httpMessage)
{
	//No Action required by Client Handler for getFiles command
	//Main thread fetches the package list from the repository server
}

//-----------Handle CheckIn requests ------------//
bool ClientHandler::handleCheckIn(Socket& socket_, HttpMessage httpMessage)
{
	std::cout << "\nCheck-In File....\n";

	string packageName = "Package5";           //will get from httpAttributes
	size_t cppFileLength = Converter<size_t>::toValue(httpMessage.findValue("cppFileLength"));
	size_t hFileLength = Converter<size_t>::toValue(httpMessage.findValue("hFileLength"));



	//client first sends cpp file: READ IT from socket
	cout << "CPP FILE: \n";
	receiveFile(packageName + ".cpp", cppFileLength, socket_);
	cout << "\n Cpp File received-------------\n";

	//receive the h file
	cout << "H FILE : \n";
	receiveFile(packageName + ".h", hFileLength, socket_);
	cout << "\n h File received-------------\n";

	return true;
}

//-------Handle CheckOut requests----------------//
void ClientHandler::handleCheckOut(Socket& socket_, HttpMessage httpMessage)
{
	std::cout << "\nCheck-Out File....\n";
	//No Action required by Client Handler for checkout command
	//Main thread fetches the package list from the repository server
}

//-------Handle Get Open check Ins request-------//
void ClientHandler::handleGetOpenCheckIn(Socket& socket_, HttpMessage httpMessage)
{
	std::cout << "\nGet Open Check-In ....\n";
	//No Action required by Client Handler for GetOpenCheckIn command
	//Main thread fetches the package list from the repository server
}

//-------Handler Close open check In request------//
void ClientHandler::handleCloseOpenCheckIn(Socket& socket_, HttpMessage httpMessage)
{
	std::cout << "\nClose Open Check-In ....\n";
	//No Action required by Client Handler for CloseOpenCheckIn command
	//Main thread fetches the package list from the repository server
}

BlockingQueue<HttpMessage>& ClientHandler::RecvQ()
{
	return recvQ;
}



//////////////////////////////////////////////////////////////////////
// Send Messages class (Reply to requests)
// Runs on a seperate thread
//
class Sender
{
public:
	void operator()(BlockingQueue<HttpMessage>& sendQ);
private:
	void sendCheckOutFiles(SocketConnecter& si, HttpMessage response);
};

void Sender::operator()(BlockingQueue<HttpMessage>& sendQ)
{
	SocketConnecter si;
	while (true)
	{
		HttpMessage response = sendQ.deQ();

		std::string ToAddr = response.findValue("ToAddr");
		std::cout << "\nSending response to Client: " << ToAddr << " \n";

		//connect to client		
		while (!si.connect("localhost", 8081))   // hard-coding now
		{
			std::cout << "\n Server waiting to connect to client\n";
			::Sleep(100);
		}
		
		std::string command = response.findValue("Command");

		if (command == "Check-Out")
		{
			sendCheckOutFiles(si, response);
		}
		else
		{
			si.sendString(response.buildMessage());
		}	
	}
}

void Sender::sendCheckOutFiles(SocketConnecter& si, HttpMessage response)
{
	XMLResponseBodyGenerator xml;

	Package checkOutPackage = xml.parseRequestBodyForCheckOutPackage(response.getBody());
	vector<Package> dependencies = xml.parseRequestBodyForDependenciesInCheckOut(response.getBody());

	vector<string> filePaths;
	
	string packageDirBase = "../root";

	//add attributes to send message to client telling about each file size to be sent by server
	Attribute attrCheckOutPackageCppLength;
	attrCheckOutPackageCppLength.first = checkOutPackage.name + "_" + checkOutPackage.version + "_" + "cpp_Length";
	FileInfo fileCppInfo(packageDirBase + "/" + checkOutPackage.name + "_" + checkOutPackage.version + "/" + checkOutPackage.name + ".cpp");
	attrCheckOutPackageCppLength.second = std::to_string(fileCppInfo.size());
	response.addAttribute(attrCheckOutPackageCppLength);
	Attribute attrCheckOutPackageHLength;
	attrCheckOutPackageHLength.first = checkOutPackage.name + "_" + checkOutPackage.version + "_" + "h_Length";
	FileInfo fileHInfo(packageDirBase + "/" + checkOutPackage.name + "_" + checkOutPackage.version + "/" + checkOutPackage.name + ".h");
	attrCheckOutPackageHLength.second = std::to_string(fileHInfo.size());
	response.addAttribute(attrCheckOutPackageHLength);

	for (auto dep : dependencies)
	{
		Attribute attrDepPackageCppLength;
		attrDepPackageCppLength.first = dep.name + "_" + dep.version + "_" + "cpp_Length";
		FileInfo fileCppInfo(packageDirBase + "/" + dep.name + "_" + dep.version + "/" + dep.name + ".cpp");
		attrDepPackageCppLength.second = std::to_string(fileCppInfo.size());
		response.addAttribute(attrDepPackageCppLength);
		Attribute attrDepPackageHLength;
		attrDepPackageHLength.first = dep.name + "_" + dep.version + "_" + "h_Length";
		FileInfo fileHInfo(packageDirBase + "/" + dep.name + "_" + dep.version + "/" + dep.name + ".h");
		attrDepPackageHLength.second = std::to_string(fileHInfo.size());
		response.addAttribute(attrDepPackageHLength);
	}

	si.sendString(response.buildMessage()); // first this message is sent to prepare client for receiving approriate files

	//send check out package first
	sendFile(packageDirBase + "/" + checkOutPackage.name + "_" + checkOutPackage.version + "/" + checkOutPackage.name + ".cpp",si);
	sendFile(packageDirBase + "/" + checkOutPackage.name + "_" + checkOutPackage.version + "/" + checkOutPackage.name + ".h", si);

	//send dependencies
	for (auto dep : dependencies)
	{
		sendFile(packageDirBase + "/" + dep.name + "_" + dep.version + "/" + dep.name + ".cpp", si);
		sendFile(packageDirBase + "/" + dep.name + "_" + dep.version + "/" + dep.name + ".h", si);
	}
}

//----< Server main thread >--------------------------------------------------

int main()
{
	Show::attach(&std::cout);
	Show::start();	
	try
	{
		//---------SendQ for communication --------------------//
		BlockingQueue<HttpMessage> sendQ;

		//-------Initialize socket System library--------------//
		SocketSystem ss;

		//---------Start server listener------------------------//
		SocketListener sl(8080, Socket::IP6);
		ClientHandler cp;
		sl.start(cp);

		//-------start response/send thread ---------//
		Sender sender;
		std::thread sendThread(sender, std::ref(sendQ));
		sendThread.detach();

		std::cout << "\n\nServer started at 8080............\n\n";

		//Initialize Repository
		Repository repo;
		XMLResponseBodyGenerator xmlResponseBodyGenerator;
		while (true)
		{
			//check for enqueued messages handled by client handler (Blocks on empty queue)
			HttpMessage msg = cp.RecvQ().deQ();


			//process Repository related tasks in main thread
			std::string command_ = msg.findValue("Command");

			std::string body;
			if (command_ == "GetFiles")
			{
				body = xmlResponseBodyGenerator.getResponseBodyForGetFiles(repo.getPackageList());
			}
			else if (command_ == "Check-In")
			{
				Package package = xmlResponseBodyGenerator.parseRequestBodyForCheckInPackage(msg.getBody());
				vector<Package> dependencies = xmlResponseBodyGenerator.parseRequestBodyForDependenciesInCheckIn(msg.getBody());
				body = repo.checkInPackage(package, dependencies);
			}
			else if (command_ == "Check-Out")
			{
				Package package = xmlResponseBodyGenerator.parseRequestBodyForCheckOutPackage(msg.getBody());

				bool includeDependencies;
				if (msg.findValue("includeDependencies") == "true")
					includeDependencies = true;
				else
					includeDependencies = false;

				vector<Package> dependencies = repo.checkOutPackageDependencies(package,includeDependencies);
				body = xmlResponseBodyGenerator.getRequestBodyforCheckOut(package, dependencies);
			}
			else if (command_ == "GetOpenCheck-In")
			{
				body = xmlResponseBodyGenerator.getResponseBodyForGetFiles(repo.getOpenCheckIns());
			}
			else if (command_ == "CloseOpenCheck-In")
			{
				Package closeCheckInPackage = xmlResponseBodyGenerator.parseRequestBodyForCloseCheckInPackage(msg.getBody());
				body = repo.closeOpenCheckIn(closeCheckInPackage);
			}


			//prepare response
			HttpMessage response;
			Attribute command; command.first = "Command"; command.second = msg.findValue("Command");
			response.addAttribute(command);
			Attribute ToAddr; ToAddr.first = "ToAddr"; ToAddr.second = msg.findValue("FromAddr");
			response.addAttribute(ToAddr);
			Attribute FromAddr; FromAddr.first = "FromAddr"; FromAddr.second = msg.findValue("ToAddr");
			response.addAttribute(FromAddr);
			response.setBody(body);


			/*enqueue response*/
			sendQ.enQ(response);
		}


		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}
