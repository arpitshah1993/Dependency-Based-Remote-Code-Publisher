///////////////////////////////////////////////////////////////
// Cpp11-HttpSender.h - Sender attached with socket            //
// ver 1.0                                                   //
// Arpit Shah, CSE687 - Object Oriented Design, Spring 2017 //
//////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This is support package which behave as wrapper aroudn socket class.
* It also provide functionality which can be used to send the message or files.
*
*Public Interface:
*-----------------
* void InitiateHttpSender(string localURl);  //start Http sender
* bool isConnected(string url);				//check whther connection is esatcblished or not
* Socket& Connect(string remoteUrl);			//established connection with given url
* void setAction();							//set up sender thread on blocking queue
* void startHttpSender();						//start sender thread on blocking queue
* bool sendMsg(string content, bool file, string remoteUrl,string category="");  //send given message by wrapping it on appropriate wrapper
* HttpMessage makeMessage(size_t n, const std::string & body, const EndPoint & ep, string msgType = "");  //wrap the given content in Http wrapper
* void shutdown();																	//shut down the file
* Required Files:
* ---------------
* Sockets.h, FileSystem.h, HttpMessage.h, BlockingQueue.h
*
* Build Process:
* --------------
* devenv CSharpCliShim-WPF-App.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.0 : 07 Feb 2017
* - first release
*/
#ifndef HTTPSENDER_H
#define HTTPSENDER_H
#include<string>
#include"../Sockets/Sockets.h"
#include"../FileSystem/FileSystem.h"
#include <iostream>
#include <unordered_map>
#include"../HttpMessage/HttpMessage.h"
#include"../BlockingQueue/BlockingQueue.h"

using namespace std;
using namespace Utilities;
class HttpSender
{
	using EndPoint = std::string;
	string localUrl;
	SocketSystem ss;
	SocketConnecter si;
	BlockingQueue<HttpMessage> sendQ;
	std::unordered_map<string, Socket> socketStore;
	std::function<void()> sendAction = nullptr;
public:
	HttpSender();
	HttpSender(string localUrl);
	void InitiateHttpSender(string localURl);
	~HttpSender();
	bool isConnected(string url);
	Socket& Connect(string remoteUrl);
	void setAction();
	void startHttpSender();
	bool sendMsg(string content, bool file, string remoteUrl,string category="");
	HttpMessage makeMessage(size_t n, const std::string & body, const EndPoint & ep, string msgType = "");
	void shutdown();
};
#endif
