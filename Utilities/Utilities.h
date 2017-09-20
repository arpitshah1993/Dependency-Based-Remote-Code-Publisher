#ifndef UTILITIES_H
#define UTILITIES_H
///////////////////////////////////////////////////////////////////////
// Utilities.h - small, generally useful, helper classes             //
//  Author:        Arpit Shah									     //
//  Instructed By: Jim Fawcett                                       //
// Application: Most Projects, CSE687 - Object Oriented Design       //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides classes StringHelper and Converter and a global
* function putline().  This class will be extended continuously for
* awhile to provide convenience functions for general C++ applications.
*
* Build Process:
* --------------
* Required Files: Utilities.h, Utilities.cpp
*
* Build Command: devenv Utilities.sln /rebuild debug
*/
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include<regex>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include<vector>
#include <locale>

using namespace std;

namespace Utilities
{
	class StringHelper
	{
	public:
		static std::vector<std::string> split(const std::string& src);
		//static void Title(const std::string& src, char underline = '-');
		static std::vector<std::string> split(const std::string & src, const char & c);
		//static void title(const std::string& src);
		static std::string trim(const std::string& src);
		static std::string addHeaderAndFooterLines(const std::string& src);
		static void Title(std::string src, std::ostream&  out = std::cout);
		static void Title(std::string src, std::ostringstream& out);
		static void title(std::string src, std::ostream& out = std::cout);
		static void title(std::string src, std::ostringstream& out);
		static void sTitle(
			std::string src, size_t offset, size_t width, std::ostream& out = std::cout, char underline = '-'
		);
		static void sTitle(
			std::string src, size_t offset, size_t width, std::ostringstream& out, char underline = '-'
		);
	};

	void putline();

	void waitForUser();
	//{
	//	/*int i;
	//	std::cin >> i;*/
	//	std::cin.get();
	//}
	///////////////////////////////////////////////////////////////////
	// DisplayLocation writes start address, ending address and size
	// to console

	std::string ToDecAddressString(size_t address);
	std::string ToHexAddressString(size_t address);



	template<typename T>
	void DisplayLocation(T& t)
	{
		size_t address = reinterpret_cast<size_t>(&t);
		size_t size = sizeof(t);

		std::cout << ToDecAddressString(address)
			<< " : "
			<< ToDecAddressString(address + size)
			<< ", " << size;
	}


	template <typename T>
	class Converter
	{
	public:
		static std::string toString(const T& t);
		static T toValue(const std::string& src);
	};

	template <typename T>
	std::string Converter<T>::toString(const T& t)
	{
		std::ostringstream out;
		out << t;
		return out.str();
	}

	template<typename T>
	T Converter<T>::toValue(const std::string& src)
	{
		std::istringstream in(src);
		T t;
		in >> t;
		return t;
	}

}
#endif
