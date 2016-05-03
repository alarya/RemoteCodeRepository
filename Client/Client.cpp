/////////////////////////////////////////////////////////////////////////
// Server.cpp - Starts the server and starts request/response threads  //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////
/*
* This package implements a server that receives requests from multiple
* clients. The clients send Http styles messages for requests and server
* responds with Http styles messages too. The server starts a Sender thread
* and a client handler thread for each request from one particular client
*
*
*
* Required Files:
*   HttpMessage.h, HttpMessage.cpp
*   XMLResponseBodyGenerator.h, XMLResponseBodyGenerator.cpp
*   FileSystem.h, FileSystem.cpp
*   Sockets.h, Sockets.cpp, Cppll-BlockingQueue.h
*   Logger.h, Logger.cpp, Cpp11-BlockingQueue.h
*   Utilities.h, Utilities.cpp
*/

#include "../Sockets/Sockets.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../XMLResponseBodyGenerator/XMLResponseBodyGenerator.h"
#include "../FileSystem/FileSystem.h"
#include "../Client/Client.h"
#include <string>
#include <iostream>
#include <thread>


using byte = std::string;
using Name = std::string;
using Value = std::string;
using Attribute = std::pair<Name, Value>;
using Attributes = std::vector<Attribute>;
using Body = std::vector<byte>;
using Message = std::string;
using Show = StaticLogger<1>;
using namespace Utilities;
using namespace FileSystem;
using namespace std;


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
	string clientDownloadsDir = "../clientFolder/downloads";
	string filePath = clientDownloadsDir + "/" + fileName;
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
// Client handler
//
// Runs a different thread for each response

BlockingQueue<HttpMessage> ClientHandler::recvQ;

//----------reads incoming messages using Socket listener---------//
void ClientHandler::operator()(Socket& socket_)
{
	while (true)
	{
		std::string msg = socket_.recvString();
		
		if (msg.size() == 0)
			break;

		HttpMessage httpMessage;
		httpMessage.parseMessage(msg);
		
		std::string command = httpMessage.findValue("Command");

		checkCommand(command, socket_, httpMessage);

		if (command == "quit")
			break;
		
		//enque in recvQ
		recvQ.enQ(httpMessage);
	}
}

void ClientHandler::checkCommand(string command, Socket& socket_, HttpMessage httpMessage)
{
	if (command == "GetFiles")
	{
		GetFiles(socket_, httpMessage);
	}
	else if (command == "Check-In")
	{
		CheckIn(socket_, httpMessage);
	}
	else if (command == "Check-Out")
	{
		CheckOut(socket_, httpMessage);
	}
	else if (command == "GetOpenCheck-In")
	{
		GetOpenCheckIns(socket_, httpMessage);
	}
	else if (command == "CloseOpenCheck-In")
	{
		CloseOpenCheckIn(socket_, httpMessage);
	}
}

//--------returns reference to the receive queue-----------------//
BlockingQueue<HttpMessage>& ClientHandler::RecvQ() {
	return recvQ;
}

#pragma region ClienHandler_Commands

void ClientHandler::GetFiles(Socket& si, HttpMessage httpMessage)
{
	std::cout << "\nResponse recieved:- \n";
	httpMessage.printMessage();

}

void ClientHandler::CheckIn(Socket& si, HttpMessage httpMessage)
{
	std::cout << "\nResponse recieved:- \n";
	httpMessage.printMessage();
}

void ClientHandler::GetOpenCheckIns(Socket& si, HttpMessage httpMessage)
{
	std::cout << "\nResponse recieved:- \n";
	httpMessage.printMessage();
}

void ClientHandler::CloseOpenCheckIn(Socket& si, HttpMessage httpMessage)
{
	std::cout << "\nResponse recieved:- \n";
	httpMessage.printMessage();
}

