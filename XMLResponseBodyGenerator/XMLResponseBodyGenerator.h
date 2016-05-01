#ifndef XMLRESPONSEBODYGENERATOR_H
#define XMLRESPONSEBODYGENERATOR_H



#include "../XmlElement/XmlElement.h"
#include "../XmlDocument/XmlDocument.h"
#include"../XmlParser/XmlParser.h"
#include <iostream>
#include <map>
#include <vector>

using namespace std;
using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement >;

struct Package {
	string name;
	string version;
	string status;

	// uniquely identfy a package by its <name,version>
	bool operator<(const Package &p) const {
		return name < p.name || (name ==p.name &&  version < p.version) ;
	}
};

struct CheckInPackage {
	string name;
	string cppFileLength;
	string hFileLength;
};

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

private:
	Package parsePackageElement(sPtr package);
};


#endif
