#ifndef EXEC_H
#define EXEC_H
/////////////////////////////////////////////////////////////////////
// Executive.h - Organizes and Directs Code Analysis               //
// ver 1.3                                                         //
//-----------------------------------------------------------------//
// Language:    C++, Visual Studio 2015                            //
// Platform:    Dell XPS 8900, Windows 10                          //
// Application: Project #2, CSE687 - Object Oriented Design, S2016 //
//  Author:        Arpit Shah									   //
//  Instructed By: Jim Fawcett                                     //
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  ===================
*  Executive uses Parser, ScopeStack, and AbstrSynTree packages to
*  provide static code analysis for C++ and C# files contained in a
*  specified directory tree.
*
*  It finds files for analysis using FileMgr and, for each file found,
*  uses parser rules to detect:
*  - namespace scopes
*  - class scopes
*  - struct scopes
*  - control scopes
*  - function definition scopes
*  - data declarations
*  and uses rule actions to build an Abstract Sytax Tree (AST)
*  representing the entire code set's static structure.  Each detected
*  scope becomes a node in the AST which spans all of the files analyzed,
*  rooted in a top-level Global Namespace scope.
*
*  Executive supports displays for:
*  - a list of all files processed with their source lines of code count.
*  - an indented representation of the AST.
*  - a list of all functions that are defined, organized by package, along
*    with their starting lines, size in lines of code, and complexity as
*    measured by the number of their descendent nodes.
*  - a list of all functions which exceed specified function size and/or
*    complexity.
*
*  Because much of the important static structure information is contained
*  in the AST, it is relatively easy to extend the application to evaluate
*  additional information, such as class relationships, dependency network,
*  and static design flaws.
*
* Public Interface:
* ----------------
*  AnalFileMgr(const Path& path, FileMap& fileMap); //analyse File manager
*  virtual void file(const File& f);				//virtual function for file
*  virtual void dir(const Dir& d);					//virtual function for getting directory
*  virtual void done();							//ending process
*  size_t numFiles();								//return number of files
*  size_t numDirs();								//return number of directory
*  CodeAnalysisExecutive();												//constructor
*  virtual ~CodeAnalysisExecutive();
*  CodeAnalysisExecutive(const CodeAnalysisExecutive&) = delete;
*  CodeAnalysisExecutive& operator=(const CodeAnalysisExecutive&) = delete;
*  void typeAnalysisProcess();												//type analysis process instatntiation
*  FileMap& getFileMap();													//get the file map
*  void showCommandLineArguments(int argc, char * argv[]);					//show Command Line Arguments
*  bool ProcessCommandLine(int argc, char * argv[]);						//process Command Line Arguments
*  std::string getAnalysisPath();											//get Analysis Path
*  virtual void getSourceFiles();											//get Source Files
*  virtual void processSourceCode(bool showActivity);						//process Source Code
*  virtual void processSourceCodeDepAnal(bool showProc);					//process Source CodeDepAnal
*  void processSourceCodeAnalysing(string file, bool showProc);			//process Source Code Analysing
*  void complexityAnalysis();												//complexity Analysis Instatntiation
*  std::vector<File>& cppHeaderFiles();									//return list of cppHeaderFiles
*  std::vector<File>& cppImplemFiles();									//return list of cppImplemFiles
*  std::vector<File>& csharpFiles();										//return list of csharpFiles
*  Slocs fileSLOCs(const File& file);										//return size of fileSLOCs
*  size_t numFiles();														//return number of files
*  size_t numDirs();														//return number of dir
*  std::string systemTime();												//return string time
*  void codeAnalysisInitialisation(int argc, char * argv[]);				//Instantiate code Analysis
*  virtual void displayMetrics(ASTNode* root);								//display metrics from the given node
*  virtual void displayMetrics();											//display metric
*  virtual void displayMetricSummary(size_t sMax, size_t cMax);			//display Metric Summary
*  virtual void displaySlocs();											//display Slocs
*  virtual void displayAST();												//display AST
*  virtual void dispatchOptionalDisplays();								//dispatch Optional Displays
*  virtual void setDisplayModes();											//set Display Modes
*  void startLogger(std::ostream& out);									//start Logger
*  void flushLogger();														//flush Logger
*  void stopLogger();														//stop Logger
*  void setLogFile(const File& file);										//set LogFile
*  void AnalyseFileToPublish(CodeAnalysisExecutive & exectemp, bool showProc, NoSqlDb<DepElement>& depAnalRef_, string & file, Publisher & p); //Analyse given file to publish
*  void procesPublishing(bool showProc);									//processing code file publishing process

*  Required Files:
*  ---------------
*  - Executive.h, Executive.cpp
*  - Parser.h, Parser.cpp, ActionsAndRules.h, ActionsAndRules.cpp,Publisher.h
*  - ConfigureParser.h, ConfigureParser.cpp
*  - ScopeStack.h, ScopeStack.cpp, AbstrSynTree.h, AbstrSynTree.cpp
*  - ITokenCollection.h, SemiExp.h, SemiExp.cpp, Tokenizer.h, Tokenizer.cpp
*  - IFileMgr.h, FileMgr.h, FileMgr.cpp, FileSystem.h, FileSystem.cpp
*  - Logger.h, Logger.cpp, Utilities.h, Utilities.cpp
*
*  Maintanence History:
*  --------------------
*  ver 1.3 : 28 Oct 2016
*  - added test for interface in AbstrSynTree functions show() and Add(ASTNode*)
*  - wrapped path in quotes for display
*  - added check for Byte Order Mark (BOM) in ConfigureParser.cpp
*  - wrapped path in quotes in Window.cpp in case path has spaces, necessary
*    since path is passed to Executive on command line.
*  ver 1.2 : 27 Oct 2016
*  - fixed bug in displayMetrics(ASTNode*) by adding test for interface
*  ver 1.1 : 28 Aug 2016
*  - many changes: added new functions and modified existing functions
*  ver 1.0 : 09 Aug 2016
*  - first release
*
*/
#include <string>
#include <vector>
#include <unordered_map>
#include <iosfwd>
#include "../Parser/Parser.h"
#include "../FileMgr/FileMgr.h"
#include "../Parser/ConfigureParser.h"
#include "../Utilities/Utilities.h"
#include"../HtmlPublisher/Publisher.h"

