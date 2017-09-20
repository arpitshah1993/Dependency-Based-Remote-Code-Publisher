#ifndef XMLPARSER_H
#define XMLPARSER_H
///////////////////////////////////////////////////////////////////
// XmlParser.h - build XML parse tree                            //
// ver 1.3                                                       //
// Application: Support for XmlDocument, Summer 2015             //
// Platform:    Dell XPS 2720, Win 8.1 Pro, Visual Studio 2013   //
//  Author:        Arpit Shah									 //
//  Instructed By: Jim Fawcett                                   //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* The XML Document Model is essentially a program-friendly wrapper
* around an Abstract Syntax Tree (AST) used to contain the results
* of parsing XML markup.
*
* This package provides parsing of an XML file or string and builds
* it's AST.  See the prologue of the XmlDocument package for more
* details.
*
* XmlParser objects throw if given an invalid path to an XML file.
*
* Required Files:
* ---------------
*   - XmlParser.h, XmlParser.cpp,
*   - XmlElementParts.h, XmlElementParts.cpp
*   - XmlDocument.h, XmlDocument.cpp, XmlElement.h, XmlElement.cpp
*   - ITokenCollection.h, Tokenizer.h, Tokenizer.cpp
*   - Utilities.Lib

* Build Process:
* --------------
* devenv ArpitShahHW2.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.0 : 07 Feb 2017
* - first release
*
*/

#include "Xitokcollection.h"
#include "xmlElementParts.h"///XmlElementParts/xmlElementParts.h"
#include "XmlElement.h"
#include <vector>
#include <stack>
#include <memory>

namespace XmlProcessing
{
	class XmlDocument;

	class XmlParser
	{
	public:
		using sPtr = std::shared_ptr < AbstractXmlElement >;
		using attrib = std::pair < std::string, std::string >;
		using attribs = std::vector <attrib>;
		using ElemStack = std::stack < sPtr >;

		enum sourceType { file, str };
		XmlParser(const std::string& src, sourceType type = file);
		bool good();
		XmlDocument* buildDocument();
		bool verbose(bool verb = true);
	private:
		std::string textFileToString(const std::string& fileSpec);
		void compress(std::string& xmlStr);
		std::string enquoteText(const std::string& src);
		std::string dequoteText(const std::string& src);
		sPtr createXmlDeclar();
		sPtr createProcInstr();
		sPtr createComment();
		sPtr createTaggedElem();
		sPtr createTextElem();
		void processMarkup(ElemStack& stack);
		void processText(ElemStack& stack);
		void processEndElem();
		void extractAttributes();
		void showAttributes();
		attribs& attributes();
		attribs attribs_;
		XITokCollection* pTokColl_;
		XmlParts* pXmlParts_;
		XToker* pToker_ = nullptr;
		std::string src_;
		bool verbose_ = false;
		bool good_ = false;
	};

	inline bool XmlParser::good() { return good_; }
	inline XmlParser::attribs& XmlParser::attributes() { return attribs_; }
	inline bool XmlParser::verbose(bool verb)
	{
		bool temp = verbose_;
		verbose_ = verb;
		return temp;
	}
}
#endif
