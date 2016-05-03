#ifndef FILEMGR_H
#define FILEMGR_H
/////////////////////////////////////////////////////////////////////////////
// FileMgr.h - File functions used by Repository to manage FS of packages  //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio Enterprise 2015                  //
// Platform:    ThinkPad L440, Core i7-4712MQ                              //
// Author:      Alok Arya                                                  //
//              (315) 728-0088, alarya@syr.edu                             //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* File Manager is a wrapper around FileSystem.h
* It provides functions for the repository to manage packages in the server File system.
*
* Public Interface:
* =================
* FileMgr fileMgr;           //create a new instance of FileMgr
*
*
* string path = "../root";
* string dirName = "Package2_2";
* bool created = fileMgr.createDir(path,dirName);    //creates a new directory with specified name and in the specified path
*
* string PackageName = "Package5";
* string sourcePath = "../root/socket_uploads";
* string destnPath = "../root/Package5_1";
* bool copied = fileMgr.copyPackage(sourcePath, PackageName, destnPath);    //copies the Package5.cpp and Package5.h files from source directory 
*																			  to the destination directory
*																			  Used after the socket has received the uploaded package for check-in
*
* string metadata = "<metadata>\n 
						<pakckage>\n 
							<name>\n Package1 \n </name>\n 
							<version>\n 1 \n </version>\n
							<status>\n open \n </version> \n
						</package>\n
						<dependencies> \n
							.
							.
							.
						</dependencies> \n
					 </metadata> 
*
* string destnPath = "../root/Metadata";
* Package package;
* package.name = "Package1"; package.version = "1"; package.status = "open";
* bool createdMetadata = fileMgr.createMetadataFile(package, destnPath, metadata);    //creates a metada file for the specified package in the destn path provided
*																					  // Also accepts the xml metadata in a string
*
*
* string fullPath = fileMgr.getFileSpec("../root/Package1_1/Package1.h");             //get full path from a relative path
*
* Required Files:
* ===============
* FileMgr.cpp
* MetadataMgr.h, MetadataMgr.cpp
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


#include"MetadataMgr.h"
#include <string>


using namespace std;
class FileMgr {

public:
	FileMgr();
	~FileMgr();

	bool createDir(string path, string dirName);
	bool copyPackage(string sourcePath, string packageName, string destnPath); //overwrites if already exists
	bool createMetadataFile(Package package, string destnPath, string metadata); //overwrites metadata if already exists
	//bool deleteMetadataFile(Package package, string destnPath);
	string getFileSpec(string relativePath);
private:

};

#endif 