#ifndef CHANNEL_H
#define CHANNEL_H




#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>

using Message = std::string;

struct ISender
{
	virtual	void postMessage(const Message& msg) = 0;
};

struct IReceiver
{
	virtual Message getMessage() = 0;
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
