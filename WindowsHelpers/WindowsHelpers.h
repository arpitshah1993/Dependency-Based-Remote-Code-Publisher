#ifndef WINDOWSHELPERS_H
#define WINDOWSHELPERS_H
/////////////////////////////////////////////////////////////////////
// WindowsHelper.h - small helper functions for using Windows API  //
// ver 1.0                                                         //
//-----------------------------------------------------------------//
// Language:    C++, Visual Studio 2015                            //
// Application: Several Projects, CSE687 - Object Oriented Design  //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package supports programming to the Windows API.  It currently
* contains three global functions that convert strings to and from
* UTF-16, and retrieving error messages.
*
* Build Process:
* --------------
* Required Files: WindowsHelper.h, WindowwsHelper.cpp
*/

#include <string>
#include <vector>

namespace WindowsHelpers
{
  std::string wstringToString(const std::wstring& wstr);
  std::wstring stringToWideString(const std::string& str);
  std::string GetLastMsg(bool wantSocketMessage);
}
 
#endif
