/////////////////////////////////////////////////////////////////////
//  TypeElement.h - provide type element to store types		       //
//  Language:      Visual C++                                      //
//  Application:   Prototype for CSE687 Pr1, Sp17                  //
//  Author:        Arpit Shah									   //
//  Instructed By: Jim Fawcett                                     //
/////////////////////////////////////////////////////////////////////
/*
* Module Purpose:
* ===============
* TypeTableElement.h - provide type element to store types
*
* Public Interface:
* ----------------
* 	void setNamepspace(string namepspace);   //set namespace
*	string getNamepspace();					//get namespace
*	void setfilePath(string desc);			//set filepath
*	string getfilePath();					//get filepath
*	void settypeName(string data);			//set typename
*	string gettypeName();					//get filename
*	std::string displayElementData();		//disply element data
*	std::string toStringXML();				//return string by converting it into XML file
*	bool Equals(TypeElement d);				//check and return whether it equals or not
*
*  Required Files:
*  ---------------
*  - TypeElement.h, TypeElement.cpp
*
* Build Process:
* --------------
* devenv CodeAnalyzer.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.0 : 06 March 2017
* - first release
*/
#ifndef TYPEELEM_H
#define TYPEELEM_H
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <iostream>
using namespace std;
using Key = std::string;
using Keys = std::vector<Key>;

class TypeElement
{
private:
	string namepspace, filePath;
	string typeName;
public:
	TypeElement();
	~TypeElement();
	TypeElement(string namepspace, string filepath, string typeName) :namepspace(namepspace), filePath(filepath), typeName(typeName)
	{}
	void setNamepspace(string namepspace);   //set namespace
	string getNamepspace();					//get namespace
	void setfilePath(string desc);			//set filepath
	string getfilePath();					//get filepath
	void settypeName(string data);			//set typename
	string gettypeName();					//get filename
	std::string displayElementData();		//disply element data
	std::string toStringXML();				//return string by converting it into XML file
	bool Equals(TypeElement d);				//check and return whether it equals or not
};
#endif

