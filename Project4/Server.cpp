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
#include "../HttpMessage/IService.h"
#include "../HttpMessage/HttpMessage.h"
#include <string>
#include <iostream>

using Show = StaticLogger<1>;


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

		if (msg.size() == 0)
			break;

		HttpMessage httpMessage;
		httpMessage.parseMessage(msg);
		std::string command = httpMessage.findValue("Command");
		std::string client = httpMessage.findValue("FromAddr");
		std::cout << "\nRequest: " << command << "  Client: " << client << "\n";

		
		if (command == "GetFiles")
		{
			std::cout << "\nSending Files....\n";
			continue;
		}
		else if (command == "Check-In")
		{
			std::cout << "\nCheck-In File....\n";
			continue;
		}
		else if (command == "Check-Out")
		{
			std::cout << "\nCheck-Out File....\n";
			continue;
		}
		else if (command == "quit")
		{			
			std::cout << "\nClient closed connection....\n";
			break;
		}
		else
		{
			std::cout << "\nCommand not recognized\n";
			continue;
		}
	}

	std::cout << "\nClosing connection with client\n";
	socket_.shutDown();
	socket_.close();
}

//////////////////////////////////////////////////////////////////////
// Server
//

class Server : IService {
public:
	static BlockingQueue<std::string> recvQ;
	static BlockingQueue<std::string> sendQ;

	Server()
	{
		SocketSystem ss;
	}

	void startListener(std::size_t port, ClientHandler ch)
	{
		sl =  new SocketListener(port, Socket::IP6);
		sl->start(ch);
		std::cout <<"\n Server started listening at port: " << port;		
	}

	void connect(std::size_t port)
	{
		si = new SocketConnecter();
		while (!si->connect("localhost", port))
		{
			Show::write("\n Server waiting to connect");
			::Sleep(100);
		}

		std::cout << "\n Connected to Client: " << port << "\n";
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


//----< test stub >--------------------------------------------------
#ifdef TEST_SERVER

int main()
{
	Show::attach(&std::cout);
	Show::start();	
	try
	{
		SocketSystem ss;
		SocketListener sl(8080, Socket::IP6);
		ClientHandler cp;
		sl.start(cp);

		std::cout << "\n\nServer started at 8080............\n\n";

		std::cout << "Press any Key to break";
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

#endif