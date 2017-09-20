/////////////////////////////////////////////////////////////////////
//  Display.h - Display facility to show each result				//
//  Language:      Visual C++                                      //
//  Application:   Prototype for CSE687 Pr1, Sp17                  //
//  Author:        Arpit Shah									   //
//  Instructed By: Jim Fawcett                                     //
/////////////////////////////////////////////////////////////////////
/*
* Module Purpose:
* ===============
*  Display.h - Display facility to show analysis each result
*
* Public Interface:
* ----------------
* 	void static displayDependencyAnalysis();							//display the dependency analysis information
*	void static displayStrongComponentnalysis(StrongComponent anlyse);  //display the strong component analysis information
*	void static displayTypeAnalysis();									//display the type analysis information							//display results
*
*  Required Files:
*  ---------------
*  - Display.h, Display.cpp, NoSQL.h,ActionsAndRules.h, StrongComponent.h
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
#ifndef DISPLAY_H
#define DISPLAY_H
#include"../Parser/ActionsAndRules.h"
#include<iostream>
#include"../StrongComponent/StrongComponent.h"
class Display
{
public:
	Display();
	~Display();
	void static displayDependencyAnalysis();							//display the dependency analysis information
	void static displayStrongComponentnalysis(StrongComponent anlyse);  //display the strong component analysis information
	void static displayTypeAnalysis();									//display the type analysis information
};
#endif

