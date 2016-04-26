





#include "XMLResponseBodyGenerator.h"
#include "../XmlDocument/XmlDocument.h"
#include "../XmlParser/XmlParser.h"
#include "../XmlElement/XmlElement.h"
#include <vector>

using namespace std;
using namespace XmlProcessing;
using sPtr = std::shared_ptr < AbstractXmlElement >;

///////////////////////////////////////////////////////
// XMLResponsBodyGenerator class
//
//
XMLResponseBodyGenerator::XMLResponseBodyGenerator()
{

}

XMLResponseBodyGenerator::~XMLResponseBodyGenerator()
{

}

//------------------Parsing for GetFiles Command----------------------------------------//

#pragma region GetFilesParse
string XMLResponseBodyGenerator::getResponseBodyForGetFiles(vector<Package> packageList)
{
	sPtr pRoot = makeTaggedElement("packages");
	XmlDocument doc(XmlProcessing::makeDocElement(pRoot));
	for (auto package : packageList)
	{
		sPtr pack = makeTaggedElement("package");
		sPtr packName = makeTaggedElement("name");
		packName->addChild(makeTextElement(package.name));
		pack->addChild(packName);
		sPtr packVersion = makeTaggedElement("version");
		packVersion->addChild(makeTextElement(package.version));
		pack->addChild(packVersion);
		sPtr packStatus = makeTaggedElement("status");
		packStatus->addChild(makeTextElement(package.status));
		pack->addChild(packStatus);
		pRoot->addChild(pack);
	}
	return doc.toString();
}

vector<Package> XMLResponseBodyGenerator::parseResponseBodyForGetFiles(string reponseBody)
{
	vector<Package> parsedPackages;
	XmlDocument doc(reponseBody);
	vector<sPtr> packages = doc.element("packages").descendents().select();

	if (packages.size() > 0)
	{
		for (auto package : packages)
		{
			Package package_;
			vector<sPtr> _p = package->children();
			for (auto packageInfo : _p)
			{
				if (packageInfo->tag() == "name")
				{
					vector<sPtr> packageInfoName = packageInfo->children();
					for (auto child : packageInfoName)
						package_.name = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
				}
				if (packageInfo->tag() == "version")
				{
					vector<sPtr> packageInfoVersion = packageInfo->children();
					for (auto child : packageInfoVersion)
						package_.version = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
				}
				if (packageInfo->tag() == "status")
				{
					vector<sPtr> packageInfoStatus = packageInfo->children();
					for (auto child : packageInfoStatus)
						package_.status = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
				}
			}
			if (package_.name != "")
				parsedPackages.push_back(package_);
		}
	}
	return parsedPackages;
}
#pragma endregion

//------------------Parsing for Check-In Command----------------------------------------//

#pragma region CheckInParse

string XMLResponseBodyGenerator::getRequestBodyForCheckIn(CheckInPackage checkInPackage, vector<Package> dependencies)
{
	sPtr pRoot = makeTaggedElement("Check-In");
	XmlDocument doc(XmlProcessing::makeDocElement(pRoot));
		sPtr package = makeTaggedElement("package");
			sPtr packageName = makeTaggedElement("name");
			packageName->addChild(makeTextElement(checkInPackage.name));
			package->addChild(packageName);
			sPtr packageCppLength = makeTaggedElement("cppfilelength");
			packageCppLength->addChild(makeTextElement(checkInPackage.cppFileLength));
			package->addChild(packageCppLength);
			sPtr packageHLength = makeTaggedElement("hfilelength");
			packageHLength->addChild(makeTextElement(checkInPackage.hFileLength));
			package->addChild(packageHLength);			
	    pRoot->addChild(package);

		sPtr dependencies_ = makeTaggedElement("dependencies");
		for (auto dep : dependencies)
		{
			sPtr depPackage = makeTaggedElement("package");
				sPtr depPackageName = makeTaggedElement("name");
				depPackageName->addChild(makeTextElement(dep.name));
				depPackage->addChild(depPackageName);
				sPtr depPackageVersion = makeTaggedElement("version");
				depPackageVersion->addChild(makeTextElement(dep.version));
				depPackage->addChild(depPackageVersion);
				sPtr depPackageStatus = makeTaggedElement("status");
				depPackageStatus->addChild(makeTextElement(dep.status));
				depPackage->addChild(depPackageStatus);
			dependencies_->addChild(depPackage);
		}
		pRoot->addChild(dependencies_);

	return doc.toString();
}

