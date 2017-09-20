/////////////////////////////////////////////////////////////////////
//  DepElement.cpp - provide dependency element to store it in NOSQL //
//  Language:      Visual C++                                      //
//  Application:   Prototype for CSE687 Pr1, Sp17                  //
//  Author:        Arpit Shah									   //
//  Instructed By: Jim Fawcett                                     //
/////////////////////////////////////////////////////////////////////
#include "DepElement.h"

DepElement::DepElement()
{
}

DepElement::~DepElement()
{
}

//return the child
Key DepElement::getchild()
{
	return this->children;
}

//check whether elements are equal are not
bool DepElement::Equals(DepElement d)
{
	if (children.compare(d.getchild()) == 0)
		return true;
	return false;
}

//display the element the data
std::string DepElement::displayElementData()
{
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n      " << std::setw(8) << "DependentFile" << " : " << children;
	return out.str();
}

//return the string XML
std::string DepElement::toStringXML()
{
	std::ostringstream stringtoint;
	stringtoint << "<DependentFile>" << children << "</DependentFile>" << std::endl;
	return stringtoint.str();
}

#ifdef TEST_DEPELEM
int main()
{
	DepElement elem;
	std::cout << elem.getchild();
	std::cout << elem.displayElementData();
	std::cout << elem.toStringXML();
}
#endif