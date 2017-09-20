/////////////////////////////////////////////////////////////////////
//  Display.cpp - Display facility to show each result				//
//  Language:      Visual C++                                      //
//  Application:   Prototype for CSE687 Pr1, Sp17                  //
//  Author:        Arpit Shah									   //
//  Instructed By: Jim Fawcett                                     //
/////////////////////////////////////////////////////////////////////
#include "Display.h"
Display::Display()
{
}

Display::~Display()
{
}

//display the type analysis information
void Display::displayTypeAnalysis()
{
	NoSqlDb<TypeElement>& typeTableRef_(CodeAnalysis::Repository::getInstance()->getTT());
	for (Key key : typeTableRef_.keys())
	{
		cout << "\n  For Token:" << key;
		auto values = typeTableRef_.value(key);
		cout << "\n    Details:";
		for (size_t i = 0; i < values.size(); i++) {
			cout << values[i].displayElementData();
		}
		cout << "\n";
	}
}

//display the dependency analysis information
void Display::displayDependencyAnalysis()
{
	NoSqlDb<DepElement>& depAnalRef_(CodeAnalysis::Repository::getInstance()->getdepAnal());
	std::cout << std::endl;
	std::cout << "\n Dependency Analysis:" << std::endl;

	for (std::string key : depAnalRef_.keys())
	{
		std::cout << "\n  File:" << key;
		auto values = depAnalRef_.value(key);
		cout << "\n    Depedencies:";
		for (size_t i = 0; i < values.size(); i++) {
			cout << values[i].displayElementData();
		}
		cout << "\n";
	}
}

//display the strong component analysis information
void Display::displayStrongComponentnalysis(StrongComponent anlyse)
{
	std::cout << "\n Strong Component Analysis:" << std::endl << " ";
	int index = 1;
	for (list<string> listStrongCompo : anlyse.getstrongComponentsList()) {
		std::cout << "  " << index << ". ";
		for (string file : listStrongCompo) {
			std::cout << FileSystem::Path::getName(file) << " ";
		}
		index++;
		std::cout << "\n ";
	}
}


#ifdef TEST_DISPLAY
int main()
{
	Display::displayDependencyAnalysis();
	Display::displayTypeAnalysis();
	StrongComponent anal;
	Display::displayStrongComponentnalysis(anal);
}
#endif