vector<Package> XMLResponseBodyGenerator::parseRequestBodyForDependenciesInCheckIn(string requestBody)
{
	vector<Package> dependencyPackages;
	XmlDocument doc(requestBody);
	vector<sPtr>  dependencies = doc.element("dependencies").descendents().select();
	if (dependencies.size() > 0)
	{
		for (auto package : dependencies)
		{
			Package package_;
			vector<sPtr> _p = package->children();
			for (auto packageInfo : _p)
			{
				if (packageInfo->tag() == "name")
				{
					vector<sPtr> packageInfoName = packageInfo->children();
					for (auto child : packageInfoName)
						package_.name = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
				}
				if (packageInfo->tag() == "version")
				{
					vector<sPtr> packageInfoVersion = packageInfo->children();
					for (auto child : packageInfoVersion)
						package_.version = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
				}
				if (packageInfo->tag() == "status")
				{
					vector<sPtr> packageInfoStatus = packageInfo->children();
					for (auto child : packageInfoStatus)
						package_.status = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
				}
			}
			if (package_.name != "")
				dependencyPackages.push_back(package_);
		}
	}
	return dependencyPackages;
}

CheckInPackage XMLResponseBodyGenerator::parseRequestBodyForCheckInPackage(string requestBody)
{
	CheckInPackage checkInPackage;
	XmlDocument doc(requestBody);
	vector<sPtr> package = doc.element("package").select();
	sPtr package_ = package[0];

	for (auto packageInfo : package_->children())
	{
		if (packageInfo->tag() == "name")
		{
			vector<sPtr> packageInfoName = packageInfo->children();
			for (auto child : packageInfoName)
				checkInPackage.name = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
		}
		if (packageInfo->tag() == "cppfilelength")
		{
			vector<sPtr> packageInfoVersion = packageInfo->children();
			for (auto child : packageInfoVersion)
				checkInPackage.cppFileLength = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
		}
		if (packageInfo->tag() == "hfilelength")
		{
			vector<sPtr> packageInfoStatus = packageInfo->children();
			for (auto child : packageInfoStatus)
				checkInPackage.hFileLength = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
		}
	}

	return checkInPackage;
}

#pragma endregion

#ifdef TEST_XMLRESPONSEBODYGEN

int main()
{
	XMLResponseBodyGenerator xml;

	std::cout << "\n---------testing getResponseBodyForGetFiles-----------\n\n";
	vector<Package> packages;
	Package package1; package1.name = "Package1"; package1.version = "1"; package1.status = "open";
	packages.push_back(package1);
	Package package2; package2.name = "Package2"; package2.version = "2"; package2.status = "open";
	packages.push_back(package2);
	Package package3; package3.name = "Package3"; package3.version = "2"; package3.status = "closed";
	packages.push_back(package3);

	std::string  getFilesResponseBody = xml.getResponseBodyForGetFiles(packages);
	std::cout << getFilesResponseBody;

	std::cout << "\n---------testing parseResponseBodyForGetFiles-----------\n\n";
	vector<Package> parsedPackages =  xml.parseResponseBodyForGetFiles(getFilesResponseBody);
	for (auto package : parsedPackages)
	{
		cout << "Package: Name - " << package.name << ", version - " << package.version << " status - " << package.status << '\n';
	}

	std::cout << "\n\n\n";
	std::cout << "\n---------testing getRequestBodyForCheckIn-----------\n\n";
	CheckInPackage checkInPackage; vector<Package> dependencies;
	checkInPackage.name = "Package3"; checkInPackage.cppFileLength = "1024"; checkInPackage.hFileLength = "768";
	dependencies = packages;
	
	std::string getRequestBodyForCheckIn = xml.getRequestBodyForCheckIn(checkInPackage, dependencies);
	std::cout << getRequestBodyForCheckIn;

	std::cout << "\n---------testing parseRequestBodyForDependenciesInCheckIn-----------\n";
	vector<Package> deps = xml.parseRequestBodyForDependenciesInCheckIn(getRequestBodyForCheckIn);
	CheckInPackage checkInPackageParsed = xml.parseRequestBodyForCheckInPackage(getRequestBodyForCheckIn);
	std::cout << "Check-In Package \n";
	std::cout << "name: " << checkInPackageParsed.name << "\n";
	std::cout << "CppFileLength: " << checkInPackageParsed.cppFileLength << "\n";
	std::cout << "hFileLength: " << checkInPackageParsed.hFileLength << "\n";
	std::cout << "------------dependencies--------------\n";
	for (auto package : deps)
	{
		cout << "Package: Name - " << package.name << ", version - " << package.version << " status - " << package.status << '\n';
	}
}

#endif