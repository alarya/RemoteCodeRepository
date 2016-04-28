



#include "MetadataMgr.h"
#include "Repository.h"
#include "FileMgr.h"
#include <string>

using dependency = pair<Package, vector<Package>>;
using dependencyList = map<Package, vector<Package>>;
using packageVersionsMap = map<string, vector<string>>;
using PackageList = vector<Package>;

/////////////////////////////////////////////////////////
// Repository class
// Maintainer of server repository of packages
// provides methods to interact with repo
//


//---------------constructor-------------------------//
Repository::Repository()
{

	//build lookup for package versions and dependencies 
	MetadataMgr mgr;
	packagesAndDependencies = new dependencyList();
	packagesAndVersions = new packageVersionsMap();
	mgr.buildDependencyList(packagesAndDependencies,packagesAndVersions);

}

//----------------destructor------------------------//
Repository::~Repository()
{
	delete packagesAndDependencies;
	delete packagesAndVersions;
}

#pragma region GetFiles

//--------------Get a list of available packages in the Repository---------//
PackageList Repository::getPackageList()
{
	PackageList availablePackages;

	for (auto packageAndDeps : *packagesAndDependencies)
	{
		availablePackages.push_back(packageAndDeps.first);
	}

	return availablePackages;
}


#pragma endregion


#pragma region CHECKIN

//-------------Check-In a new package into the repository-----------------//
string Repository::checkInPackage(Package package, PackageList dependencies)
{
	FileMgr fileMgr;
	MetadataMgr metaDataMgr;
	//check if the packagename already exists in the repository
	if (packageExists(package.name))
	{
		if (isLatestVersionofPackageClosed(package.name))
		{
			//create new version
			string newVersion = to_string(stoi(latestVersionOfPackage(package.name)) + 1);
			package.version = newVersion;
			package.status = "open";

			//create dir for it
			string newDir = packageDirPath + "/" + package.name + "_" + newVersion;
			fileMgr.createDir(packageDirPath, package.name + "_" + newVersion);

			//copy files
			fileMgr.copyPackage(socketUploadPath, package.name,newDir);

			//create metadata
			fileMgr.createMetadataFile(package, MetaDataPath, metaDataMgr.createMetaData(package, dependencies));
		}
		else
		{
			//overwrite existing latest check-In in existing dir
			package.version = latestVersionOfPackage(package.name);
			string destnDir = packageDirPath + "/" + package.name + "_" + latestVersionOfPackage(package.name);
			fileMgr.copyPackage(socketUploadPath, package.name, destnDir);
			fileMgr.createMetadataFile(package, MetaDataPath, metaDataMgr.createMetaData(package, dependencies));
		}
	}
	else
	{
		//have to create version 1
		package.version = "1";
		package.status = "open";
		string newDir = packageDirPath + "/" + package.name + "_" + package.version;
		fileMgr.createDir(packageDirPath, package.name + "_" + package.version);
		fileMgr.copyPackage(socketUploadPath, package.name, newDir);
		fileMgr.createMetadataFile(package, MetaDataPath, metaDataMgr.createMetaData(package, dependencies));
	}

	return "Package Checked-In";
}

bool Repository::packageExists(string packageName)
{
	if (packagesAndVersions->find(packageName) == packagesAndVersions->end())
		return false;

	return true;
}

string Repository::latestVersionOfPackage(string packageName)
{
	int latestVersion = 0;

	vector<string> versions = (*packagesAndVersions)[packageName];
	for (auto version : versions)
		if (stoi(version) > latestVersion)
			latestVersion = stoi(version);

	return to_string(latestVersion);
}

bool Repository::isLatestVersionofPackageClosed(string packageName)
{
	Package package;
	package.name = packageName;
	package.version = latestVersionOfPackage(packageName);

	//check if exist
	if (packagesAndDependencies->find(package) == packagesAndDependencies->end())
		return false;

	Package found = packagesAndDependencies->find(package)->first;
	return found.status == "closed";		
}

#pragma endregion


#pragma region OpenCheckIns

//-------------returns a list of packages which are currently open--------//
PackageList Repository::getOpenCheckIns()
{
	PackageList openPackagesList;

	for (auto packageAndDeps : *packagesAndDependencies)
	{
		if (packageAndDeps.first.status == "open")
			openPackagesList.push_back(packageAndDeps.first);
	}

	return openPackagesList;
}

#pragma endregion



//-------------close checkInf for a package-------------------------------//
string Repository::closeOpenCheckIn(Package package)
{

	return "";
}

#ifdef TEST_REPOSITORY
int main()
{
	cout << "\n\n-------------------Testing Repository functions ----------------------\n\n";

	cout << "\n---------get list of packages-------------\n";
	Repository repo;
	PackageList availablePackages = repo.getPackageList();
	for (auto package : availablePackages)
		cout << "Name: " + package.name << ", version: " << package.version << ", status: " << package.status << "\n";

	
	//cout << "\n---------get list of open packages-------------\n";
	//PackageList openPackages = repo.getOpenCheckIns();
	//for (auto package : openPackages)
	//	cout << "Name: " + package.name << ", version: " << package.version << ", status: " << package.status << "\n";


	//cout << "\n\n---------Test Check-In of a package-------------\n";

	////private helper function test
	//string packageName = "Package5";
	//cout << "Package Name: " << packageName << "\n";
	//cout << "is available ? : " << repo.packageExists(packageName) << "\n";
	//cout << "latest version : " << repo.latestVersionOfPackage(packageName) << "\n";
	//cout << "is latest version closed? : " << repo.isLatestVersionofPackageClosed(packageName) << "\n";

	//check-In new version of a package test
	//Package newPackage; newPackage.name = "Package3";
	//PackageList dependencies;
	//Package dep1; dep1.name = "Package4"; dep1.version = "1";
	//dependencies.push_back(dep1);
	//repo.checkInPackage(newPackage, dependencies);

	//test check-In of a new package (version 1)
	//Package newPackage; newPackage.name = "Package5";
	//PackageList dependencies;
	//Package dep1; dep1.name = "Package4"; dep1.version = "1";
	//dependencies.push_back(dep1);
	//repo.checkInPackage(newPackage, dependencies);

	//check-In package when lates version is open
/*	Package newPackage; newPackage.name = "Package4";
	PackageList dependencies;
	Package dep1; dep1.name = "Package5"; dep1.version = "1";
	dependencies.push_back(dep1);
	repo.checkInPackage(newPackage, dependencies);*/	

}

#endif // TEST_REPOSITORY


