/////////////////////////////////////////////////////////////////////
//  TestExecutiveClient.h - Demonstartions	requirement     	   //
//  Language:      Visual C++                                      //
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
* ~Project3TestExecutives();
* void Requirement1();                                                   //demonstrate requirement 1
* void Requirement2();													//demonstrate requirement 2
* void Requirement3();													//demonstrate requirement 3
* void Requirement4(int argc, char * argv[]);								//demonstrate requirement 4
* void Requirement5();													//demonstrate requirement 5
* void Requirement6(StrongComponent& anlyse);								//demonstrate requirement 6
* void Requirement7(int argc, char * argv[], StrongComponent& anlyse);	//demonstrate requirement 7
* void Requirement8();													//demonstrate requirement 8
* void Requirement9();													//demonstrate requirement 9
* void Requirement10();													//demonstrate requirement 10
* void Requirement11();													//demonstrate requirement 11
*void connect(HttpSender & sndr, HttpReceiver & rcv, string & serverAddress);     // demonstarte connection
*void upload(HttpSender & sndr, HttpReceiver & rcv, string & serverAddress);		//demonstarte file uploading
*void download(HttpSender & sndr, HttpReceiver & rcv, string & serverAddress);    //demonstarte downloading and opening of the file
*void deleteFile(HttpSender & sndr, HttpReceiver & rcv, string & serverAddress);   // demonstarte deletion of code file
*void deleteHtmlFile(HttpSender & sndr, HttpReceiver & rcv, string & serverAddress); //demonstarte deletion of html file
*
* Required Files:
* ---------------
* TestExecutiveClient.h, TestExecutiveClient.cpp, Utilities.h, HttpSender.h, HttpReceiver.h
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


#ifndef MAINEXECUTE_Client
#define MAINEXECUTE_Client
#include<iostream>
using namespace std;
#include"../Utilities/Utilities.h"
#include"../HttpSender/HttpSender.h"
#include"../HttpReceiver/HttpReceiver.h"
using namespace Utilities;
class TestExecutiveClient
{
public:
	TestExecutiveClient();
	~TestExecutiveClient();
		void Requirement1();                                                   //demonstrate requirement 1
		void Requirement2();													//demonstrate requirement 2
		void Requirement3();													//demonstrate requirement 3
		void Requirement4();													//demonstrate requirement 4
		void Requirement5();													//demonstrate requirement 5
		void Requirement6();													//demonstrate requirement 6
		void Requirement7();													//demonstrate requirement 7
		void Requirement8();													//demonstrate requirement 8
		void Requirement9();													//demonstrate requirement 9
		void Requirement10();													//demonstrate requirement 10
		void Requirement11();
		void connect(HttpSender & sndr, HttpReceiver & rcv, string & serverAddress);     // demonstarte connection
		void upload(HttpSender & sndr, HttpReceiver & rcv, string & serverAddress);		//demonstarte file uploading
		void download(HttpSender & sndr, HttpReceiver & rcv, string & serverAddress);    //demonstarte downloading and opening of the file
		void deleteFile(HttpSender & sndr, HttpReceiver & rcv, string & serverAddress);   // demonstarte deletion of code file
		void deleteHtmlFile(HttpSender & sndr, HttpReceiver & rcv, string & serverAddress); //demonstarte deletion of html file
};
#endif