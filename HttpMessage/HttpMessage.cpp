



#include "HttpMessage.h"
#include <iostream>
#include <sstream>



/////////////////////////////////////////////
//HttpMessage Class
//
/////////////////////////////////////////////

using byte = std::string;
using Name = std::string;
using Value = std::string;
using Attribute = std::pair<Name, Value>;
using Attributes = std::vector<Attribute>;
using Body = std::vector<byte>;
using Message = std::string;

//-------------Constructor--------------------//
HttpMessage::HttpMessage()
{
}

//-------------Destructor---------------------//
HttpMessage::~HttpMessage()
{
}

//------------Adds a Key Value pair to Http Message------//
void HttpMessage::addAttribute(const Attribute& attrib)
{
	attributes_.push_back(attrib);
}

//-----------Find value of an existing attribute---------//
Value HttpMessage::findValue(const Name& name)
{
	for (auto pair : attributes_)
	{
		if (pair.first == name)
			return pair.second;
	}

	return "";
}

//---------returns a reference to all Key Value pairs----//
Attributes HttpMessage::attributes()
{

	return attributes_ ;
}

//--------set Body of the message ----------------//
void HttpMessage::setBody(byte buffer[], size_t bufLen)
{
	for (size_t i = 0; i < bufLen; i++)
	{
		body_.push_back(buffer[i]);
	}
}

//--------set Body of the message ----------------//
void HttpMessage::setBody(Body& body)
{
	for (auto byte : body)
	{
		body_.push_back(byte);
	}
}

//-------returns a reference the body of msg---//
Body HttpMessage::body()
{
	return body_;
}

//-------returns the length of body in msg---------//
size_t HttpMessage::bodyLength()
{
	size_t len = 0;
	for (auto byte : body_)
	{
		len += byte.length();
	}
	return len;
}

//------builds and returns the message ----------//
Message HttpMessage::buildMessage()
{
	Message msg;
	for (auto attribute : attributes_)
	{
		msg += attribute.first;
		msg += ":";
		msg += attribute.second;
		msg += "\n";
	}
	msg += "\n";
	for (auto byte : body_)
	{
		msg += byte;
	}
	return msg;
}

//-----parse an Http Message---------------------//
void HttpMessage::parseMessage(const Message& msg)
{
	if (msg.size() <= 0)
		return;

	std::string line;
	
	size_t counter = 0;
	std::string first;
	std::string second;
	//scan attributes
	while(true)
	{

		if (msg[counter] == '\n')
		{
			first = "";
			second = "";
			size_t i = 0;
			while (line[i] != ':')
			{
				first += line[i];
				i++;
			}
			i++;
			while (i < line.size())
			{
				second += line[i];
				i++;
			}
			Attribute attr;
			attr.first = first; attr.second = second;
			addAttribute(attr);
			line = "";

			if (msg[counter + 1] == '\n')  //it was last attribute
				break;
		}
		else
		{
			line += msg[counter];
		}
		counter++;
	}
	counter++;
	counter++;
	//body starts
	Body body;
	byte byte_;
	while (counter < msg.size() )
	{
		if (msg[counter] == ' ')
		{
			body.push_back(byte_);
			byte_ = "";
		}
		else
		{
			byte_ += msg[counter];
		}

		counter++;
	}
	if (byte_.size() != 0)
		body.push_back(byte_);

	setBody(body);
}





#ifdef TEST_HTTPMESSAGE

int main()
{
	//Testing building an Http Message
	std::cout << "\n---------Testing Building an Http Message-----------\n";
	HttpMessage httpMessage;
	Attribute attrib1;
	attrib1.first = "Command";
	attrib1.second = "GetFiles";
	httpMessage.addAttribute(attrib1);
	std::cout << "\nAdded attribute - " << attrib1.first << ":" << attrib1.second << "\n";

	Attribute attrib2;
	attrib2.first = "ToAddr";
	attrib2.second = "127.0.0.1:8080";
	httpMessage.addAttribute(attrib2);
	std::cout << "\nAdded attribute - " << attrib2.first << ":" << attrib2.second << "\n";

	Attribute attrib3;
	attrib3.first = "FromAddr";
	attrib3.second = "127.0.0.1:8081";
	httpMessage.addAttribute(attrib3);
	std::cout << "\nAdded attribute - " << attrib3.first << ":" << attrib3.second << "\n";

	Body msg;
	msg.push_back("This"); msg.push_back("is"); msg.push_back("a"); msg.push_back("messsage");
	httpMessage.setBody(msg);

	Attribute attrib4;
	attrib4.first = "content_length";
	std::stringstream size;
	size << httpMessage.bodyLength();
	attrib4.second = size.str() ;
	httpMessage.addAttribute(attrib4);
	std::cout << "\nAdded attribute - " << attrib4.first << ":" << attrib4.second << "\n";

	Message builtMessage = httpMessage.buildMessage();
	std::cout << "\nThe built Message\n";
	std::cout << "\n-------------------------------------\n";
	std::cout << builtMessage;
	std::cout << "\n-------------------------------------\n";

	std::cout << "\n---------Testing parsing an Http Message-----------\n";
	HttpMessage httpMessageParse;
	httpMessageParse.parseMessage(builtMessage);
	std::cout << "Printing the attributes.........";
	for (auto attr : httpMessageParse.attributes() )
	{
		std::cout << "\n" << "Name: " << attr.first << ", Value: " << attr.second;
	}
	std::cout << "\nPrinting the body............\n";
	for (auto byte_ : httpMessageParse.body())
	{
		std::cout << byte_;
	}

	std::cout << "\n---------Testing getting value of an attribute-----------\n";
	std::cout << "attribute: Command \n";
	std::cout << "Value: " << httpMessageParse.findValue("Command");
}

#endif // TEST_HTTPMESSAGE


