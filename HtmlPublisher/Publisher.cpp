/////////////////////////////////////////////////////////////////////
//  PUblisher.cpp - Publishing facility to create web pages from codes//
//  Language:      Visual C++                                      //
//  Application:   Prototype for CSE687 Pr1, Sp17                  //
//  Author:        Arpit Shah									   //
//  Instructed By: Jim Fawcett                                     //
/////////////////////////////////////////////////////////////////////
#include "Publisher.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <utility>
#include <cctype>
#include <time.h>
#include <locale>
#include<regex>
#include "../FileSystem/FileSystem.h"

using namespace std;
using namespace FileSystem;

//constuctor which read all the predefind html texts which will use during the program exxcution to publish multiple code files
Publisher::Publisher()
{
	std::cout << std::endl;
	readfile("./ImportantFiles/a0.txt", a0);
	std::cout << std::endl;
	readfile("./ImportantFiles/a1.txt", a1);
	std::cout << std::endl;
	readfile("./ImportantFiles/a1_1.txt", a1_1);
	std::cout << std::endl;
	readfile("./ImportantFiles/a2.txt", a2);
	std::cout << std::endl;
	readfile("./ImportantFiles/a3.txt", a3);
	std::cout << std::endl;
	readfile("./ImportantFiles/a4.txt", a4);
	std::cout << std::endl;
	readfile("./ImportantFiles/a5.txt", a5);
	std::cout << std::endl;
	readfile("./ImportantFiles/a6.txt", a6);
	std::cout << std::endl;
	readfile("./ImportantFiles/a7.txt", a7);
	std::cout << std::endl;
	readfile("./ImportantFiles/a8.txt", a8);
	std::cout << std::endl;
	readfile("./ImportantFiles/index.txt", index);
}

Publisher::~Publisher()
{
}

//return the index string
string Publisher::getIndex()
{
	return index;
}

//return the ending part of html
string Publisher::getEnding()
{
	return a3;
}

//read the given file and put into the string
string Publisher::readfile(string filename, string& s)
{
	std::string fileIn = filename;
	File bufferIn(fileIn);
	bufferIn.open(File::in, File::binary);
	if (!bufferIn.isGood())
	{
		std::cout << "\n  could not open \"" << fileIn << "\" for reading";
		return s;
	}
	else
	{
		std::cout << "  opening: \"" << fileIn << "\" for reading";
	}

	const size_t bufLen = 124;
	File::byte buffer[bufLen];
	while (true)
	{
		size_t resultSize = bufferIn.getBuffer(bufLen, buffer);
		std::string temp1(buffer, resultSize);
		s += temp1;
		if (resultSize < bufLen || !bufferIn.isGood())
		{
			bufferIn.close();
			break;
		}
	}
	return s;
}

//search whether vector contains number or not
bool Publisher::contains(int number, std::vector<int> numbers)
{
	for (int i : numbers)
	{
		if (i == number)
			return true;
	}
	return false;
}

//help publisher code to process new line
void Publisher::processNewLine(int& isBEnable, int& buttonNumber, bool& isBracketFound, int& isRBEnable, bool& isRBracketFound, string& startb, string& sl, std::ofstream& out) {
	if (isBEnable > 0 && isBracketFound)
	{
		if (sl.find('{') == 0)
		{
			isBracketFound = false;
			isBEnable--;
			startb = a4 + std::to_string(buttonNumber) + a5 + std::to_string(buttonNumber) + a6 + std::to_string(buttonNumber) + a7;
			buttonNumber++;
			out << startb << sl;
		}
		else
		{
			startb = a4 + std::to_string(buttonNumber) + a5 + std::to_string(buttonNumber) + a6 + std::to_string(buttonNumber) + a7;
			buttonNumber++;
			int bracketIndex = (int)sl.find('{') - 1;
			while (bracketIndex >= 0 && (sl[bracketIndex] == ' ' || sl[bracketIndex] == '\t' || sl[bracketIndex] == '\n'))
				bracketIndex--;
			if (bracketIndex == -1)
			{
				isBracketFound = false;
				isBEnable--;
				out << startb << sl;
			}
			else if (bracketIndex == 0)
			{
				isBracketFound = false;
				isBEnable--;
				out << sl[0] << startb << sl.substr(1, sl.size());
			}
			else if (bracketIndex > 0)
			{
				isBracketFound = false;
				isBEnable--;
				out << sl.substr(0, bracketIndex + 1) << startb << sl.substr(bracketIndex + 1, sl.size());
			}
			else
				out << sl;
		}
		startb.clear();
	}
	else
		out << sl;
	if (isRBEnable > 0 && isRBracketFound)
	{
		isRBracketFound = false;
		isRBEnable--;
		out << a8;
	}
}

