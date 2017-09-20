///////////////////////////////////////////////////////////////
//  Project3TestExecutives.cpp - Run the publishier for server//
// ver 1.0                                                   //
// Arpit Shah,  CSE687 - Object Oriented Design, Spring 2017 //
///////////////////////////////////////////////////////////////
#include "Project3TestExecutives.h"

using namespace Utilities;
using namespace CodeAnalysis;

Project3TestExecutives::Project3TestExecutives()
{
	
}

Project3TestExecutives::~Project3TestExecutives()
{
}

//process started
void Project3TestExecutives::startProcessing(int argc, char* argv[], unordered_map<string, bool>& fileState)
{
	exec.showCommandLineArguments(argc, argv);
	StringHelper::title("\n  Processing Files got from command Lines \n");
	exec.codeAnalysisInitialisation(argc, argv, fileState);
	std::cout << std::endl;
	StringHelper::title("Type Analysis");
    exec.typeAnalysisProcess();
	std::cout << std::endl;
}

// do type analysis for given file
void Project3TestExecutives::doTypeAnalysis(string file, unordered_map<string, bool>& fileState)
{
	fileState[file] = false;
	exec.codeAnalysisInitialisation(file);
	std::cout << std::endl;
	StringHelper::title("Type Analysis");
	exec.typeAnalysisProcess();
	std::cout << std::endl;
}

//do publishing for given file
void Project3TestExecutives::doPublishing(unordered_map<string,bool>& fileState, string fileName, std::unordered_set<string>& filePublished, std::queue<string>& fileToPublish,string publishPath)
{
		StringHelper::title("Dependency Analysis");
		std::cout << std::endl;
		NoSqlDb<DepElement>& depAnalRef_(CodeAnalysis::Repository::getInstance()->getdepAnal());
		exec.processSourceCodeDepAnal(fileName);
		
		if (depAnalRef_.contains(fileName))
		{
			vector<DepElement> elms = depAnalRef_.value(fileName);
			for (DepElement el : elms)
			{
				string child = el.getchild();
				if (filePublished.find(child) == filePublished.end())
					fileToPublish.push(child);
			}
		}
		StringHelper::title("Publishing Code Files");
		//if (!fileState[fileName])
		exec.procesPublishing(fileName, publishPath);
		filePublished.insert(fileName);
		fileState[fileName] = true;
}


#ifdef PROJECT_TEST
int main(int argc, char* argv[])
{
	try {
		if (argc == 5)
		{
			Project3TestExecutives ptr;
			unordered_map<string, bool> fileState;
			ptr.startProcessing(argc, argv, fileState);
			ptr.doTypeAnalysis("C:\\Test.cpp", fileState);
		}
		else
		{
			std::cout << "\nPlease provide valid command arguments\n";
		}
	}
	catch (...)
	{
		std::cout << "\n\n  Caught exception in Executive::main: \n\n";
		return 1;
	}
	return 0;
}
#endif