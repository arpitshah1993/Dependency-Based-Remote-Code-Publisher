//////////////////////////////////////////////////////////////
// Server.cpp - Server repository of the file                //
// Ver 1.0                                                   //
// Language:    Visual C++ 2015                              //
// Application: Help for CSE687 - OOD,  Spring 2017          //
// Author:      Arpit Shah							         //
///////////////////////////////////////////////////////////////
#include"../HttpSender/HttpSender.h"
#include"../HttpReceiver/HttpReceiver.h"
#include "../Utilities/Utilities.h"
#include"../FileSystem/FileSystem.h"
#include "../FileSystem/FileSystem.h"
#include "../XmlDocument/XmlParser.h"
#include "../XMLDocument/XmlElement.h"
#include "../XMLDocument/XmlDocument.h"
#include"../Project3TestExecutives/Project3TestExecutives.h"
#include<iostream>
#include<iostream>
#include<unordered_map>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include <cctype>
#include <locale>
#include <fstream>
#include <string>
#include <windows.h>
#include <vector>
#include<unordered_map>

using namespace std;
using namespace XmlProcessing;
using SPtr = std::shared_ptr<AbstractXmlElement>;
using namespace Utilities;
using namespace CodeAnalysis;

//get html file using path as input
string getHtmlFiles(string path)
{
	std::vector<std::string> files = FileSystem::Directory::getFiles(path, "*.htm");
	string result = "";
	for (string file : files)
	{
		result += file + ";";
	}
	return result;
}

//return xml of directory view of repository
string getTreeView(string path)
{
	string repositoryAddress = path;
	std::unordered_map<string, std::vector<string>> treeviewMap;
	std::vector<std::string> currdirs = FileSystem::Directory::getDirectories(repositoryAddress, "*");
	for (size_t i = 0; i < currdirs.size(); ++i)
	{
		string dirAddr = repositoryAddress + currdirs[i] + "/";
		std::vector<std::string> currfiles = FileSystem::Directory::getFiles(dirAddr, "*.*");
		treeviewMap[currdirs[i]] = currfiles;
	}

	XmlDocument doc;
	SPtr pRoot = makeTaggedElement("TreeViewData");
	doc.docElement() = pRoot;
	SPtr elementData = makeTaggedElement("elementData");
	vector<string> keys;
	for (std::pair<string, vector<string>> item : treeviewMap)
	{
		keys.push_back(item.first);
	}

	for (string key : keys)
	{
		SPtr element = makeTaggedElement("element");
		SPtr parent = makeTaggedElement("parent");
		SPtr pkeyparent = makeTextElement(key);
		parent->addChild(pkeyparent);
		element->addChild(parent);

		SPtr child = makeTaggedElement("child");
		for (string childText : treeviewMap[key])
		{
			SPtr stringg = makeTaggedElement("string");
			SPtr stringgText = makeTextElement(childText);
			stringg->addChild(stringgText);
			child->addChild(stringg);
		}

		element->addChild(child);
		elementData->addChild(element);
	}
	pRoot->addChild(elementData);
	return doc.toString();
}

