#pragma once


#include <iostream>

struct IService {
	virtual void PostMessage(std::string msg)=0;
	virtual ~IService() {};
};