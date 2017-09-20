/////////////////////////////////////////////////////////////////////
//  StrongComponent.h - Do Strong Component Analysis	           //
//  Language:      Visual C++                                      //
//  Application:   Prototype for CSE687 Pr1, Sp17                  //
//  Author:        Arpit Shah									   //
//  Instructed By: Jim Fawcett                                     //
/////////////////////////////////////////////////////////////////////
/*
* Module Purpose:
* ===============
* StrongComponent.h - Do Strong Component Analysis
*
* Public Interface:
* ----------------
* 	StrongComponent();												//constructor
*	~StrongComponent();
*	void  analyseStrongComponeent(NoSqlDb<DepElement> depAnalRef_);	//analyse strong component
*	void  toXMLfile(string name);									//convert analysis result into XML file
*	vector<list<string>> getstrongComponentsList();					//get result list after analysis in done
*
*  Required Files:
*  ---------------
*  - StrongComponent.h, StrongComponent.cpp, NoSQL.h,Graph.h
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
#ifndef SCAL_H
#define SCAL_H
#include"../NoSQLdb/NoSQL.h"
#include"../Graph/Graph.h"
class StrongComponent
{
	GraphLib::Graph<std::string, std::string> graph;
public:
	StrongComponent();												//constructor
	~StrongComponent();
	void  analyseStrongComponeent(NoSqlDb<DepElement> depAnalRef_);	//analyse strong component
	void  toXMLfile(string name);									//convert analysis result into XML file
	vector<list<string>> getstrongComponentsList();					//get result list after analysis in done
};
#endif

