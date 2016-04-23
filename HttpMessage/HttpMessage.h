#pragma once




#include <string>
#include <vector>


/*
	Sample structure of Http Message
	----------------------------------
	Command : "SomeCommand"
	ToAddr: 127.0.0.1:8080
	FromAddr: 127.0.0.1:8081
	content_length: 10
	//-----This is a space-------//
	Body....................
	............
	----------------------------------
*/
class HttpMessage
{
public:
	HttpMessage();
	~HttpMessage();

	using byte = std::string;
	using Name = std::string;
	using Value = std::string;
	using Attribute = std::pair<Name, Value>;
	using Attributes = std::vector<Attribute>;
	using Body = std::vector<byte>;
	using Message = std::string;

	//message attributes
	void addAttribute(const Attribute& attrib);
	Value findValue(const Name& name);
	Attributes attributes();

	//message body
	void setBody(byte buffer[], size_t bufLen);
	void HttpMessage::setBody(Body& body);
	Body body();
	size_t bodyLength();

	//HttpMessage
	Message buildMessage();
	void parseMessage(const Message& msg);

private:
	Attributes attributes_;
	Body body_;
	//void printHttpMessage();
};

