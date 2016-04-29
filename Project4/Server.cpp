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

		//enq msg on receiveQueue to prepare response
		recvQ.enQ(httpMessage);
	}

	std::cout << "\nClosing connection with client\n";
	socket_.shutDown();
	socket_.close();
}

//------------Handle get files request------------//
void ClientHandler::handleGetFiles(Socket& socket_, HttpMessage httpMessage)
{

}

//-----------Handle CheckIn requests ------------//
bool ClientHandler::handleCheckIn(Socket& socket_, HttpMessage httpMessage)
{
	std::cout << "\nCheck-In File....\n";

	string packageName = "Package5";           //will get from httpAttributes
	size_t cppFileLength = Converter<size_t>::toValue(httpMessage.findValue("cppFileLength"));
	size_t hFileLength = Converter<size_t>::toValue(httpMessage.findValue("hFileLength"));

	const size_t BlockSize = 2048;
	const int bufferLen = 2000;
	char buffer[bufferLen];

	//client first sends cpp file: READ IT from socket
	cout << "CPP FILE: \n";
	size_t bytesToRead;
	while (true)
	{
		if (cppFileLength > BlockSize)
			bytesToRead = BlockSize;
		else bytesToRead = cppFileLength;

		socket_.recv(bytesToRead, buffer);

		if (socket_ == INVALID_SOCKET)
			return false;

		Block blk;
		for (size_t i = 0; i < bytesToRead; ++i)
			blk.push_back(buffer[i]);

		//To:do push block to file
		cout << blk.size();
		
		//-------check if need to read more
		if (cppFileLength < BlockSize)
			break;
		cppFileLength -= BlockSize;
	}
	cout << "\n Cpp File received-------------\n";

	//receive the h file
	cout << "H FILE : \n";
	while (true)
	{
		if (hFileLength > BlockSize)
			bytesToRead = BlockSize;
		else bytesToRead = hFileLength;

		socket_.recv(bytesToRead, buffer);

		if (socket_ == INVALID_SOCKET)
			return false;

		Block blk;
		for (size_t i = 0; i < bytesToRead; ++i)
			blk.push_back(buffer[i]);

		//To:do push block to file
		cout << blk.size();


		//-------check if need to read more
		if (hFileLength < BlockSize)
			break;
		hFileLength -= BlockSize;
	}
	cout << "\n h File received-------------\n";

	return true;
}

//-------Handle CheckOut requests----------------//
void ClientHandler::handleCheckOut(Socket& socket_, HttpMessage httpMessage)
{
	std::cout << "\nCheck-Out File....\n";
}

//-------Handle Get Open check Ins request-------//
void ClientHandler::handleGetOpenCheckIn(Socket& socket_, HttpMessage httpMessage)
{

}

//-------Handler Close open check In request------//
void ClientHandler::handleCloseOpenCheckIn(Socket& socket_, HttpMessage httpMessage)
{

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
		si.sendString(response.buildMessage());

		//si.shutDownSend();

		//si.close();

		//------check if file transfer needed--------
		//HttpMessage httpMessage;
		//httpMessage.parseMessage(msg);
		//if (httpMessage.findValue("Command") == "Check-In")
		//{
		//	std::cout << "\nSending file to Server..\n";
		//	//mocking fileTransfer
		//	const int bufferLen = 2000;   //mocking for now, ideally previous message will send file length
		//	std::size_t fileLength = 200;
		//	char buffer[bufferLen];
		//	for (int i = 0; i < 200; i++)
		//		buffer[i] = 'a';

		//	si.send(fileLength, buffer);
		//}
		//------------------------------------------

	}
}



using byte = std::string;
using Name = std::string;
using Value = std::string;
using Attribute = std::pair<Name, Value>;
using Attributes = std::vector<Attribute>;
using Body = std::vector<byte>;
using Message = std::string;


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


			//process from Repository related tasks in main thread
			std::string command_ = msg.findValue("Command");

			std::string body;
			if (command_ == "GetFiles")
			{
				body = xmlResponseBodyGenerator.getResponseBodyForGetFiles(repo.getPackageList());
			}
			else if (command_ == "Check-In")
			{

			}
			else if (command_ == "Check-Out")
			{
		
			}
			else if (command_ == "GetOpenCheck-In")
			{
				body = xmlResponseBodyGenerator.getResponseBodyForGetFiles(repo.getOpenCheckIns());
			}
			else if (command_ == "CloseOpenCheck-In")
			{

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
