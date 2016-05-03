/////////////////////////////////////////////////////////////////////////////
// ClientForm.cpp - Starts up the client GUI                               //
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
* Acts as an entry point for starting up the GUI application
*
*
* Required Files:
* ===============
* HttpMessage.h, HttpMessage.cpp
*
* Build Command:
* ==============
* Build Command: devenv Project4.sln /rebuild debug /project ClientGUI/ClientGUI.vcxproj
*
* Maintenance History:
* ====================
*
* ver 1.0 : 2 May 2016
* - first release
*/
#include "ClientForm.h"

using namespace ClientGUI;


[STAThreadAttribute]
int main()
{
	ClientForm clf;
	clf.ShowDialog();

	return 0;
}