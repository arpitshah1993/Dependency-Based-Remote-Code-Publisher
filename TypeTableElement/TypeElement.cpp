/////////////////////////////////////////////////////////////////////
//  TypeElement.cpp - provide type element to store types	    	//
//  Language:      Visual C++                                      //
//  Application:   Prototype for CSE687 Pr1, Sp17                  //
//  Author:        Arpit Shah									   //
//  Instructed By: Jim Fawcett                                     //
/////////////////////////////////////////////////////////////////////

#include "TypeElement.h"

TypeElement::TypeElement()
{
}

TypeElement::~TypeElement()
{
}

//display element data
std::string TypeElement::displayElementData()
{
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n      " << std::setw(8) << "Namespace" << " : " << this->namepspace;
	out << "\n      " << std::setw(8) << "FilePath" << " : " << this->filePath;
	out << "\n      " << std::setw(8) << "TypeName:" << " : " << this->typeName.c_str();
	out << "\n";
	return out.str();
}

//convert it into string XML file
std::string TypeElement::toStringXML()
{
	std::ostringstream stringtoint;
	stringtoint << "<Namespace>" << namepspace << "</Namespace>" << std::endl;
	stringtoint << "<FilePath>" << filePath << "</FilePath>" << std::endl;
	stringtoint << "<TypeName>" << typeName.c_str() << "</TypeName>" << std::endl;
	return stringtoint.str();
}

//check and return whther it equals or not
bool TypeElement::Equals(TypeElement d)
{
	if (filePath == d.getfilePath() && namepspace == d.getNamepspace() && typeName == d.gettypeName())
		return true;
	return false;
}

//set namespace
void TypeElement::setNamepspace(string namepspace)
{
	this->namepspace = namepspace;
}

//get namespace
string TypeElement::getNamepspace()
{
	return this->namepspace;
}

//set filepath
void TypeElement::setfilePath(string desc)
{
	this->filePath = filePath;
}

//get filepath
string TypeElement::getfilePath()
{
	return this->filePath;
}

//set type name
void TypeElement::settypeName(string data)
{
	this->typeName = data;
}

//get type name
string TypeElement::gettypeName()
{
	return this->typeName;
}

#ifdef TEST_TYPEELEM
int main()
{
	TypeElement elem;
	//std::cout << elem.;
	std::cout << elem.displayElementData();
	std::cout << elem.toStringXML();
}
#endif