namespace CodeAnalysis
{
	using Utils = Utilities::StringHelper;

	///////////////////////////////////////////////////////////////////
	// AnalFileMgr class derives from FileManager::FileMgr
	// - provides application specific handling of file and dir events

	class AnalFileMgr : public FileManager::FileMgr
	{
	public:
		using Path = std::string;
		using File = std::string;
		using Files = std::vector<File>;
		using Pattern = std::string;
		using Ext = std::string;
		using FileMap = std::unordered_map<Pattern, Files>;

		AnalFileMgr(const Path& path, FileMap& fileMap); //analyse File manager
		virtual void file(const File& f);				//virtual function for file
		virtual void dir(const Dir& d);					//virtual function for getting directory	
		virtual void done();							//ending process
		size_t numFiles();								//return number of files
		size_t numDirs();								//return number of directory
	private:
		Path d_;
		FileMap& fileMap_;
		bool display_;
		size_t numFiles_;
		size_t numDirs_;
	};

	///////////////////////////////////////////////////////////////////
	// CodeAnalysisExecutive class directs C++ and C# code analysis

	class CodeAnalysisExecutive
	{
	public:
		using Path = std::string;
		using Pattern = std::string;
		using Patterns = std::vector<Pattern>;
		using File = std::string;
		using Files = std::vector<File>;
		using Ext = std::string;
		using Options = std::vector<char>;
		using FileMap = std::unordered_map<Pattern, Files>;
		using FileNodes = std::vector<std::pair<File, ASTNode*>>;
		using Slocs = size_t;
		using SlocMap = std::unordered_map<File, Slocs>;

		CodeAnalysisExecutive();												//constructor
		virtual ~CodeAnalysisExecutive();

		CodeAnalysisExecutive(const CodeAnalysisExecutive&) = delete;
		CodeAnalysisExecutive& operator=(const CodeAnalysisExecutive&) = delete;
		void typeAnalysisProcess();						                     	//type analysis process instatntiation
		FileMap& getFileMap();													//get the file map
		void showCommandLineArguments(int argc, char * argv[]);					//show Command Line Arguments
		bool ProcessCommandLine(int argc, char * argv[]);						//process Command Line Arguments
		std::string getAnalysisPath();											//get Analysis Path
		virtual void getSourceFiles();											//get Source Files
		void processSourceCodeAnalysing(string file, bool showProc);			//process Source Code Analysing
		virtual void initprocessSourceCode(bool showProc, unordered_map<string, bool>& fileState); //initialize processing of souce code for all the files
		void processSourceCode(string file);                                     //process source code
		void complexityAnalysis();												//complexity Analysis Instatntiation
		std::vector<File>& cppHeaderFiles();									//return list of cppHeaderFiles
		void AnalyseFileToPublish(CodeAnalysisExecutive & exectemp, bool showProc, NoSqlDb<DepElement>& depAnalRef_, string & file, Publisher & p, string & publishPath);
		void procesPublishing(string file, string publishPath);
		std::vector<File>& cppImplemFiles();									//return list of cppImplemFiles
		std::vector<File>& csharpFiles();										//return list of csharpFiles
		Slocs fileSLOCs(const File& file);										//return size of fileSLOCs
		size_t numFiles();														//return number of files
		size_t numDirs();														//return number of dir
		std::string systemTime();												//return string time
		void codeAnalysisInitialisation(int argc, char * argv[], unordered_map<string, bool>& fileState);
		void codeAnalysisInitialisation(string file);
		virtual void displayMetrics(ASTNode* root);								//display metrics from the given node 
		virtual void displayMetrics();											//display metric
		virtual void displayMetricSummary(size_t sMax, size_t cMax);			//display Metric Summary	
		virtual void displaySlocs();											//display Slocs
		virtual void displayAST();												//display AST
		virtual void dispatchOptionalDisplays();								//dispatch Optional Displays
		virtual void setDisplayModes();											//set Display Modes
		void startLogger(std::ostream& out);									//start Logger
		void flushLogger();														//flush Logger
		void stopLogger();					
		void processSourceCodeDepAnal(string file);//stop Logger 
		void setLogFile(const File& file);										//set LogFile
		void initProcessSourceCodeDepAnal(bool showProc);
	private:
		void setLanguage(const File& file);
		void showActivity(const File& file);
		void clearActivity();
		virtual void displayHeader();
		virtual void displayMetricsLine(const File& file, ASTNode* pNode);
		virtual void displayDataLines(ASTNode* pNode, bool isSummary = false);
		std::string showData(const Scanner::ITokCollection* ptc);
		Parser* pParser_;
		ConfigParseForCodeAnal configure_;
		Repository* pRepo_;
		Path path_;
		Patterns patterns_;
		Options options_;
		FileMap fileMap_;
		FileNodes fileNodes_;
		std::vector<File> cppHeaderFiles_;
		std::vector<File> cppImplemFiles_;
		std::vector<File> csharpFiles_;
		size_t numFiles_;
		size_t numDirs_;
		SlocMap slocMap_;
		bool displayProc_ = false;
		std::ofstream* pLogStrm_ = nullptr;
	};

}
#endif