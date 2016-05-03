/////////////////////////////////////////////////////////////////////////////
// FileMgr.cpp - File functions used by Repository to manage FS of packages //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio Enterprise 2015                  //
// Platform:    ThinkPad L440, Core i7-4712MQ                              //
// Author:      Alok Arya                                                  //
//              (315) 728-0088, alarya@syr.edu                             //
/////////////////////////////////////////////////////////////////////////////
#include "FileMgr.h"
#include "../FileSystem/FileSystem.h"
#include "MetadataMgr.h"
#include <iostream>


using namespace std;
using namespace FileSystem;


///////////////////////////////////////////////////////////////////
// class FileMgr
// 
//

FileMgr::FileMgr()
{

}


FileMgr::~FileMgr()
{

}


//----------Create directory in specified path with specified name-------------//
bool FileMgr::createDir(string path, string dirName)
{

	string dirPath = path + "/" + dirName;
	bool exists = Directory::exists(dirPath);
	if (exists)
	{
		cout << "\n Directory already exists: " << dirPath << "\n";
		return false;
	}
	
	return Directory::create(dirPath);
	
}

//----------move package from sourcePath to destination Path---------------------//
bool FileMgr::copyPackage(string sourcePath,string packageName, string destnPath)
{

	if (!Directory::exists(destnPath))
	{
		cout << "\nCould not find source directory while copying package...\n";
		return false;
	}
		

	string sourceCppFilePath = Path::getFullFileSpec(sourcePath + "/" + packageName + ".cpp");
	string sourceHFilePath = Path::getFullFileSpec(sourcePath + "/" + packageName + ".h");
	string destnCppFilePath = Path::getFullFileSpec(destnPath + "/" + packageName + ".cpp");
	string destnHFilePath = Path::getFullFileSpec(destnPath + "/" + packageName + ".h");

	if (!File::copy(sourceCppFilePath, destnCppFilePath))
	{
		cout << "\nCould not copy Cpp file...\n";
		return false;
	}

	if (!File::copy(sourceHFilePath, destnHFilePath))
	{
		cout << "\nCould not copy h file..\n";
		return false;
	}

	//remove source files
	File::remove(sourceCppFilePath);
	File::remove(sourceHFilePath);

	return true;
}

//----------create metadata for a package (overwrites if already exists)-----------//
bool FileMgr::createMetadataFile(Package package, string destnPath, string metadata)
{
	string fileName = package.name + "_" + package.version + ".xml";
	string fileSpec = Path::getFullFileSpec(destnPath + "/" + fileName);

	File file(fileSpec);
	file.open(File::out, File::text);

	file.putLine(metadata);

	return false;
}

//--------returns full path of a file from relative path------------------------//
string FileMgr::getFileSpec(string relativePath)
{
	return Path::getFullFileSpec(relativePath);
}

#ifdef TEST_FILEMGR

int main()
{
	FileMgr fileMgr;

	//create dir test
	/*fileMgr.createDir("../root", "Package5_1");*/

	//copy package test
	/*fileMgr.copyPackage("../root/socket_uploads", "Package5", "../root/Package5_1");*/

	//create metadataTest
	/*Package package; package.name = "Package5"; package.version = "1";
	fileMgr.createMetadataFile(package, "../root/Metadata", "<root> \n </root>");*/
}

#endif // TEST_FILEMGR
