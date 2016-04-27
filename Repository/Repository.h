#ifndef REPOSITORY_H
#define REPOSITORY_H


#include "MetadataMgr.h"
#include <vector>
#include <map>

class Repository {

public: 

private:
	vector<pair<Package, vector<Package>>> packagesAndDependencies;    //maintains list of packages with it's dependencies
};

#endif
