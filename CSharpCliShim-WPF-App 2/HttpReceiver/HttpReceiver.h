#ifndef HTTPRECEIVER_H
#define HTTPRECEIVER_H
///////////////////////////////////////////////////////////////
// Cpp11-HttpReceiver.h - Receiver attached with socket       //
// ver 1.0                                                   //
// Arpit Shah, CSE687 - Object Oriented Design, Spring 2017 //
//////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This is support package which behave as wrapper around socket class.
* It also provide functionality which can be used to receiver the message or files.
*
*Public Interface:
*-----------------
* void InitiateHttpSender(string localURl);  //start Http receiver
* void setAction();							//set up receiver thread on blocking queue
* void startHttpReceiever();						//start receiver thread on blocking queue
* void shutdown();									//shut down the file
* HttpMessage deQ();								//enQ the message in queue
* HttpReceiver();									//deQueue the message in queue
* Required Files:
* ---------------
* Sockets.h, FileSystem.h, HttpMessage.h, BlockingQueue.h,Utilities.h,BlockingQueue.h
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

#include<string>
#include<string>
#include"../Sockets/Sockets.h"
#include"../FileSystem/FileSystem.h"
#include "../Utilities/Utilities.h"
#include"../BlockingQueue/BlockingQueue.h"
#include <iostream>
#include <unordered_map>
#include"../HttpMessage/HttpMessage.h"
using namespace std;
using namespace Utilities;

class HttpReceiver
{
	using EndPoint = string;
	string localUrl, localPath;
	bool connectionClosed_ = false;
	SocketSystem ss;
	std::unordered_map<string, Socket> socketStore;
	std::function<void(Socket socket)> receiveAction = nullptr;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	BlockingQueue<HttpMessage> receiveQ;
	SocketListener sl;
public:
	void enQ(HttpMessage msg);
	HttpMessage deQ();
	HttpReceiver();
	HttpReceiver(string localUrl, string localFilePath);
	void InitiateReceiver(string localUrl, string localFilePath);
	void startHttpReceiver();
	void setAction();
	~HttpReceiver();
};
#endif