//handling the messages of all types and repond according to them
bool messageHandler(string& msgBody, HttpSender& sndr, HttpMessage& msg, string& publishPath, unordered_map<string, bool>& fileState, ServerRepo& svrvRepo, int argc, char* argv[]) {
	if (msgBody.find("Server Connect") != std::string::npos) {
		sndr.sendMsg("./ImportantFiles/mystyle.css.txt", true, msg.findValue("fromAddr"));
		sndr.sendMsg("./ImportantFiles/mystyle.css.txt", true, msg.findValue("fromAddr"));
		sndr.sendMsg("./ImportantFiles/myscript.js.txt", true, msg.findValue("fromAddr"));
		std::cout << "\n  Sending stylesheet files to client and alloted path where we are publishing files.\n";
		FileSystem::File::copy("./ImportantFiles/myscript.js.txt", publishPath + "myscript.js.txt", false);
		FileSystem::File::copy("./ImportantFiles/mystyle.css.txt", publishPath + "mystyle.css.txt", false);
		return true;}
	else if (msgBody.find("DownLoad File") != std::string::npos) {
		string fileName = StringHelper::split(msgBody, '|')[1]; std::size_t found = fileName.find("/");
		std::string str2 = fileName.substr(found, fileName.length());
		string file = FileSystem::Path::getFullFileSpec(argv[3] + str2);
		if (!FileSystem::File::exists(file)) {
			std::cout << "\n File is not Exist. Chenge your tabs to refresh Directory View.\n";
			return true;} std::cout << "\n Start lazy dependency analysis for"<< file <<" and publish this files and all the file which this file links to and send those to requesting client.\n";
		std::unordered_set<string> filePublished; std::queue<string> fileToPublish; fileToPublish.push(file);
		while (fileToPublish.size() != 0) {
			string fileName = fileToPublish.front();
			fileToPublish.pop();
			Project3TestExecutives execute;
			execute.doPublishing(fileState, fileName, filePublished, fileToPublish, publishPath);
			fileName = FileSystem::Path::getName(fileName, true);
			sndr.sendMsg(publishPath + fileName + ".htm", true, msg.findValue("fromAddr"));
		} std::cout << "\n  File sent";
		sndr.sendMsg("DownLoad File|" + FileSystem::Path::getName(file), false, msg.findValue("fromAddr"));
		return true;}
	else if (msgBody.find("Delete File") != std::string::npos) {
		string fileName = StringHelper::split(msgBody, '|')[1];
		std::size_t found = fileName.find("/");
		std::string str2 = fileName.substr(found, fileName.length());
		string file = FileSystem::Path::getFullFileSpec(argv[3] + str2);
		bool b = FileSystem::File::remove(file);
		std::cout << "\n File Deleted:";
		NoSqlDb<TypeElement>& typeTable = svrvRepo.getInstance()->getTT();
		NoSqlDb<DepElement>& nameSpaceTable = svrvRepo.getInstance()->getNameSpaceTable();
		typeTable.clear();
		nameSpaceTable.clear();
		Project3TestExecutives execute;
		execute.startProcessing(argc, argv, fileState);
		return true;}
	else if (msgBody.find("Deletehtml File") != std::string::npos) {
		vector<string> filetoDelete = StringHelper::split((StringHelper::split(msgBody, '|')[1]), ';');
		for (string file : filetoDelete) {
			file = FileSystem::Path::getFullFileSpec(publishPath + file);
			bool b = FileSystem::File::remove(file);
			std::cout << "file Deleted: " << file;
			sndr.sendMsg("HtmlFileView|" + getHtmlFiles(publishPath), false, msg.findValue("fromAddr"));}
		return true;}
	return false;
}

#ifdef RUN_SERVER
int main(int argc, char* argv[])
{
	try
	{
		string portNumberClient = argv[1], publishPath = argv[2], path = argv[3], serverUrl = "localhost:" + portNumberClient;
		StringHelper::Title("\n Starting Server on port: " + portNumberClient); ::putline();
		StringHelper::Title("\n Server repository: " + path); ::putline();
		unordered_map<string, bool> fileState; ServerRepo svrvRepo;
		{
			Project3TestExecutives execute;
			execute.startProcessing(argc, argv, fileState);
		}
		HttpReceiver rcv(serverUrl, path);
		rcv.startHttpReceiver();
		HttpSender sndr(serverUrl);
		sndr.startHttpSender();
		while (true)
		{
			HttpMessage msg = rcv.deQ();
			std::cout << "\n\n  server recvd message contents:\n" << msg.bodyString();
			std::string msgBody = msg.bodyString();
			if (messageHandler(msgBody, sndr, msg, publishPath, fileState, svrvRepo, argc, argv))
			{
			}
			else if (msgBody.find("Treeview") != std::string::npos)
			{
				sndr.sendMsg(msgBody + getTreeView(path), false, msg.findValue("fromAddr"));
			}
			else if (msgBody.find("HtmlFileView") != std::string::npos)
			{
				//call and crate XML into str
				//string files = StringHelper::split(msgBody, '|')[1];
				sndr.sendMsg(msgBody + getHtmlFiles(publishPath), false, msg.findValue("fromAddr"));
			}
			else if (msgBody.find(".") != std::string::npos)
			{
				string filePath = path + msg.findValue("file");
				Project3TestExecutives execute;
				execute.doTypeAnalysis(FileSystem::Path::getFullFileSpec(filePath), fileState);
			}
		}
		Utilities::waitForUser();
	}
	catch (...) {
		std::cout << "\n\n  Caught exception in Executive::main: \n\n";
	}return 0;
}
#endif