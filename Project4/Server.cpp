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
#include <string>
#include <iostream>

using Show = StaticLogger<1>;


/////////////////////////////////////////////////////////////////////
// Client handler (Runs on a new thread for every client)
//

class ClientHandler
{
public:
	void operator()(Socket& socket_);
	BlockingQueue<HttpMessage>& RecvQ();
private:
	void handleGetFiles(Socket& socket_);
	bool handleCheckIn(Socket& socket_);
	void handleCheckOut(Socket& socket_);
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
		std::string client = httpMessage.findValue("FromAddr");
		std::cout << "\nRequest: " << command << "  Client: " << client << "\n";
		
		if (command == "GetFiles")
		{
			handleGetFiles(socket_);			
		}
		else if (command == "Check-In")
		{
			if (!handleCheckIn(socket_))
				break;
		}
		else if (command == "Check-Out")
		{
			handleCheckOut(socket_);
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
void ClientHandler::handleGetFiles(Socket& socket_)
{
	std::cout << "\nSending Files....\n";
}

//-----------Handle CheckIn requests ------------//
bool ClientHandler::handleCheckIn(Socket& socket_)
{
	std::cout << "\nCheck-In File....\n";
	//making up shit for time being
	std::string fileName = "package1.cpp";   //client message should provide this in prev message
	int fileLength = 200;					 //client message should provide this in prev message
	const size_t bufferLen = 2000;
	char buffer[bufferLen];
	bool ok = socket_.recv(fileLength, buffer);
	if (socket_ == INVALID_SOCKET)
		return false;
	if (ok)
	{
		buffer[fileLength] = '\0';
		std::string fileData(buffer);
		std::cout << "Client uploaded file \ndata :- \n";
		std::cout << fileData;
		std::cout << "\nSize: " << fileData.size();
	}
	return true;
}

//-------Handle CheckOut requests----------------//
void ClientHandler::handleCheckOut(Socket& socket_)
{
	std::cout << "\nCheck-Out File....\n";
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
		std::cout << "Press any Key to break";

		while (true)
		{
			//check for enqueued messages handled by client handler (Blocks on empty queue)
			HttpMessage msg = cp.RecvQ().deQ();

			//prepare response
			HttpMessage response;
			Attribute command; command.first = "Command"; command.second = msg.findValue("Command");
			response.addAttribute(command);
			Attribute ToAddr; ToAddr.first = "ToAddr"; ToAddr.second = msg.findValue("FromAddr");
			response.addAttribute(ToAddr);
			Attribute FromAddr; FromAddr.first = "FromAddr"; FromAddr.second = msg.findValue("ToAddr");
			response.addAttribute(FromAddr);

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
