///////////////////////////////////////////////////////////////////////////////////
// Repository.cpp - Manages the check-In, Check-Out and access of server packages  //
// ver 1.0                                                                       //
// -----------------------------------------------------------------------       //
// Language:    Visual C++, Visual Studio Enterprise 2015                        //
// Platform:    ThinkPad L440, Core i7-4712MQ                                    //
// Author:      Alok Arya                                                        //
//              (315) 728-0088, alarya@syr.edu                                   //
///////////////////////////////////////////////////////////////////////////////////
#include "MetadataMgr.h"
#include "Repository.h"
#include "FileMgr.h"
#include <string>
#include <unordered_set>

using dependency = pair<Package, vector<Package>>;
using dependencyList = map<Package, vector<Package>>;
using packageVersionsMap = map<string, vector<string>>;
using PackageList = vector<Package>;
using namespace std;

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

	//update dep lookup table and package version lookup table
	(*packagesAndDependencies)[package] = dependencies;
	updatePackageAndVersionsMap(package);

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


#pragma region Check-OutPackage

bool packageExists_(PackageList& packagesAdded, Package package)
{
	for (auto package_ : packagesAdded)
	{
		if (package_.name == package.name && package_.version == package.version)
			return true;

	}
	return false;
}

void addDependencies(Package checkOutPackage,Package depPackage, dependencyList* packagesAndDependencies,PackageList& packagesAdded)
{
	for (auto package_ : (*packagesAndDependencies)[depPackage])
	{
		if (!packageExists_(packagesAdded, package_) && !(checkOutPackage.name == package_.name && checkOutPackage.version == package_.version))
		{
			packagesAdded.push_back(package_);
			addDependencies(checkOutPackage, package_, packagesAndDependencies, packagesAdded);
		}
	}
}


//----------returns a list of packages needed to be check-Out a package----------//
PackageList Repository::checkOutPackageDependencies(Package package, bool includeDependencies)
{
	PackageList packagesAdded;

	if (includeDependencies)
	{
		for (auto package_ : (*packagesAndDependencies)[package])
		{
			if (! packageExists_(packagesAdded, package_) && !(package.name == package_.name && package.version == package_.version))
			{			
				packagesAdded.push_back(package_);
				addDependencies(package, package_, packagesAndDependencies, packagesAdded);
			}
		}
	}

	return packagesAdded;
}

#pragma endregion


#pragma region CloseOpenCheckIn

//-------------close checkIn for a package-------------------------------//
string Repository::closeOpenCheckIn(Package package)
{
	dependency packageDep;
	package.status = "closed";
	packageDep.first = package; packageDep.second = (*packagesAndDependencies)[package];

	FileMgr fileMgr;
	MetadataMgr metadataMgr;
	string destnPath = MetaDataPath;
	string metadata = metadataMgr.createMetaData(package, packageDep.second);
	fileMgr.createMetadataFile(package, destnPath, metadata);

	//update lookup tables
	PackageList depList = (*packagesAndDependencies)[package];
	packagesAndDependencies->erase(package);
	(*packagesAndDependencies)[package] = depList;
	updatePackageAndVersionsMap(package);

	return "Package check-In has been closed";
}

#pragma endregion


//--------------update package versions map------------------------------//
void Repository::updatePackageAndVersionsMap(Package package)
{
	if (packagesAndVersions->find(package.name) == packagesAndVersions->end())
	{
		vector<string> versions;  versions.push_back("1");
		(*packagesAndVersions)[package.name] = versions;
	}
	else
	{
		if (latestVersionOfPackage(package.name) != package.version)
		{
			vector<string> versions = (*packagesAndVersions)[package.name];
			versions.push_back(package.version);
			(*packagesAndVersions)[package.name] = versions;
		}
	}
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

	////private helper function tests
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

	//test check-In package when latest version is open
/*	Package newPackage; newPackage.name = "Package4";
	PackageList dependencies;
	Package dep1; dep1.name = "Package5"; dep1.version = "1";
	dependencies.push_back(dep1);
	repo.checkInPackage(newPackage, dependencies);*/	

	//--------test checking-Out package function------------//
	Package checkOutPackage; checkOutPackage.name = "Package1"; checkOutPackage.version = "1";
	PackageList checkoutFiles = repo.checkOutPackageDependencies(checkOutPackage, true);

	//-------test closing an existing open check-In---------//
	//Package closeCheckIn; closeCheckIn.name = "Package4"; closeCheckIn.version = "2";
	//repo.closeOpenCheckIn(closeCheckIn);
	//availablePackages = repo.getPackageList();
	//for (auto package : availablePackages)
	//	cout << "Name: " + package.name << ", version: " << package.version << ", status: " << package.status << "\n";
}

#endif // TEST_REPOSITORY


