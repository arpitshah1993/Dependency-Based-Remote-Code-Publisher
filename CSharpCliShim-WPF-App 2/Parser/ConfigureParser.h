#ifndef CONFIGUREPARSER_H
#define CONFIGUREPARSER_H
/////////////////////////////////////////////////////////////////////
//  ConfigureParser.h - builds and configures parsers              //
//  ver 3.2                                                        //
//                                                                 //
//  Lanaguage:     Visual C++ 2005                                 //
//  Platform:      Dell Dimension 9150, Windows XP SP2             //
//  Application:   Prototype for CSE687 Pr1, Sp06                  //
//  Author:        Arpit Shah									   //
//  Instructed By: Jim Fawcett                                     //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations:
  ==================
  This module builds and configures parsers.  It builds the parser
  parts and configures them with application specific rules and actions.

  Public Interface:
  =================
  ConfigParseForCodeAnal config;
  config.Build();
  config.Attach(someFileName);

  Build Process:
  ==============
  Required files
	- ConfigureParser.h, ConfigureParser.cpp, Parser.h, Parser.cpp,
	  ActionsAndRules.h, ActionsAndRules.cpp,
	  SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
	- devenv Project1HelpS06.sln
	- cl /EHsc /DTEST_PARSER ConfigureParser.cpp parser.cpp \
		 ActionsAndRules.cpp \
		 semiexpression.cpp tokenizer.cpp /link setargv.obj
*/

#include <fstream>
#include "Parser.h"
#include "ActionsAndRules.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

namespace CodeAnalysis
{
	///////////////////////////////////////////////////////////////
	// build parser that writes its output to console

	class ConfigParseForCodeAnal : IBuilder
	{
	public:
		ConfigParseForCodeAnal() : pIn(nullptr) {};
		~ConfigParseForCodeAnal();
		void detach();
		bool Attach(const std::string& name, bool isFile = true);
		Parser* Build();

	private:
		// Builder must hold onto all the pieces

		std::ifstream* pIn;
		Scanner::Toker* pToker;
		Scanner::SemiExp* pSemi;
		Parser* pParser;
		Repository* pRepo;

		// add Rules and Actions

		BeginScope* pBeginScope = nullptr;
		HandleBeginScope* pHandleBeginScope = nullptr;

		EndScope* pEndScope = nullptr;
		HandleEndScope* pHandleEndScope = nullptr;

		PreprocStatement* pPreprocStatement = nullptr;
		HandlePreprocStatement* pHandlePreprocStatement = nullptr;

		NamespaceDefinition* pNamespaceDefinition = nullptr;
		HandleNamespaceDefinition* pHandleNamespaceDefinition = nullptr;

		ClassDefinition* pClassDefinition = nullptr;
		HandleClassDefinition* pHandleClassDefinition = nullptr;

		StructDefinition* pStructDefinition = nullptr;
		HandleStructDefinition* pHandleStructDefinition = nullptr;

		EnumDefinition* pEnumDefinition = nullptr;
		HandleEnumDefinition* pHandleEnumDefinition = nullptr;

		CppFunctionDefinition* pCppFunctionDefinition = nullptr;
		HandleCppFunctionDefinition* pHandleCppFunctionDefinition = nullptr;
		//PrintFunction* pPrintFunction;

		CSharpFunctionDefinition* pCSharpFunctionDefinition = nullptr;
		HandleCSharpFunctionDefinition* pHandleCSharpFunctionDefinition = nullptr;

		ControlDefinition* pControlDefinition = nullptr;
		HandleControlDefinition* pHandleControlDefinition = nullptr;

		CppDeclaration* pCppDeclaration = nullptr;
		HandleCppDeclaration* pHandleCppDeclaration = nullptr;

		CSharpDeclaration* pCSharpDeclaration = nullptr;
		HandleCSharpDeclaration* pHandleCSharpDeclaration = nullptr;

		CppExecutable* pCppExecutable = nullptr;
		HandleCppExecutable* pHandleCppExecutable = nullptr;

		CSharpExecutable* pCSharpExecutable = nullptr;
		HandleCSharpExecutable* pHandleCSharpExecutable = nullptr;

		Default* pDefault = nullptr;
		HandleDefault* pHandleDefault = nullptr;

		// prohibit copies and assignments

		ConfigParseForCodeAnal(const ConfigParseForCodeAnal&) = delete;
		ConfigParseForCodeAnal& operator=(const ConfigParseForCodeAnal&) = delete;
	};
}
#endif
