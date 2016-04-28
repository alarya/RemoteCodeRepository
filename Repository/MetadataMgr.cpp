




#include "../FileSystem//FileSystem.h"
#include "../XmlDocument/XmlDocument.h"
#include "MetadataMgr.h"
#include <vector>
#include <map>
#include <chrono>

//////////////////////////////////////////////////////////
// class MetadataMgr
//
//

using namespace std;
using namespace XmlProcessing;
using namespace FileSystem;
using dependency = pair<Package, vector<Package>>;
using dependencyList = map<Package, vector<Package>>;
using packageVersionsMap = map<string, vector<string>>;
using sPtr = std::shared_ptr < AbstractXmlElement >;

MetadataMgr::MetadataMgr()
{

}

MetadataMgr::~MetadataMgr()
{

}

#pragma region createMetadata
//--------------------create metadata for a file beign checked-In-------------------------//
string MetadataMgr::createMetaData(Package checkIn, vector<Package> dependencies)
{
	sPtr pRoot = makeTaggedElement("metadata");
	XmlDocument doc(XmlProcessing::makeDocElement(pRoot));
	//add checkedIn package
	sPtr checkInPackage = makeTaggedElement("package");
	sPtr checkInPackageName = makeTaggedElement("name");
	checkInPackageName->addChild(makeTextElement(checkIn.name));
	checkInPackage->addChild(checkInPackageName);
	sPtr checkInPackageVersion = makeTaggedElement("version");
	checkInPackageVersion->addChild(makeTextElement(checkIn.version));
	checkInPackage->addChild(checkInPackageVersion);
	sPtr checkInPackageStatus = makeTaggedElement("status");
	checkInPackageStatus->addChild(makeTextElement("open"));   //default check-In status
	checkInPackage->addChild(checkInPackageStatus);
	pRoot->addChild(checkInPackage);

	sPtr dependencies_ = makeTaggedElement("dependencies");
	for (auto package : dependencies)
	{
		sPtr package_ = makeTaggedElement("package");
		sPtr packageName = makeTaggedElement("name");
		packageName->addChild(makeTextElement(package.name));
		package_->addChild(packageName);
		sPtr packageVersion = makeTaggedElement("version");
		packageVersion->addChild(makeTextElement(package.version));
		package_->addChild(packageVersion);
		sPtr packageStatus = makeTaggedElement("status");
		packageStatus->addChild(makeTextElement(package.status));   
		package_->addChild(packageStatus);

		dependencies_->addChild(package_);
	}
	pRoot->addChild(dependencies_);

	return doc.toString();
}

#pragma endregion

			
#pragma region parsePackageMetaData

//--------------------return parsed metadata of a file----------------------------//
dependency MetadataMgr::parsePackageMetaData(string metadata)
{
	Package package = parsePackageInfo(metadata);

	vector<Package> dependencies = parsePackageDependencies(metadata);

	pair<Package, vector<Package>> parsedResult;
	parsedResult.first = package;
	parsedResult.second = dependencies;

	return parsedResult;
}

//-------------Scan metadata to find package whose information is saved-----------//
Package MetadataMgr::parsePackageInfo(string metadata)
{
	Package package;
	XmlDocument doc(metadata);
	vector<sPtr> packages = doc.element("package").select();
	sPtr package_ = packages[0];

	for (auto packageInfo : package_->children())
	{
		if (packageInfo->tag() == "name")
		{
			vector<sPtr> packageInfoName = packageInfo->children();
			for (auto child : packageInfoName)
				package.name = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
		}
		if (packageInfo->tag() == "status")
		{
			vector<sPtr> packageInfoVersion = packageInfo->children();
			for (auto child : packageInfoVersion)
				package.status = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
		}
		if (packageInfo->tag() == "version")
		{
			vector<sPtr> packageInfoStatus = packageInfo->children();
			for (auto child : packageInfoStatus)
				package.version = child->value().erase(child->value().find_last_not_of(" \n\r\t") + 1);
		}
	}

	return package;
}