void ClientHandler::CheckOut(Socket& si, HttpMessage httpMessage)
{
	std::cout << "\nResponse recieved:- \n";
	httpMessage.printMessage();

	XMLResponseBodyGenerator xml;

	Package checkOutPackage = xml.parseRequestBodyForCheckOutPackage(httpMessage.getBody());
	vector<Package> dependencies = xml.parseRequestBodyForDependenciesInCheckOut(httpMessage.getBody());

	//receive Check-Out Package
	string checkOutPackageCppFile = checkOutPackage.name + ".cpp";
	string checkOutPackageHFile = checkOutPackage.name + ".h";
	size_t checkOutPackageCppLength = Converter<size_t>::toValue(httpMessage.findValue(checkOutPackage.name + "_" + checkOutPackage.version + "_cpp_Length"));
	size_t checkOutPackageHLength = Converter<size_t>::toValue(httpMessage.findValue(checkOutPackage.name + "_" + checkOutPackage.version + "_h_Length"));

	receiveFile(checkOutPackageCppFile, checkOutPackageCppLength, si);
	receiveFile(checkOutPackageHFile, checkOutPackageHLength, si);
	cout << "\n Package: " << checkOutPackage.name + "_" + checkOutPackage.version << " received \n";
	//receive Dependencies if any
	for (auto dep : dependencies)
	{
		string depPackageCppFile = dep.name + ".cpp";
		string depPackageHFile = dep.name + ".h";
		size_t depPackageCppLength = Converter<size_t>::toValue(httpMessage.findValue(dep.name + "_" + dep.version + "_cpp_Length"));
		size_t depPackageHLength = Converter<size_t>::toValue(httpMessage.findValue(dep.name + "_" + dep.version + "_h_Length"));

		receiveFile(depPackageCppFile, depPackageCppLength, si);
		receiveFile(depPackageHFile, depPackageHLength, si);

		cout << "\n Package: " << dep.name + "_" + dep.version << " received \n";
	}

}

#pragma endregion





//////////////////////////////////////////////////////////////////////
// Send Messages class (Reply to requests)
// Runs on a seperate thread
//

//---------------sender thread----------------------------------------------//
void SenderHandler::operator()(BlockingQueue<HttpMessage>& sendQ,SocketConnecter& si)
{
	while (true)
	{
		HttpMessage httpMessage = sendQ.deQ();
		
		std::string command = httpMessage.findValue("Command");

		if (command == "GetFiles")
		{
			GetFiles(si, httpMessage);
		}
		else if (command == "Check-In")
		{
			CheckIn(si, httpMessage);
		}
		else if (command == "Check-Out")
		{
			CheckOut(si, httpMessage);
		}
		else if (command == "GetOpenCheck-In")
		{
			GetOpenCheckIns(si, httpMessage);
		}
		else if (command == "CloseOpenCheck-In")
		{
			CloseOpenCheckIn(si, httpMessage);
		}
		else if (command == "quit")
		{
			break;
		}
		else
		{
			si.sendString(httpMessage.buildMessage());
		}
	}
}

#pragma region Sender_Commands

//-------------Command: GetFiles------------------------//
void SenderHandler::GetFiles(SocketConnecter& si, HttpMessage httpMessage)
{
	std::cout << "\nSending Message to Server:-\n";
	std::cout << "Command: " << httpMessage.findValue("Command") << "\n";
	std::cout << "ToAddr: " << httpMessage.findValue("ToAddr") << "\n";
	std::cout << "FromAddr: " << httpMessage.findValue("FromAddr") << "\n";
	std::string body;
	for (auto c : httpMessage.body())
		body += c;
	std::cout << "Body: " << body << "\n";
	
	si.sendString(httpMessage.buildMessage());
}

//------------Command: CheckIn--------------------------//
void SenderHandler::CheckIn(SocketConnecter& si, HttpMessage httpMessage)
{
	string cppFilePath = httpMessage.findValue("cppFilePath");
	string hFilePath = httpMessage.findValue("hFilePath");
	FileInfo cppFileInfo(cppFilePath);
	FileInfo hFileInfo(hFilePath);

	Attribute cppFileLengthAttrib ; 
	cppFileLengthAttrib.first = "cppFileLength"; 
	cppFileLengthAttrib.second = std::to_string( cppFileInfo.size() );
	Attribute hFileLengthAttrib ;
	hFileLengthAttrib.first = "hFileLength";
	hFileLengthAttrib.second = std::to_string(hFileInfo.size());

	httpMessage.addAttribute(cppFileLengthAttrib);
	httpMessage.addAttribute(hFileLengthAttrib);

	std::cout << "\nSending Message to Server:-\n";
	httpMessage.printMessage();

	si.sendString(httpMessage.buildMessage());

	/*Server waits for the package files when it sees a Check-In command*/
	std::cout << "\nSending files to Server..\n";

	//--------Send Cpp file first--------------
	sendFile(cppFilePath, si);
	

	//------Send h file ----------------------
	sendFile(hFilePath,si);
}

//-----------Command: GetOpenCheckIns------------------//
void SenderHandler::GetOpenCheckIns(SocketConnecter& si, HttpMessage httpMessage)
{
	std::cout << "\nSending Message to Server:-\n";
	httpMessage.printMessage();

	si.sendString(httpMessage.buildMessage());
}

