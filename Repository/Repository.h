#ifndef REPOSITORY_H
#define REPOSITORY_H


#include "MetadataMgr.h"
#include <vector>
#include <map>

using namespace std;

class Repository {

	using dependency = pair<Package, vector<Package>>;
	using dependencyList = map<Package, vector<Package>>;
	using packageVersionsMap = map<string, vector<string>>;
	using PackageList = vector<Package>;	

public: 
	Repository();
	~Repository();

	//main repository functions
	PackageList getPackageList();
	string checkInPackage(Package package, PackageList dependencies); //return status string as response
	PackageList getOpenCheckIns();
	string closeOpenCheckIn(Package package); //return status string as response
	PackageList checkOutPackageDependencies(Package package, bool includeDependencies);   //returns dependencies of checkOutPackage

private:
	dependencyList* packagesAndDependencies;    //maintains list of packages with it's dependencies
	packageVersionsMap* packagesAndVersions;

	string packageDirPath = "../root";
	string MetaDataPath = "../root/Metadata";
	string socketUploadPath = "../root/socket_uploads";

	bool packageExists(string packageName);
	string latestVersionOfPackage(string packageName);
	bool isLatestVersionofPackageClosed(string packageName);
	void updatePackageAndVersionsMap(Package package);
};

#endif
