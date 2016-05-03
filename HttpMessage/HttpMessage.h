#ifndef HTTPMESSAGE_H
#define HTTPMESSAGE_H
/////////////////////////////////////////////////////////////////////////////
// HttpMessage.h - Defines and HTTP like Message class for communication   //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio Enterprise 2015                  //
// Platform:    ThinkPad L440, Core i7-4712MQ                              //
// Author:      Alok Arya                                                  //
//              (315) 728-0088, alarya@syr.edu                             //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* The HttpMessage class provides a way of sending request/response over a network
* It comprises of a set of attributes which are basically a key, value pair.
* It also provides a body for sending other accompanying data, which can be plain
* string or structured like XML.
* The HttpMessage can be converted to a string with Key,Value attributes being seperated by
* each other by a "\n" and before the body starts there are 2 "\n"
*
* Public Interface:
* =================
* HttpMessage httpMessage;           //create a new HttpMessage
*
* pair<string,string> attr;
* attr.first = "Key"; attr.second = "Value"  
* httpMessage.addAttribute(attr)              //add a new attribute to the httpMessage
* 
* string Value = httpMessage("Key")           //return the value for a given key, empty string if key not present
*
* vector<pair<string,string>> attributes ; 
* attributes = httpMessage.attributes()       //returns all the attributes in the httpMessage
*
* string body = "This is a test body"
* httpMessage.setBody(body);				  //Set the body of the Http Message by passing a string
*
* vector<char> body ;
* httpMessage.setBody(body);                  //set the body of the Http Message by passing a vector of chars
*
* vector<char> body ;
* body = httpMessage.body();                  //return the body of http message as a vector of char
*
* string body;
* body = httpMessage.getBody();               //return the body of http message as a string
*
* size_t body_Length;
* body_Length = httpMessage.bodyLength();     //return the length of the body
*
* string httpMessageStr;
* httpMessageStr = httpMessage.buildMessage();// returns a string version of the built message
*
* HttpMessage parsed;
* string msg = "Key1:Value1\nKey2:Value2\n\nThis is the contents of the body";
* parsed.parseMessage(msg);                   // builds back the HttpMessage components from the string version of it
*
* parsed.printMessage();                      // print the companents of an http message on the console
*
* Required Files:
* ===============
* HttpMessage.h, HttpMessage.cpp
*
* Build Command:
* ==============
* Build Command: devenv Project4.sln /rebuild debug /project HttpMessage/HttpMessage.vcxproj
*
* Maintenance History:
* ====================
*
* ver 1.0 : 2 May 2016
* - first release
*/

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

	using byte = char;
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
	void setBody(std::string msg);
	void HttpMessage::setBody(Body& body);
	Body body();
	std::string getBody();
	size_t bodyLength();

	//HttpMessage
	Message buildMessage();
	void parseMessage(const Message& msg);

	//print
	void printMessage();

private:
	Attributes attributes_;
	Body body_;
	//void printHttpMessage();
};


#endif