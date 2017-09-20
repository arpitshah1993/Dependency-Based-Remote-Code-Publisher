/////////////////////////////////////////////////////////////////////
//  DepElement.h - provide dependency element to store it in NOSQL //
//  Language:      Visual C++                                      //
//  Application:   Prototype for CSE687 Pr1, Sp17                  //
/////////////////////////////////////////////////////////////////////
/*
* Module Purpose:
* ===============
* DepElement.h - provide dependency element to store it in NOSQ
*
* Public Interface:
* ----------------
* DepElement(std::string s):children(s) {}	//sequenece constructor to create string children also
* DepElement();								//dependency constuctor
* ~DepElement();
* Key getchild();								//get child
* bool Equals(DepElement d);					//check whether it equals or not
* std::string displayElementData();			//display data of the element
* std::string toStringXML();					//conver and return data into XML
*
*  Required Files:
*  ---------------
*  - DepElement.h, DepElement.cpp
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
#ifndef DEPELEMENT_H
#define DEPELEMENT_H
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <iostream>

using Key = std::string;
class DepElement {
private:
	Key children;
public:
	DepElement();								//dependency constuctor
	DepElement(std::string s) :children(s) {}	//sequenece constructor to create string children also
	~DepElement();
	Key getchild();								//get child
	bool Equals(DepElement d);					//check whether it equals or not
	std::string displayElementData();			//display data of the element
	std::string toStringXML();					//conver and return data into XML
};
#endif


