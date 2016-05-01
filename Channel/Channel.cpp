





#define IN_DLL
#include "Channel.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../Client/Client.h"
#include <string>
#include <thread>
#include <iostream>


//////////////////////////////////////////////////////////////////////////
// Sender Class
// Accepts message from Client
//

class Sender : public ISender
{
public:
	void postMessage(const Message& msg);
	BlockingQueue<Message> & queue();
private:
	BlockingQueue<Message> sendQ;
};

void Sender::postMessage(const Message& msg)
{
	sendQ.enQ(msg);
}

BlockingQueue<Message>& Sender::queue()
{
	return sendQ;
}


/////////////////////////////////////////////////////////////////////////
// Receiver class
//  accepts message from Channel for Client
//

class Receiver : public IReceiver
{
public: 
	Message getMessage();
	BlockingQueue<Message>&  queue();
private:
	BlockingQueue<Message> recvQ;
};

Message Receiver::getMessage()
{
	return recvQ.deQ();
}

BlockingQueue<Message>& Receiver::queue()
{
	return recvQ;
}


///////////////////////////////////////////////////////////////////////////
// Channel class
//	reads messages from sender and writes to receiver
//


class Channel : public IChannel
{
public:
	Channel(ISender* pSender, IReceiver* pReceiver);
	void start();
	void stop();
private:
	std::thread thread_;
	ISender* pSender_;
	IReceiver* pReceiver_;
	bool stop_ = false;
};


Channel::Channel(ISender* pSender, IReceiver* pReceiver) : pSender_(pSender), pReceiver_(pReceiver) {}


void Channel::start()
{
	std::cout << "\n Channel Starting up";
	thread_ = std::thread(
		[this] {
			Sender* pSender = dynamic_cast<Sender*>(pSender_);
			Receiver* pReceiver = dynamic_cast<Receiver*>(pReceiver_);

			if (pSender == nullptr || pReceiver == nullptr)
			{
				std::cout << "\n failed to start Channel \n\n";
				return;
			}

			BlockingQueue<Message>& sendQ = pSender->queue();
			BlockingQueue<Message>& recvQ = pReceiver->queue();

			Client client;
			std::thread clientThread([&] {
				client.startClient();
			});
			clientThread.detach();

			////mockshit
			//sendQ.enQ("GetFiles");

			while (!stop_)
			{
				//get message from the GUI
				Message msg = sendQ.deQ();

				//get work done from client
				string result = client.doOperation(msg);

				//send back to GUI
				recvQ.enQ(msg);
			}

			std::cout << "\n Client Stopping....\n";
		}
	);
}


void Channel::stop()
{
	stop_ = true;
}


/////////////////////////////////////////////////////////////////////////
// Object Factory class
//
//

ISender* ObjectFactory::createSender()
{
	return new Sender;
}

IReceiver* ObjectFactory::createReceiver()
{
	return new Receiver;
}

IChannel* ObjectFactory::createChannel(ISender* pSender, IReceiver* pReceiver)
{
	return new Channel(pSender, pReceiver);
}



