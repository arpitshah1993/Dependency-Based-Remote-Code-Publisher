/////////////////////////////////////////////////////////////////////
//  Tokenizer.cpp - Reads words from a file                        //
//  ver 1.9                                                        //
//  Application:   Prototype for CSE687 Pr1,	                   //
//  Author:        Arpit Shah									   //
//  Instructed By: Jim Fawcett                                     //
/////////////////////////////////////////////////////////////////////

#include <cctype>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <assert.h>
#include "XTokenizer.h"

//----< constructor may be called with no argument >-----------
XmlProcessing::XToker::XToker(const std::string& src, bool isFile)
	: prevprevChar(0), prevChar(0), currChar(0), nextChar(0),
	doReturnComments(false), numLines(0), braceCount(0), pIn(0),
	_state(default_state)
{
	if (src.length() > 0)
	{
		if (!attach(src, isFile))
		{
			std::string temp = std::string("can't open ") + src;
			throw std::exception(temp.c_str());
		}
	}
	scTok = "()[]{};.\n";
	if (_mode == xml)
		scTok = "<>!" + scTok;
}

//----< destructor >-------------------------------------------
XmlProcessing::XToker::~XToker()
{
	if (pIn)
	{
		pIn->clear();
		std::ifstream* pFs = dynamic_cast<std::ifstream*>(pIn);
		if (pFs)
		{
			pFs->close();
		}
		delete pIn;
	}
}

//----< set mode of tokenizing, e.g., code or xml >------------
void XmlProcessing::XToker::setMode(XToker::mode md)
{
	_mode = md;
	scTok = "()[]{};.\n";
	if (_mode == xml)
		scTok = "<>!" + scTok;
}

//----< set mode of tokenizing, e.g., code or xml >------------

void XmlProcessing::XToker::setSingleCharTokens(std::string tokChars)
{
	_mode = custom;
	scTok = tokChars;
}
//----< attach tokenizer to a source file or string >----------

bool XmlProcessing::XToker::attach(const std::string& name, bool srcIsFile)
{
	if (pIn && srcIsFile)
	{
		pIn->clear();
		std::ifstream* pFs = dynamic_cast<std::ifstream*>(pIn);
		if (pFs)
		{
			pFs->close();
		}
		delete pIn;
	}
	if (srcIsFile)
		pIn = new std::ifstream(name.c_str());
	else
		pIn = new std::istringstream(name.c_str());
	return pIn->good();
}
//----< peek function that works with multiple putbacks >------

int XmlProcessing::XToker::peek()
{
	if (putbacks.size() > 0)
		return putbacks[putbacks.size() - 1];
	else
		return pIn->peek();

}
//----< multiple putBack that won't break peek >---------------

void XmlProcessing::XToker::putback(int ch)
{
	putbacks.push_back(ch);
	nextChar = ch;
	currChar = prevChar;
	prevChar = prevprevChar;
}
//----< get consistent with peek and putback >-----------------

int XmlProcessing::XToker::get()
{
	if (putbacks.size() > 0)
	{
		char ch = putbacks.front();
		putbacks.pop_back();
		return ch;
	}
	return pIn->get();
}
//
//----< extract character from attached stream >---------------

bool XmlProcessing::XToker::getChar()
{
	char oldNext = nextChar;
	prevprevChar = prevChar;
	prevChar = currChar;
	currChar = this->get();
	nextChar = this->peek();
	_ASSERT(currChar == oldNext || oldNext == 0);
	if (currChar == '\n')
		++numLines;
	if (currChar == '{' && _state == default_state)
		++braceCount;
	if (currChar == '}' && _state == default_state)
		--braceCount;
	return !pIn->eof();
}
//----< is this char a single char token? >--------------------

bool XmlProcessing::XToker::isSingleCharTok(char ch)
{
	if (scTok.find(ch) < scTok.length())
		return true;
	return false;
}
//----< remove contiguous white space except for newline >-----

void XmlProcessing::XToker::stripWhiteSpace()
{
	if (nextChar == '\n')
		return;
	while (isspace(nextChar) && nextChar != '\n')
	{
		getChar();
	}
}
//----< is this an indentifier character? >--------------------

bool XmlProcessing::XToker::isIdentifierChar(char ch)
{
	if (isalpha(ch) || ch == '_' || isdigit(ch))
		return true;
	return false;
}
//----< is this the end of a token? >--------------------------

bool XmlProcessing::XToker::isTokEnd()
{
	if (isspace(nextChar))
		return true;
	if (isSingleCharTok(nextChar) || isSingleCharTok(currChar))
		return true;
	if (isIdentifierChar(currChar) && !isIdentifierChar(nextChar))
		return true;
	if (!isIdentifierChar(currChar) && isIdentifierChar(nextChar))
		return true;
	if (isFileEnd())
		return true;
	return false;
}
//----< is this the beginning of a comment? >------------------

