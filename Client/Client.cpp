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
#include "../HttpMessage/IService.h"
#include "../HttpMessage/HttpMessage.h"
#include <string>
#include <iostream>
#include <thread>

using Show = StaticLogger<1>;
using namespace Utilities;


/////////////////////////////////////////////////////////////////////
// Client handler
//

class ClientHandler
{
public:
	void operator()(Socket& socket_);
};

void ClientHandler::operator()(Socket& socket_)
{
	while (true)
	{
		std::string msg = socket_.recvString();
		Show::write("\n  Client recvd message \"" + msg + "\"");
		if (msg == "quit")
			break;
	}
}


/////////////////////////////////////////////////////////////////////
// Client Class
//

class Client : IService {
public:
	static BlockingQueue<std::string> recvQ;
	static BlockingQueue<std::string> sendQ;

	Client()
	{
		SocketSystem ss;
	}

	void startListener(std::size_t port, ClientHandler ch)
	{
		sl = new SocketListener(port, Socket::IP6);
		sl->start(ch);
		std::cout << "\n Client started listening at port: " << port << "\n";		
	}

	void connect(std::size_t port)
	{
		si = new SocketConnecter();
		while (!si->connect("localhost", port))
		{
			Show::write("\n Client waiting to connect");
			::Sleep(100);
		}

		std::cout << "\n Connected to Server: " << port << "\n";
	}

	void PostMessage(std::string msg)
	{
		si->sendString(msg);
	}

	void shutDownSend()
	{
		si->shutDownSend();
	}
private:
	SocketListener* sl;
	SocketConnecter* si;
};



#ifdef TEST_CLIENT

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
		SocketSystem ss;
		SocketConnecter si;
		
		while (!si.connect("localhost", 8080))
		{
			std::cout << "\n client waiting to connect\n";
			::Sleep(100);
		}		

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
			Body body;
			body.push_back("someBody");
			httpMessage.setBody(body);
			si.sendString(httpMessage.buildMessage());
		}

		si.shutDownSend();
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

#endif