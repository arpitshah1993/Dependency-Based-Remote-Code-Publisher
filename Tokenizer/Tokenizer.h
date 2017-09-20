#ifndef TOKENIZER_H
#define TOKENIZER_H
///////////////////////////////////////////////////////////////////////
// Tokenizer.h - read words from a std::stream                       //
// ver 4.1                                                           //
// Language:    C++, Visual Studio 2015                              //
// Platform:    Dell XPS 8900, Windows 10                            //
// Application: Parser component, CSE687 - Object Oriented Design    //
//  Author:        Arpit Shah									     //
//  Instructed By: Jim Fawcett                                       //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a public Toker class and private ConsumeState class.
 * Toker reads words from a std::stream, throws away whitespace and optionally
 * throws away comments.
 *
 * Toker returns words from the stream in the order encountered.  Quoted
 * strings and certain punctuators and newlines are returned as single tokens.
 *
 * This is a new version, based on the State Design Pattern.  Older versions
 * exist, based on an informal state machine design.
 *
 * Build Process:
 * --------------
 * Required Files: Tokenizer.h, Tokenizer.cpp
 *
 * Build Command: devenv Tokenizer.sln /rebuild debug
 */
#include <iosfwd>
#include <string>

namespace Scanner
{
	class ConsumeState;    // private worker class
	struct Context;        // private shared data storage

	class Toker
	{
	public:
		Toker();
		Toker(const Toker&) = delete;
		~Toker();
		Toker& operator=(const Toker&) = delete;
		bool attach(std::istream* pIn);
		std::string getTok();
		bool canRead();
		void returnComments(bool doReturnComments = true);
		bool isComment(const std::string& tok);
		size_t currentLineCount();
		void setSpecialTokens(const std::string& commaSeparatedString);
	private:
		ConsumeState* pConsumer;
		Context* _pContext;
	};
}
#endif
