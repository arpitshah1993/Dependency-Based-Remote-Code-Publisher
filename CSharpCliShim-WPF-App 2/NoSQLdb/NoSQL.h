#ifndef NoSQL_H
#define NoSQL_H
///////////////////////////////////////////////////////////////
// Cpp11-NoSQL.h - NoSQL database system                     //
// ver 1.0                                                   //
// Arpit Shah, CSE687 - Object Oriented Design, Spring 2017 //
//////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains NoSQLdb<T> and Elements<T>. It contains
* methods to add, delete, edit the database and its elements value.
* It provides an No sql storage system which is the core functionalty
* of the class.
*
*Public Interface:
*-----------------
* void clear();											//clear the database
* Keys keys();											//return keys
* bool saveOrUpdate(Key key, Data elem);					//save or update the keys or element in database
* bool contains(Key key);									//return true if contains
* vector<Data>& value(Key key);							//return the value of specified key
* size_t count();											//retrun size of database entries
* std::string ExportDatabaseToXMLDocument(string);		//Export Database To XML Document.

* Required Files:
* ---------------
* NoSQL.h, NoSQL.cpp, DepElement.h, TypeElement.h, FileSystem.h, XmlParser.h, XmlElement.h, XmlDocument.h
*
* Build Process:
* --------------
* devenv ArpitShahHW2.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.0 : 07 Feb 2017
* - first release
*/

#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <iostream>
#include"../DependencyTableElement/DepElement.h"
#include"../TypeTableElement/TypeElement.h"
#include "../FileSystem/FileSystem.h"
#include "../XmlDocument/XmlParser.h"
#include "../XMLDocument/XmlElement.h"
#include "../XMLDocument/XmlDocument.h"

using namespace std;
using namespace XmlProcessing;
using Key = std::string;
using Keys = std::vector<Key>;
using SPtr = std::shared_ptr<AbstractXmlElement>;
/*
NoSqlDb class is work as system which has all the database related functionalities
*/
template<typename Data>
class NoSqlDb
{
private:
	using Item = std::pair<Key, vector<Data>>;
	std::unordered_map<Key, vector<Data>> store;
public:
	NoSqlDb();
	void clear();											//clear the database
	Keys keys();											//return keys
	bool saveOrUpdate(Key key, Data elem);					//save or update the keys or element in database
	bool contains(Key key);									//return true if contains
	vector<Data>& value(Key key);							//return the value of specified key
	size_t count();											//retrun size of database entries
	std::string ExportDatabaseToXMLDocument(string);		//Export Database To XML Document.
};

template<typename Data>
void NoSqlDb<Data>::clear()
{
	store.clear();
}

/*
Export Database To XML Document.
*/
template<typename Data>
std::string NoSqlDb<Data>::ExportDatabaseToXMLDocument(string fileName)
{
	XmlDocument doc;
	SPtr pRoot = makeTaggedElement("NoSQLdb");
	doc.docElement() = pRoot;
	Keys keys = this->keys();
	for (Key key : keys) {
		auto element = value(key);
		SPtr recordElem = makeTaggedElement("Records");

		SPtr keyElem = makeTaggedElement("Key");
		SPtr pkeyTextElem = makeTextElement(key);
		keyElem->addChild(pkeyTextElem);

		SPtr valueElem = makeTaggedElement("Value");
		for (auto elems : element) {
			SPtr pTextElem = makeTextElement(elems.toStringXML());
			valueElem->addChild(pTextElem);
		}
		recordElem->addChild(keyElem);
		recordElem->addChild(valueElem);
		pRoot->addChild(recordElem);
	}
	std::ofstream out(fileName);
	out << doc.toString();
	out.close();
	return  doc.toString();
}

/*
default constuctor.
*/
template<typename Data>
inline NoSqlDb<Data>::NoSqlDb()
{
}

/*
return all the keys from database.
*/
template<typename Data>
inline Keys NoSqlDb<Data>::keys()
{
	Keys keys;
	for (Item item : store)
	{
		keys.push_back(item.first);
	}
	return keys;
}

/*
It will save element and key if it is not there. If it is there in
database, it wil upadate it.
*/
template<typename Data>
inline bool NoSqlDb<Data>::saveOrUpdate(Key key, Data elem)
{
	bool shouldAdd = true;
	vector<Data> depTypes = this->store[key];
	for (size_t i = 0; i < depTypes.size(); i++) {
		if (elem.Equals(depTypes[i])) {
			shouldAdd = false;
		}
	}
	if (shouldAdd) {
		depTypes.push_back(elem);
		this->store[key] = depTypes;
		return true;
	}
	return false;
}

/*
check whether it contains the key or not
*/
template<typename Data>
inline bool NoSqlDb<Data>::contains(Key key)
{
	if (store.find(key) != store.end())
		return true;
	return false;
}

/*
Return value of specified key.
*/
template<typename Data>
inline vector<Data>& NoSqlDb<Data>::value(Key key)
{
	if (store.find(key) != store.end())
		return store[key];
	return store.begin()->second;
}

/*
Return count of entry database have.
*/
template<typename Data>
inline size_t NoSqlDb<Data>::count()
{
	return store.size();
}
#endif



