/////////////////////////////////////////////////////////////////////
//  Project3TestExecutives.h - Demonstartions					   //
//  Language:      Visual C++                                      //
//  Application:   Prototype for CSE687 Pr1, Sp17                  //
//  Author:        Arpit Shah									   //
//  Instructed By: Jim Fawcett                                     //
/////////////////////////////////////////////////////////////////////
/*
* Module Purpose:
* ===============
* Demonstartions package
*
* Public Interface:
* ----------------
* void startProcessing(int argc, char * argv[], unordered_map<string, bool>& fileState); //start the processing of the files
* void doTypeAnalysis(string file, unordered_map<string, bool>& fileState); //do type analysis of given file
* void doPublishing(unordered_map<string, bool>& fileState, string fileName, std::unordered_set<string>& filePublished, std::queue<string>& fileToPublish, string publishPath);   //publish the given file
* Required Files:
* ---------------
* Project3TestExecutives.h, Project3TestExecutives.cpp, Utilities.h, Display.h, Executive.h, StrongComponent.h
*
* Build Process:
* --------------
* devenv CSharpCliShim-WPF-App.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.0 : 06 March 2017
* - first release
*/

#ifndef P2TESTEXECUTIVE_H
#define P2TESTEXECUTIVE_H
#include<iostream>
#include <string>
#include <sstream>
#include <windows.h>
#include<unordered_set>
#include"../Utilities/Utilities.h"
#include"../Display/Display.h"
#include "../Analyzer/Executive.h"
#include"../StrongComponent/StrongComponent.h"
namespace CodeAnalysis
{
	class Project3TestExecutives
	{
	private:
		CodeAnalysis::CodeAnalysisExecutive exec;
	public:
		Project3TestExecutives();
		~Project3TestExecutives();
		void startProcessing(int argc, char * argv[], unordered_map<string, bool>& fileState); //start the processing of the files
		void doTypeAnalysis(string file, unordered_map<string, bool>& fileState); //do type analysis of given file
		void doPublishing(unordered_map<string, bool>& fileState, string fileName, std::unordered_set<string>& filePublished, std::queue<string>& fileToPublish, string publishPath);   //publish the given file
	};
}
#endif
