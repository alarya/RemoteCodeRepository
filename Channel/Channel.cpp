/////////////////////////////////////////////////////////////////////////////
// Channel.h - Used to Transfer messages between native client code and    //
//             managed C++ code with GUI                                   //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - Channel reads from sendQ and writes to recvQ                          //
//                                                                         //
// Alok Arya															   //
/////////////////////////////////////////////////////////////////////////////

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
	void postMessage(const HttpMessage& msg);
	BlockingQueue<HttpMessage> & queue();
private:
	BlockingQueue<HttpMessage> sendQ;
};

void Sender::postMessage(const HttpMessage& msg)
{
	sendQ.enQ(msg);
}

BlockingQueue<HttpMessage>& Sender::queue()
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
	HttpMessage getMessage();
	BlockingQueue<HttpMessage>&  queue();
private:
	BlockingQueue<HttpMessage> recvQ;
};

HttpMessage Receiver::getMessage()
{
	return recvQ.deQ();
}

BlockingQueue<HttpMessage>& Receiver::queue()
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

			BlockingQueue<HttpMessage>& sendQ = pSender->queue();
			BlockingQueue<HttpMessage>& recvQ = pReceiver->queue();


			//---------Start the client process------------------//
			Client client;
			std::thread clientThread([&] {
				client.startClient();
			});
			clientThread.detach();

			while (!stop_)
			{
				//get message from the GUI
				HttpMessage msg = sendQ.deQ();

				//get work done from client (enqueues request on client channel receiver queue
				// and wait for the client response)
				HttpMessage result = client.doOperation(msg);

				//send back to GUI 
				recvQ.enQ(result);
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



