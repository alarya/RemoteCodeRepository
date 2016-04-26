/////////////////////////////////////////////////////////////////////////
// StringClient.cpp - Demonstrates simple one-way string messaging     //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////
/*
* This package implements a client that sends string messages
* to a server that simply displays them.
*
* It's purpose is to provide a very simple illustration of how to use
* the Socket Package provided for Project #4.
*/
/*
* Required Files:
*   StringClient.cpp, StringServer.cpp
*   Sockets.h, Sockets.cpp
*   Logger.h, Logger.cpp, Cpp11-BlockingQueue.h
*   Utilities.h, Utilities.cpp
*/
#include "../Sockets/Sockets.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include <string>
#include <iostream>
#include <thread>

using Show = StaticLogger<1>;
using namespace Utilities;


/////////////////////////////////////////////////////////////////////
// Client handler
//
// Runs a different thread for each response
class ClientHandler
{
public:
	void operator()(Socket& socket_);
	BlockingQueue<HttpMessage>& RecvQ();
private:	
	static BlockingQueue<HttpMessage> recvQ;      //shared by all client handlers 
};

BlockingQueue<HttpMessage> ClientHandler::recvQ;

//----------reads incoming messages using Socket listener---------//
void ClientHandler::operator()(Socket& socket_)
{
	while (true)
	{
		std::string msg = socket_.recvString();
		Show::write("\n  Client recvd message \"" + msg + "\"");
		if (msg == "quit")
			break;

		//enque in recvQ
		HttpMessage httpMessage;
		httpMessage.parseMessage(msg);
		recvQ.enQ(httpMessage);
	}
}

//--------returns reference to the receive queue-----------------//
BlockingQueue<HttpMessage>& ClientHandler::RecvQ() {
	return recvQ;
}


//////////////////////////////////////////////////////////////////////
// Send Messages class (Reply to requests)
// Runs on a seperate thread
//
class Sender
{
public:
	void operator()(BlockingQueue<HttpMessage>& sendQ, SocketConnecter& si);
};

void Sender::operator()(BlockingQueue<HttpMessage>& sendQ,SocketConnecter& si)
{
	while (true)
	{
		HttpMessage httpMessage = sendQ.deQ();
		std::cout << "\nSending message to Server..\n";
		si.sendString(httpMessage.buildMessage());


		//------check if file transfer needed--------
		if ( httpMessage.findValue("Command") == "Check-In")
		{
			std::cout << "\nSending file to Server..\n";
			//mocking fileTransfer
			const int bufferLen = 2000;   //mocking for now, ideally previous message will send file length
			std::size_t fileLength = 200;
			char buffer[bufferLen];
			for (int i = 0; i < 200; i++)
					buffer[i] = 'a';

			si.send(fileLength, buffer);
		}
		//------------------------------------------

	}
}







//--------------Client main thread-----------------------------//

using byte = std::string;
using Name = std::string;
using Value = std::string;
using Attribute = std::pair<Name, Value>;
using Attributes = std::vector<Attribute>;
using Body = std::vector<byte>;
using Message = std::string;

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
		Sender sender;
		std::thread sendThread(sender, std::ref(sendQ),std::ref(si));
		sendThread.detach();

		//------Main thread: places request/response to send queue---//
		while (true)
		{
			HttpMessage httpMessage;
			std::string command;
			std::cout << "\nEnter a command: ";
			std::getline(std::cin, command); 

			if (command == "quit")
			{				
				break;
			}

			Attribute commandAttrib;
			commandAttrib.first = "Command"; commandAttrib.second = command;
			httpMessage.addAttribute(commandAttrib);
			Attribute fromAddrAttrib;
			fromAddrAttrib.first = "FromAddr"; fromAddrAttrib.second = "127.0.0.1:8081";
			httpMessage.addAttribute(fromAddrAttrib);
			Attribute ToAddrAttrib;
			ToAddrAttrib.first = "ToAddr"; ToAddrAttrib.second = "127.0.0.1:8080";
			httpMessage.addAttribute(ToAddrAttrib);

			Body body;						
			httpMessage.setBody("someBody");

			//place message to send on the sendQ
			sendQ.enQ(httpMessage);


			//wait for response
			HttpMessage response = cp.RecvQ().deQ();
			std::cout << "\nResponse recieved for: Command: " << response.findValue("Command");
			
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