//----------Command: CloseOpenCheckIn------------------//
void SenderHandler::CloseOpenCheckIn(SocketConnecter& si, HttpMessage httpMessage)
{
	std::cout << "\nSending Message to Server:-\n";
	httpMessage.printMessage();

	si.sendString(httpMessage.buildMessage());
}

//----------Command: CheckOut--------------------------//
void SenderHandler::CheckOut(SocketConnecter& si, HttpMessage httpMessage)
{
	httpMessage.printMessage();

	si.sendString(httpMessage.buildMessage());
}

#pragma endregion


//////////////////////////////////////////////////////////////////////////////////
// Client class
//
//

void Client::startClient(){
	try{	
		//---------sendQ for communication --------------------//		
		BlockingQueue<HttpMessage> sendQ;

		//-------Initialize socket System library--------------//
		SocketSystem ss;

		//-------------start Client Listener-------------------//
		SocketListener sl(8081, Socket::IP6);
		ClientHandler cp;
		sl.start(cp);

		//-------------Connect to server-----//
		SocketConnecter si;
		while (!si.connect("localhost", 8080)){		
			std::cout << "\n client waiting to connect\n";
			::Sleep(100);
		}
		//-------start send thread ---------//
		SenderHandler sender;
		std::thread sendThread(sender, std::ref(sendQ), std::ref(si));
		sendThread.detach();

		//------Main thread: places request/response to send queue---//
		while (true){
			HttpMessage httpMessage = channelSendQ.deQ();

			//----Add hearders for command,fromAddr, ToAddr----------------------//
			Attribute fromAddrAttrib;
			fromAddrAttrib.first = "FromAddr"; fromAddrAttrib.second = "127.0.0.1:8081";
			httpMessage.addAttribute(fromAddrAttrib);
			Attribute ToAddrAttrib;
			ToAddrAttrib.first = "ToAddr"; ToAddrAttrib.second = "127.0.0.1:8080";
			httpMessage.addAttribute(ToAddrAttrib);
			//place message to send on the sendQ
			sendQ.enQ(httpMessage);
			//wait for response
			HttpMessage response = cp.RecvQ().deQ();
			channelRecvQ.enQ(response);
		}
		si.shutDownSend(); //quit command sent as input
	}
	catch (std::exception& exc){	
		cout << "\n  Exeception caught: ";
		cout << "\n  " << std::string(exc.what()) << "\n\n";
	}
}

HttpMessage Client::doOperation(HttpMessage message)
{
	
	//enqueue request msg from Client GUI into client queue
	channelSendQ.enQ(message);

	//wait for the response from client getting the request processed by the server
	return channelRecvQ.deQ();
}

#ifdef TEST_CLIENT


//--------------Client main thread-----------------------------//
int main()
{
	try
	{
		//---------sendQ for communication --------------------//		
		BlockingQueue<HttpMessage> sendQ;

		//-------Initialize socket System library--------------//
		SocketSystem ss;

		//-------------start Client Listener-------------------//
		SocketListener sl(8081, Socket::IP6);
		ClientHandler cp;
		sl.start(cp);

		//-------------Connect to server-----//
		SocketConnecter si;
		while (!si.connect("localhost", 8080))
		{
			std::cout << "\n client waiting to connect\n";
			::Sleep(100);
		}

		//-------start send thread ---------//
		SenderHandler sender;
		std::thread sendThread(sender, std::ref(sendQ), std::ref(si));
		sendThread.detach();

		//------Main thread: places request/response to send queue---//
		while (true)
		{
			HttpMessage httpMessage;
			std::string command;
			std::cout << "\nEnter a command: ";
			std::getline(std::cin, command);

			//don't send anymore commands: quit
			if (command == "quit")
			{
				break;
			}

			//----Add hearders for command,fromAddr, ToAddr----------------------//
			Attribute commandAttrib;
			commandAttrib.first = "Command"; commandAttrib.second = command;
			httpMessage.addAttribute(commandAttrib);
			Attribute fromAddrAttrib;
			fromAddrAttrib.first = "FromAddr"; fromAddrAttrib.second = "127.0.0.1:8081";
			httpMessage.addAttribute(fromAddrAttrib);
			Attribute ToAddrAttrib;
			ToAddrAttrib.first = "ToAddr"; ToAddrAttrib.second = "127.0.0.1:8080";
			httpMessage.addAttribute(ToAddrAttrib);

			//place message to send on the sendQ
			sendQ.enQ(httpMessage);

			//wait for response
			HttpMessage response = cp.RecvQ().deQ();
		}
		si.shutDownSend(); //quit command sent as input
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

#endif // TEST_CLIENT


