#ifndef REPOSITORY_H
#define REPOSITORY_H
///////////////////////////////////////////////////////////////////////////////////
// Repository.h - Manages the check-In, Check-Out and access of server packages  //
// ver 1.0                                                                       //
// -----------------------------------------------------------------------       //
// Language:    Visual C++, Visual Studio Enterprise 2015                        //
// Platform:    ThinkPad L440, Core i7-4712MQ                                    //
// Author:      Alok Arya                                                        //
//              (315) 728-0088, alarya@syr.edu                                   //
///////////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* The Repository is used for following functions
*           - Helps in Check-In of a new package
*           - Get a list of packages in the server.
*           - Close a new Check-In package which is "open"
*           - Manages versioning of the packages
*           - Check-out a package with optionally including it's dependencies too
* 
* Public Interface:
* =================
*
* Repository repo;                                          //instantiates an instance of the repository
* 
* vector<Package> Packages = repo.getPackageList();         //returns all the packages in the server
* 
* Package checkInPackage;
* checkInPackage.name = "Package10";
* vector<Packages> dependencies;
* Package dep1;
* dep1.name = "Package4" ; dep1.version = "3";
* dependencies.push_back(dep1);
* string status = repo.checkInPackage(checkInPackage, dependencies);     //checks in a new package with specified dependencies, and returns the status
*
* vector<Package> openPackages = repo.getOpenCheckIns();                 //returns a list of only open packages
*
* Pacakge closePackage;
* closePackage.name = "Package10"; closePackage.version = "1" ;
* string status = repo.closeOpenCheckIn(closePackage);                  // marks the status of the specified packages as "closed"
* 
* vector<Package> checkoutList = repo.checkOutPackageDependencies(package,true);  //returns a list of packages needed to check-out the specified package
*																				    2nd param determines of dependencies should be added as well
*																					
*
* Required Files:
* ===============
* Repository.cpp
* MetadataMgr.h, MetadaMgr.cpp
* XMLResponseBodyGenerator.h, XMLResponseBodyGenerator.cpp
* FileMgr.h, FileMgr.cpp
* FileSystem.h, FileSystem.cpp
*
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
