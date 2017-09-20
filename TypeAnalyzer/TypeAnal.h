/////////////////////////////////////////////////////////////////////
//  TypeAnal.h - Do type Analysis					               //
//  Language:      Visual C++                                      //
//  Application:   Prototype for CSE687 Pr1, Sp17                  //
//  Author:        Arpit Shah									   //
//  Instructed By: Jim Fawcett                                     //
/////////////////////////////////////////////////////////////////////
/*
* Module Purpose:
* ===============
* TypeAnal.h module is used to perform type analysis of provided
* files using type table and toker.
*
* Public Interface:
* ----------------
*  TypeAnal();						//contsructor
*  void BFS(ASTNode * pNode);		//BFS iteration on AST
*  void doTypeAnal();				//analysing types of specified file
*  void show();					//showing type analysis
*  Required Files:
*  ---------------
*  - TypeAnal.h, TypeAnal.cpp, NoSQL.h,ActionsAndRules.h
*
* Build Process:
* --------------
* devenv ArpitShahHW2.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.0 : 06 March 2017
* - first release
*/
#include "../Parser/ActionsAndRules.h"
#include"../NoSQLdb/NoSQL.h"
#include <iostream>
#include<queue>

namespace CodeAnalysis
{
	class TypeAnal
	{
	public:
		using SPtr = std::shared_ptr<ASTNode*>;
		TypeAnal();						//contsructor
		void BFS(ASTNode * pNode);		//BFS iteration on AST
		void doTypeAnal();				//analysing types of specified file
		void show();					//showing type analysis
	private:
		NoSqlDb<TypeElement>& typeTableRef_;
		NoSqlDb<DepElement>& namespaceTableRef_;
		AbstrSynTree& ASTref_;
		ScopeStack<ASTNode*> scopeStack_;
		Scanner::Toker& toker_;
	};
}