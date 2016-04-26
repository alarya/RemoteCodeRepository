#ifndef XMLRESPONSEBODYGENERATOR_H
#define XMLRESPONSEBODYGENERATOR_H



#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct Package {
	string name;
	string version;
	string status;
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

	string getRequestBodyForCheckIn(CheckInPackage checkInPackage, vector<Package> dependencies);
	vector<Package> parseRequestBodyForDependenciesInCheckIn(string requestBody);
	CheckInPackage parseRequestBodyForCheckInPackage(string requestBody);
};


#endif