//html creation helper
void Publisher::htmlHelper(string& fileSaveName, NoSqlDb<DepElement>& depAnalRef_, ofstream& out,string& fileName)
{
	struct tm newtime;
	__time32_t aclock;
	char buffer[32];
	_time32(&aclock);   // Get time in seconds.
	_localtime32_s(&newtime, &aclock);
	asctime_s(buffer, 32, &newtime);
	out << a0 << "\n  " + fileSaveName + ".htm\n" << "\n  Published on: " << buffer << "\n" << a1 << fileSaveName << a1_1;
	vector<DepElement> elms;
	if (depAnalRef_.contains(fileName))
		elms = depAnalRef_.value(fileName);
	for (DepElement el : elms)
		out << "<a href=\"" << FileSystem::Path::getName(el.getchild(), true) + ".htm\">" << FileSystem::Path::getName(el.getchild(), true) << "</a><br>";
	out << a2;
}
//publish files which is read in the string
void Publisher::publishHtml(std::string fileName, vector<int>& start, vector<int>& end, NoSqlDb<DepElement>& depAnalRef_,string& publishPath)
{
	string fileSaveName = FileSystem::Path::getName(fileName, true), s, sl, startb;
	std::ofstream out(publishPath +fileSaveName + ".htm");
	readfile(fileName, s);
	std::cout << "\n     Publishing process started..";
	s = std::regex_replace(s, std::regex("<"), "&lt;");
	s = std::regex_replace(s, std::regex(">"), "&gt;");
	s += "\n";
	htmlHelper(fileSaveName, depAnalRef_, out, fileName);
	int line = 1, buttonNumber = 0, isBEnable = 0, isRBEnable = 0;
	bool isStat = false, isBracketFound = false, isRBracketFound = false;
	for (size_t i = 0; i < s.length(); i++) {
		if (!isStat &&  s[i] == '\r') continue;
		if (!isStat &&  s[i] == '{') isBracketFound = true;
		if (!isStat &&  s[i] == '}') isRBracketFound = true;
		sl += s[i];
		if (isStat) {
			if (s[i] == '\"') {
				int count = 0, tempIndex = i;
				while (tempIndex>0 && s[--tempIndex] == '\\') 
					count++;
				if(count%2==0) isStat = false;
			}
		}
		else if (s[i] == '\"')
		{
			int count = 0, tempIndex = i;
			while (tempIndex>0 && s[--tempIndex] == '\\') 
				count++;
			if (count % 2 == 0) isStat = true;
		}
		else if (s[i] == '\n') {
			line++;
			processNewLine(isBEnable, buttonNumber, isBracketFound, isRBEnable, isRBracketFound, startb, sl, out);
			if (contains(line, start))
				isBEnable++;
			if (contains(line - 1, end)) {
				if (isRBracketFound)
					out << a8;
				else
					isRBEnable++;
			}
			sl.clear();
			isRBracketFound = isBracketFound = false;
		}
	}
	out << sl << a3;
	out.close();
}

#ifdef TEST_PUBLISH
int main()
{
	Publisher p;
	string s;
	NoSqlDb<DepElement> depAnalRef_;
	p.readfile("abc.cpp", s);
	vector<int> start = { 1,2,3,4 };
	vector<int> end = { 10,20,30,40 };

	p.publishHtml("abc.cpp", start, end, depAnalRef_);
}
#endif