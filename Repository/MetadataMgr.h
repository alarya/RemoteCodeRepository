#ifndef METADATAMGR_H 

#define METADATAMGR_H

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
	string metadataFilesPath = "../root/Metadata/";
};

#endif