bool XmlProcessing::XToker::isBeginComment()
{
	if (prevChar != '\\' && currChar == '/' && nextChar == '*')
	{
		aCppComment = false;
		return true;
	}
	if (prevChar != '\\' && currChar == '/' && nextChar == '/')
	{
		aCppComment = true;
		return true;
	}
	return false;
}
//----< is this the end of a comment? >------------------------

bool XmlProcessing::XToker::isEndComment()
{
	if (aCppComment && currChar != '\\' && nextChar == '\n')
		return true;
	if (!aCppComment && prevChar != '\\' && currChar == '*' && nextChar == '/')
		return true;
	return false;
}
//----< return comment as a token >----------------------------

std::string XmlProcessing::XToker::eatComment()
{
	_state = comment_state;
	std::string tok(1, currChar);
	while (!isEndComment() && pIn->good())
	{
		getChar();
		tok.append(1, currChar);
	}

	if (!aCppComment)
	{
		getChar();
		tok.append(1, currChar);
	}
	_state = default_state;
	return tok;
}
//----< is this the beginning of a quote? >--------------------

bool XmlProcessing::XToker::isBeginQuote()
{
	if (prevChar != '\\' && currChar == '\'')
	{
		aSingleQuote = true;
		return true;
	}
	if (prevChar != '\\' && currChar == '\"')
	{
		aSingleQuote = false;
		return true;
	}
	return false;
}
//----< is this the end of quote? >----------------------------

bool XmlProcessing::XToker::isEndQuote()
{
	if (prevChar == '\\' || currChar != '\\')
	{
		if (aSingleQuote && nextChar == '\'')
			return true;
		if (!aSingleQuote && nextChar == '\"')
			return true;
	}
	return false;
}
//----< return single or double quote as token >---------------

std::string XmlProcessing::XToker::eatQuote()
{
	_state = quote_state;
	std::string tok(1, currChar);
	while (!isEndQuote())
	{
		if (!pIn->good())
			throw std::exception("missing end of quote");
		getChar();
		tok.append(1, currChar);
	}
	getChar();
	tok.append(1, currChar);
	_state = default_state;
	return tok;
}
//----< read token from attached file >------------------------

std::string XmlProcessing::XToker::getTok()
{
	std::string tok = "";
	stripWhiteSpace();
	if (isSingleCharTok(nextChar))
	{
		getChar();
		tok.append(1, currChar);
		return tok;
	}
	do
	{
		if (isFileEnd())
			return tok;

		getChar();
		if (isBeginComment())
		{
			if (tok.length() > 0)
			{
				this->putback(currChar);
				return tok;
			}
			tok = eatComment();
			if (doReturnComments)
				return tok;
			else
			{
				tok = "";
				continue;
			}
		}
		if (isBeginQuote())
		{
			if (tok.length() > 0)
			{
				this->putback(currChar);
				return tok;
			}
			tok = eatQuote();
			return tok;
		}
		if (!isspace(currChar))
			tok.append(1, currChar);
	} while (!isTokEnd() || tok.length() == 0);
	return tok;
}
//----< test stub >--------------------------------------------

#ifdef TEST_TOKENIZER

int main(int argc, char* argv[])
{
	XToker t_fromStr("tokens from a string: -> int x; /* a comment */", false);
	std::string tok;
	do{
		tok = t_fromStr.getTok();
		std::cout << "  " << tok;
	} while (tok != "");
	std::cout << "\n\n";
	if (argc < 2)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}

	for (int i = 1; i<argc; ++i)
	{
		std::cout << "\n  Processing file " << argv[i];
		std::cout << "\n  " << std::string(16 + strlen(argv[i]), '-') << "\n";

		try
		{
			Toker t;
			t.setMode(Toker::xml);        // comment out to show tokenizing for code
			if (!t.attach(argv[i]))
			{
				std::cout << "\n    can't open file " << argv[i] << "\n\n";
				continue;
			}
			t.returnComments();  // remove this statement to discard comment tokens
			std::string temp;
			do
			{
				temp = t.getTok();
				std::cout << "  ln: " << t.lines() << ", br lev: "
					<< t.braceLevel() << ", tok size: " << std::setw(3) << temp.length() << " -- ";
				if (temp != "\n")
					std::cout << temp << std::endl;
				else
					std::cout << "newline\n";
			} while (temp != "");
		}
		catch (std::exception& ex)
		{
			std::cout << "  " << ex.what() << "\n\n";
		}
	}
}

#endif
