#ifndef CLIENT_H
#define CLIENT_H
/////////////////////////////////////////////////////////////////////////
// Client.h - Starts the client and starts request/response threads    //
//                                                                     //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016           //
// Application: OOD Project #4                                         //
// Platform:    Visual Studio 2015, Dell XPS 8900, Windows 10 pro      //
/////////////////////////////////////////////////////////////////////////
/*
*
* Module Operations:
* ==================
* This package implements a client that receives sends requests to a server 
* The clients send Http styles messages for requests and server
* responds with Http styles messages too. The client starts a Sender thread
* and a client handler thread to respond to responses from the server
*
*
* Required Files:
*   HttpMessage.h, HttpMessage.cpp
*   XMLResponseBodyGenerator.h, XMLResponseBodyGenerator.cpp
*   FileSystem.h, FileSystem.cpp
*   Sockets.h, Sockets.cpp, Cppll-BlockingQueue.h
*   Logger.h, Logger.cpp, Cpp11-BlockingQueue.h
*   Utilities.h, Utilities.cpp
*
*
*
* Public Interface:
* =================
* 
* ClientHandler cl;              //instantiates a new clientHandler
*								   is a callable object to handle client requests
*								   started on an independent thread
*							       
* SendHandler se;               //instantiates a new SendHandler
*								   is a callable object to handle sending request and it's related tasks(like file transfer)
*								   started on an independent thread
*
* Client client;
* client.startClient();         // starts the client (listener and sender)
* client.doOperation();         // enques operation for the client to execute
*
*
* Build Command:
* ==============
* Build Command: devenv Project4.sln /rebuild debug /project Client/Client.vcxproj
*
* Maintenance History:
* ====================
*
* ver 1.0 : 2 May 2016
* - first release
*/


#include "../Logger/Cpp11-BlockingQueue.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include <string>

using namespace std;


class ClientHandler
{
public:
	void operator()(Socket& socket_);
	BlockingQueue<HttpMessage>& RecvQ();
private:
	static BlockingQueue<HttpMessage> recvQ;      //shared by all client handlers 
	void GetFiles(Socket& si, HttpMessage httpMessage);
	void CheckIn(Socket& si, HttpMessage httpMessage);
	void GetOpenCheckIns(Socket& si, HttpMessage httpMessage);
	void CloseOpenCheckIn(Socket& si, HttpMessage httpMessage);
	void CheckOut(Socket& si, HttpMessage httpMessage);
	void checkCommand(string command, Socket& socket_, HttpMessage httpMessage);
};




class SenderHandler
{
public:
	void operator()(BlockingQueue<HttpMessage>& sendQ, SocketConnecter& si);
private:
	void GetFiles(SocketConnecter& si, HttpMessage httpMessage);
	void CheckIn(SocketConnecter& si, HttpMessage httpMessage);
	void GetOpenCheckIns(SocketConnecter& si, HttpMessage httpMessage);
	void CloseOpenCheckIn(SocketConnecter& si, HttpMessage httpMessage);
	void CheckOut(SocketConnecter& si, HttpMessage httpMessage);
};


class Client {

public:
	void startClient();
	HttpMessage doOperation(HttpMessage command) ;
private:
	BlockingQueue<HttpMessage> channelSendQ;
	BlockingQueue<HttpMessage> channelRecvQ;
};



#endif // !CLIENT_H
