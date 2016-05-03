#ifndef METADATAMGR_H 
#define METADATAMGR_H
/////////////////////////////////////////////////////////////////////////////////
// MetadataMgr.h - Manages creating/parsing metadata for packages              //
// ver 1.0                                                                     //
// -----------------------------------------------------------------------     //
// Language:    Visual C++, Visual Studio Enterprise 2015                      //
// Platform:    ThinkPad L440, Core i7-4712MQ                                  //
// Author:      Alok Arya                                                      //
//              (315) 728-0088, alarya@syr.edu                                 //
/////////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* MetadataMgr module helps the repository with managing the metadata for the packages in the repository
* Functions include:-
*		creating metadata for a new check-In
*		building lookup tables for the repository for easy lookup of stored packages and their dependencies
*		parse the metadata of an existing package to read it's information.
*
* Public Interface:
* =================
* 
* MetadataMgr metadataMgr;        //create a new instance of MetadataMgr
*
* Package checkIn; checkIn.name = "Package2" ; checkIn.version = "1";
* vector<Package> dependencies; 
* Package dep1 ; dep1.name = "Package1" ; dep1.version = "2";
* dependencies.push_back(dep1);
* string metadata = metadataMgr(Package, dependencies);         //creates and xml metadata info and returns as a string
*
* map<Package, vector<Package>> dependencyMap;  //Key- Package, Value - vector of its dependency packages
* map<string, vector<string>> packageVersionsMap;  //key - package name, values- versions of that package
* buildDependencyList(&dependencyMap, &packageVersionsMap);      //builds a map by reading all the metadatafiles
* 
* 
* pair<Package, vector<Package>> dependencies;
* dependencies = metadataMgr.parsePackageMetadata(xmlMetadata);      // parses metadata for a single file
*
* Required Files:
* ===============
* MetadaMgr.cpp
* XMLResponseBodyGenerator.h, XMLResponseBodyGenerator.cpp
* FileSystem.h, FileSystem.cpp
* XmlDocument.h, XmlDocument.cpp
* itokcollection.h
* Tokenizer.h, Tokenizer.cpp
* XmlElementParts.h, xmlElementParts.cpp
* XmlParser.h, XmlParser.cpp
*
* Build Command:
* ==============
* Build Command: devenv Project4.sln /rebuild debug /project Repository/Repository.vcxproj
*
* Maintenance History:
* ====================
*
* ver 1.0 : 2 May 2016
* - first release
*/

#include "../XMLResponseBodyGenerator/XMLResponseBodyGenerator.h"
#include <iostream>
#include <map>
#include <vector>

class MetadataMgr {

	using dependency = pair<Package, vector<Package>>;
	using dependencyList = map<Package, vector<Package>>;
	using packageVersionsMap = map<string, vector<string>>;
public:
	MetadataMgr();
	~MetadataMgr();

	string createMetaData(Package checkIn, vector<Package> dependencies);
	void buildDependencyList(dependencyList*& depList, packageVersionsMap*& packVersionMap);
	dependency parsePackageMetaData(string metadata);
private:	
	Package MetadataMgr::parsePackageInfo(string metadata);
	vector<Package> MetadataMgr::parsePackageDependencies(string metadata);
	Package MetadataMgr::parsePackageElement(sPtr package);
	string metadataFilesPath = "../root/Metadata/";
};

#endif