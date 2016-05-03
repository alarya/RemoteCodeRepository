#ifndef CHANNEL_H
#define CHANNEL_H
/////////////////////////////////////////////////////////////////////////////
// Channel.h - Used to Transfer messages between native client code and    //
//             managed C++ code with GUI                                   //
// - build as DLL to show how C++\CLI client can use native code channel   //
// - Channel reads from sendQ and writes to recvQ                          //
//                                                                         //
// Alok Arya															   //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations :
* == == == == == == == == ==
* 
*
* Required Files :
*	HttpMessage.h, HttpMessage.cpp
*  
*
*
*
* Public Interface :
* == == == == == == == == =
*
* ISender
*	- postMessage - post message from the GUI to the channel queue
*
* IReceiver
*	- getMessage - get the message posted by the client as a response
*
* Build Command :
* == == == == == == ==
* Build Command : devenv Project4.sln /rebuild debug/ project Channel/Channel.vcxproj
*
* Maintenance History :
*== == == == == == == == == ==
*
* ver 1.0 : 2 May 2016
* -first release
*/


#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
#include "../HttpMessage/HttpMessage.h"

struct ISender
{
	virtual	void postMessage(const HttpMessage& msg) = 0;
};

struct IReceiver
{
	virtual HttpMessage getMessage() = 0;
};

struct IChannel
{
	public:
		virtual void start() = 0;
		virtual void stop() = 0;
};

extern "C" {

	struct ObjectFactory
	{
		DLL_DECL ISender* createSender();
		DLL_DECL IReceiver* createReceiver();
		DLL_DECL IChannel* createChannel(ISender* pSender, IReceiver* pReceiver);
	};

}
#endif // !CHANNEL_H
