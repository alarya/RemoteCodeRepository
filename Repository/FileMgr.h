#ifndef FILEMGR_H
#define FILEMGR_H



#include"MetadataMgr.h"
#include <string>


using namespace std;
class FileMgr {

public:
	FileMgr();
	~FileMgr();

	bool createDir(string path, string dirName);
	bool copyPackage(string sourcePath, string packageName, string destnPath); //overwrites if already exists
	bool createMetadataFile(Package package, string destnPath, string metadata);
	//bool deleteMetadataFile(Package package, string destnPath);
	string getFileSpec(string relativePath);
private:

};

#endif 