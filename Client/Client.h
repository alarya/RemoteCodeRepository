#ifndef CLIENT_H
#define CLIENT_H



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
	string doOperation(string command) ;
private:
	BlockingQueue<string> channelQ;
};



#endif // !CLIENT_H
