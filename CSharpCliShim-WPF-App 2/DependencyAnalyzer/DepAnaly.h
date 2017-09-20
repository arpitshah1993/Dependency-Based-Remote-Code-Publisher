/////////////////////////////////////////////////////////////////////
//  DepAnal.h - Do dependency Analysis					          //
//  Language:      Visual C++                                      //
//  Application:   Prototype for CSE687 Pr1, Sp17                  //
/////////////////////////////////////////////////////////////////////
/*
* Module Purpose:
* ===============
* DepAnal.h module is used to perform dependency analysis of provided
* files using type table and toker.
*
* Public Interface:
* ----------------
* DepAnal();									// constuctor
* void doDepAnal(string file);				// do depenedncy analysis using type table reference.
* void show();								//display results
* void addFileDependency(string & file, std::vector<TypeElement>& values, bool & isClass, bool & isNameSpace, string & keyFile, string & dependentfile, bool & isAdded); //add dependency for same namespaces
* void updateDependnecy(string file, TypeElement value, bool & isClass, bool & isNameSpace, string & keyFile, string & dependentfile);					 		    	//update dependency in the list
*
*  Required Files:
*  ---------------
*  - DepAnal.h, DepAnal.cpp, NoSQL.h,ActionsAndRules.h
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
#ifndef DEPANAL_H
#define DEPANAL_H
#include"../NoSQLdb/NoSQL.h"
#include"../Parser/ActionsAndRules.h"

namespace CodeAnalysis
{
	class DepAnal
	{
	public:
		DepAnal();									// constuctor
		void addFileDependency(string & file, std::vector<TypeElement>& values, bool & isClass, bool & isNameSpace, string & keyFile, string & dependentfile, bool & isAdded, string & nextTok);
		//void addFileDependency(string & file, std::vector<TypeElement>& values, bool & isClass, bool & isNameSpace, string & keyFile, string & dependentfile, bool & isAdded); //add dependency for same namespaces
		void updateDependnecy(string file, TypeElement value, bool & isClass, bool & isNameSpace, string & keyFile, string & dependentfile);					 		    	//update dependency in the list
		void doDepAnal(string file);				// do depenedncy analysis using type table reference.				
		void show();								//display results
	private:
		NoSqlDb<TypeElement>& typeTableRef_;
		Scanner::Toker& toker_;
		NoSqlDb<DepElement>& depAnalRef_;
		NoSqlDb<DepElement>& namespaceTableRef_;
	};
}
#endif