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
private:
	void handleGetFiles(Socket& socket_);
	bool handleCheckIn(Socket& socket_);
	void handleCheckOut(Socket& socket_);
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
			handleGetFiles(socket_);			
			continue;
		}
		else if (command == "Check-In")
		{
			if (!handleCheckIn(socket_))
				break;
			continue;
		}
		else if (command == "Check-Out")
		{
			handleCheckOut(socket_);
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




//////////////////////////////////////////////////////////////////////
// Send Messages class (Reply to requests)
//
class Sender
{
public:
	void operator()(BlockingQueue<std::string>& sendQ);
};

void Sender::operator()(BlockingQueue<std::string>& sendQ)
{

}



//----< test stub >--------------------------------------------------
#ifdef TEST_SERVER

int main()
{
	Show::attach(&std::cout);
	Show::start();	
	try
	{
		//---------Queues for communication --------------------//
		BlockingQueue<std::string> recvQ;
		BlockingQueue<std::string> sendQ;

		//---------Start server listener------------------------//
		SocketSystem ss;
		SocketListener sl(8080, Socket::IP6);
		ClientHandler cp;
		sl.start(cp);

		std::cout << "\n\nServer started at 8080............\n\n";
		std::cout << "Press any Key to break";

		//-------start thread to send reply messages from sendQ--//
		//Sender sender;
		//std::thread sendThread(sender,std::ref(sendQ));
		//sendThread.detach();

		//Main thread blocks on recvQ 
		//while (true)
		//{
		//	std::string response= recvQ.deQ();


		//}

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