#ifndef XMLRESPONSEBODYGENERATOR_H
#define XMLRESPONSEBODYGENERATOR_H
//////////////////////////////////////////////////////////////////////////////////////////
// XMLResponseBodyGenerator.h - Used for generating and parsing XML body used for       //
//				                HttpMessages                                            //
// ver 1.0                                                                              //
// -----------------------------------------------------------------------              //
// Language:    Visual C++, Visual Studio Enterprise 2015                               //
// Platform:    ThinkPad L440, Core i7-4712MQ                                           //
// Author:      Alok Arya                                                               //
//              (315) 728-0088, alarya@syr.edu                                          //
//////////////////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This XMLResponeBodyGenerator class is an application specific class. It is used to 
* make the generation and parsion og XML data easy. The XML data used is for sending information
* about packages and it's dependencies.
*
* Public Interface:
* =================
* XMLResponseBodyGenerator xml;           //create a new xml generator/parser
*
* vector<Package> packageList;
* string xmlBody = xml.getResponseBodyForGetFiles();          // generates xml body for GetFiles request
* xmlBody:
*   <packages>
		<package>
			<name>Package1</name>
			<version>2</version>
			<status>open</status>
		</package>
		<package>
		    ....
		</package>
    </packages>
*
*
* vector<Package> parsedPackageList;
* parsedPackageList = xml.parseResponseBodyForGetFiles(xmlBody);    //  get back a list of packages  from xml
*
*
*
* Package checkInPackage;
* vector<Package> dependencies;
* string xmlBody = xml.getResponseBodyForCheckIn(checkInPackage, dependencies);  // get back the XML body for check-In requests          
* xmlBody:
*	<Check-In>
*		<package>
			<name>Package5</name>
			<version>1</version>
			<status> </status>
		</package>
		<dependencies>
			<package>
				<name>Package4</name>
				<version>2</version>
				<status> </status>
			</package>
			.
			.
			.
		</dependencies>
*   </Check-In>
* Package parseCheckInPackage = xml.parseRequestBodyForCheckInPackage(xmlBody);                 //returns the package which needs to be checked In
* vector<Package> parseDependencies = xml.parseRequestBodyForDependenciesInCheckIn(xmlBody);    //returns the list packages on which the check-In package
*																						          needs to depend on
*
* Package closeCheckInPackage;
* string xmlBody = xml.getRequestBodyForCloseCheckIn(closeCheckInPackage);             // get the xml body for close check-In requests
* xmlBody:
*	<CloseCheck-In>
*		<package>
			<name>Package5</name>
			<version>1</version>
			<status> </status>
		</package>
*   </CloseCheck-In>
* 
* Package parseCloseCheckInPackage ;
* parseCloseCheckInPackage = xml.parseRequestBodyForCloseCheckInPackage(xmlBody);               //returns the package which needs to be marked closed
*
*
* Package checkOutPackage;
* xmlBody:
*	<Check-Out>
		<package>
			<name>Package2</name>
			<version>1</version>
			<status> </status>
		</package
		<dependencies>      //empty when client sends request, server populates this with dependencies if include dependencies attributes
		</dependencies>		  present in the request							
*   </Check-Out>
*
* Package parseCheckOutPackage;
* parseCheckOutPackage = xml.parseRequestBodyForCheckOutPackage(xmlBody);                        //returns the package to be checkedout for checkout
* vector<Package> dependencies;
* dependencies = xml.parseRequestBodyForDependenciesInCheckOut(xmlBody);						 //returns the dependencies of the check-out package
*
*
* Required Files:
* ===============
* XMLResponseBodyGenerator.h, XMLResponseBodyGenerator.cpp
* itokcollection.h
* Tokenizer.h, Tokenizer.cpp
* XmlDocument.h, XmlDocument.cpp
* XmlElementParts.h, xmlElementParts.cpp
* XmlParser.h, XmlParser.cpp
*
* Build Command:
* ==============
* Build Command: devenv Project4.sln /rebuild debug /project XMLResponseBodyGenerator/XMLResponseBodyGenerator.vcxproj
*
* Maintenance History:
* ====================
*
* ver 1.0 : 2 May 2016
* - first release
*/


#include "../XmlElement/XmlElement.h"
#include "../XmlDocument/XmlDocument.h"
#include"../XmlParser/XmlParser.h"
#include <iostream>
#include <map>
#include <vector>

using namespace std;
using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement >;


//-----------Package Info--------------------------------------------//
struct Package {
	string name;
	string version;
	string status;

	// uniquely identfy a package by its <name,version>
	bool operator<(const Package &p) const {
		return name < p.name || (name ==p.name &&  version < p.version) ;
	}
};


//----------XMLResponseBodyGenerator---------------------------------//
class XMLResponseBodyGenerator {
	
public:
	XMLResponseBodyGenerator();
	~XMLResponseBodyGenerator();

	string getResponseBodyForGetFiles(vector<Package> packageList);
	vector<Package> parseResponseBodyForGetFiles(string reponseBody);

	string getRequestBodyForCheckIn(Package checkInPackage, vector<Package> dependencies);
	vector<Package> parseRequestBodyForDependenciesInCheckIn(string requestBody);
	Package parseRequestBodyForCheckInPackage(string requestBody);

	string getRequestBodyForCloseCheckIn(Package CloseCheckInPackage);
	Package parseRequestBodyForCloseCheckInPackage(string requestBody);

	string getRequestBodyforCheckOut(Package checkOutPackage, vector<Package> dependencies);
	vector<Package> parseRequestBodyForDependenciesInCheckOut(string requestBody);
	Package parseRequestBodyForCheckOutPackage(string requestBody);

private:
	Package parsePackageElement(sPtr package);
};


#endif
