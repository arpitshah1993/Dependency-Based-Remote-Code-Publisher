
#ifndef PUBLISHER_H
#define PUBLISHER_H
/////////////////////////////////////////////////////////////////////
//  PUblisher.h - Publishing facility to create web pages from codes//
//  Language:      Visual C++                                      //
//  Application:   Prototype for CSE687 Pr1, Sp17                  //
//  Author:        Arpit Shah									   //
//  Instructed By: Jim Fawcett                                     //
/////////////////////////////////////////////////////////////////////
/*
* Module Purpose:
* ===============
*   PUblisher.h - Publishing facility to create web pages from codes
*
* Public Interface:
* ----------------
* 	string getIndex();  //get index web page html part string
*	string getEnding();  //get end part of web page html  string
*	string readfile(string filename, string & s); //reading given file and put it into string
*	bool contains(int, std::vector<int>);  //search whether it vector cotains number or not
*	void processNewLine(int & isBEnable, int & buttonNumber, bool & isBracketFound, int & isRBEnable, bool & isRBracketFound, string & startb, string & sl, std::ofstream & out); //help publishing the read file when it comes to new line
*	void publishHtml(std::string fileName, vector<int>& start, vector<int>& end, NoSqlDb<DepElement>& depAnalRef_);  //publis the readfile into web page
*   void htmlHelper(string & fileSaveName, NoSqlDb<DepElement>& depAnalRef_, ofstream & out, string & fileName);	//html creation helper
*
*  Required Files:
*  ---------------
*  - ActionsAndRules.h
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
#include<vector>
#include"../Parser/ActionsAndRules.h"
#include <ctime>
#include <iostream>

class Publisher
{
	//strings to read predefined html text which will help to create html files
	//this strings will help to add predefind parts of html web pages files
	string a0, a1, a1_1, a2, a4, a5, a6, a7, a3, a8, index;
	const time_t ctt = time(0);
public:
	Publisher();
	~Publisher();
	string getIndex();  //get index web page html part string
	string getEnding();  //get end part of web page html  string
	string readfile(string filename, string & s); //reading given file and put it into string
	bool contains(int, std::vector<int>);  //search whether it vector cotains number or not
	void processNewLine(int & isBEnable, int & buttonNumber, bool & isBracketFound, int & isRBEnable, bool & isRBracketFound, string & startb, string & sl, std::ofstream & out); //help publishing the read file when it comes to new line
	void htmlHelper(string & fileSaveName, NoSqlDb<DepElement>& depAnalRef_, ofstream & out, string & fileName);	//html creation helper
	void publishHtml(std::string fileName, vector<int>& start, vector<int>& end, NoSqlDb<DepElement>& depAnalRef_, string & publishPath);
	//void publishHtml(std::string fileName, vector<int>& start, vector<int>& end, NoSqlDb<DepElement>& depAnalRef_);  //publis the readfile into web page
};
#endif // !PUBLISHER_H