//-------------Scan metadata to find dependency packages ------------------------//
vector<Package> MetadataMgr::parsePackageDependencies(string metadata)
{
	vector<Package> dependencyPackages;
	XmlDocument doc(metadata);
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

#pragma endregion

//------------build dependency Map and package map from metadata files----------------------------//
void MetadataMgr::buildDependencyList(dependencyList*& depList, packageVersionsMap*& packVersionMap)
{
	dependencyList dependencies;

	string path = Path::getFullFileSpec(metadataFilesPath);

	vector<string> metadataFiles = Directory::getFiles(path, "*.xml");

	for (auto file : metadataFiles)
	{
		XmlDocument doc(path + file, XmlDocument::file);
		dependency packageDeps = parsePackageMetaData(doc.toString());
		dependencies[packageDeps.first] = packageDeps.second;
		depList->insert(packageDeps);

		if (packVersionMap->find(packageDeps.first.name) == packVersionMap->end())
		{
			pair<string, vector<string>> p;
			vector<string> versions;
			versions.push_back(packageDeps.first.version);
			p.first = packageDeps.first.name; p.second = versions;
			packVersionMap->insert(p);
		}
		else
		{
			vector<string> versions = (*packVersionMap)[packageDeps.first.name];
			versions.push_back(packageDeps.first.version);
			(*packVersionMap)[packageDeps.first.name] = versions;
		}

	}
}


#ifdef TEST_METADATAMGR
int main()
{
	MetadataMgr metadataMgr;

	std::cout << "\n------------------Create metadata----------------------\n";
	Package checkIn;
	checkIn.name = "Package4"; checkIn.status = "open"; checkIn.version = "1";

	vector<Package> dependencies;
	Package dep1;
	dep1.name = "Package3"; dep1.status = "closed"; dep1.version = "1";
	Package dep2;
	dep2.name = "Package1"; dep2.status = "closed"; dep2.version = "2";
	dependencies.push_back(dep1); dependencies.push_back(dep2);

	std::string data = metadataMgr.createMetaData(checkIn, dependencies);
	std::cout << data;

	std::cout << "\n\n---------------parsed metadata-------------------------\n";
	pair < Package, vector<Package>> parseMetadata;
	parseMetadata = metadataMgr.parsePackageMetaData(data);
	std::cout << "\n Package: \n";
	std::cout << "Name: " << parseMetadata.first.name << ", version: " << parseMetadata.first.version << ", status: " << parseMetadata.first.status;
	std::cout << "\nDependencies:- \n";
	for (auto dep : parseMetadata.second)
	{
		std::cout << "\n";
		std::cout << dep.name << ", " << dep.version << ", " << dep.status;
		std::cout << "\n";
	}

	std::cout << "\n\n  -----------------------test build dependency map --------------\n";
	dependencyList* depList = new dependencyList();
	packageVersionsMap* packVersionMap = new packageVersionsMap();
	metadataMgr.buildDependencyList(depList, packVersionMap);
	for (auto dep : *depList)
	{
		std::cout << "Name: " << dep.first.name << ", version: " << dep.first.version << ", status: " << dep.first.status << "\n";
		std::cout << "Dependencies: \n";
		for (auto deps : dep.second)
		{
			std::cout << "\tName: " << deps.name << ", version: " << deps.version << ", status: " << deps.status << "\n";
		}
	}
	cout << "\n-------pack version list-----------\n";
	for (auto packVersions : *packVersionMap)
	{
		std::cout << "\nName: " << packVersions.first << "\n";
		std::cout << "Versions: \n";
		for (auto version : packVersions.second)
		{
			std::cout <<  version << ", ";
		}
	}
	delete depList;
	delete packVersionMap;
}
#endif // TEST_METADATAMGR

