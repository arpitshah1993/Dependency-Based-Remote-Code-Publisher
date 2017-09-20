/////////////////////////////////////////////////////////////////////
//  DepAnal.cpp - Do dependency Analysis					       //
//  Language:      Visual C++                                      //
//  Application:   Prototype for CSE687 Pr1, Sp17                  //
/////////////////////////////////////////////////////////////////////
#include "DepAnaly.h"

using namespace CodeAnalysis;

//------<constuctor for dependency analysis>----------

DepAnal::DepAnal() :
	toker_(*(Repository::getInstance()->Toker())),
	typeTableRef_(Repository::getInstance()->getTT()),
	depAnalRef_(Repository::getInstance()->getdepAnal()),
	namespaceTableRef_(Repository::getInstance()->getNameSpaceTable())
{
}

//iterate through all the namespaces of the file and add them if namespaces matches with current namespace
void DepAnal::addFileDependency(string& file, std::vector<TypeElement>& values, bool& isClass, bool& isNameSpace, string& keyFile, string& dependentfile, bool& isAdded,string& nextTok)
{
	for (auto namespacesUsed : namespaceTableRef_.value(file))
	{
		for (size_t i = 0; i < values.size(); i++)
		{
			if (file == values[i].getfilePath()) continue;
			if (values[i].getNamepspace() != namespacesUsed.getchild() && !(values[i].getNamepspace()=="zaxscdvf" && nextTok=="::")) continue;
			updateDependnecy(file, values[i], isClass, isNameSpace, keyFile, dependentfile);
			isAdded = true;
			//break;
		}
		/*if (isAdded == true)
			break;*/
	}
}

//update Dependnecy in dependency list
void DepAnal::updateDependnecy(string file, TypeElement value, bool &isClass, bool &isNameSpace, string& keyFile, string& dependentfile)
{
	string path = value.getfilePath();
	if (value.getNamepspace() == "zaxscdvf")
	{
		vector<string> mergePath = Utilities::StringHelper::split(path,'|');
		path = mergePath[0];
		if (mergePath[1] != file)
			return;
	}

	if (file == path) return;
	depAnalRef_.saveOrUpdate(file, DepElement(path));
	if (value.gettypeName() == "class")
	{
		isClass = true;
		keyFile = file;
		dependentfile = path;
	}
	if (value.gettypeName() == "namespace")
		isNameSpace = true;
}

//------<do dependency analysis>----------
void DepAnal::doDepAnal(string file)
{
	std::ifstream in(file);
	Scanner::Toker toker;
	toker.returnComments();
	toker.attach(&in);
	bool isClass = false, isNameSpace = false;
	string keyFile, dependentfile;
	string nextTok = toker.getTok();
	do {
		string tok = nextTok;
		nextTok= toker.getTok();
		isClass = false;
		if (tok == "::" && (isNameSpace))
		{
			toker.getTok();
			isNameSpace = false;
			continue;
		}
		isNameSpace = false;
		if (tok == "\n") tok = "newline";
		if (!typeTableRef_.contains(tok)) continue;
		bool shouldAdd = true;
		auto values = typeTableRef_.value(tok);
		/*for (size_t i = 0; i < values.size(); i++) {
			if (values[i].getfilePath() != file) continue;
			shouldAdd = false; break;
		}*/
		if (shouldAdd && values.size() == 1 && file!= values[0].getfilePath())
			updateDependnecy(file, values[0], isClass, isNameSpace, keyFile, dependentfile);

		else if (shouldAdd && values.size() > 1) {
			bool isAdded = false;
			if (namespaceTableRef_.contains(file))
			{
				addFileDependency(file, values, isClass, isNameSpace, keyFile, dependentfile, isAdded , nextTok);
			}
			else if (!(namespaceTableRef_.contains(file)) || !isAdded)
			{
				for (size_t i = 0; i < values.size(); i++)
					updateDependnecy(file, values[0], isClass, isNameSpace, keyFile, dependentfile);
			}
		}
	} while (in.good());
	in.close();
}

//------<show dependency analysis>----------
void CodeAnalysis::DepAnal::show()
{
	depAnalRef_.ExportDatabaseToXMLDocument("DependencyAnalysis.xml");
	std::cout << std::endl;
	std::cout << "\n Dependency Analysis:" << std::endl;

	for (Key key : depAnalRef_.keys())
	{
		cout << "\n File:" << key;
		auto values = depAnalRef_.value(key);
		cout << "\n Depedencies:";
		for (size_t i = 0; i < values.size(); i++) {
			cout << values[i].displayElementData() << "\n";
		}
	}
}

#ifdef TEST_DEPANAL
int main()
{
	DepAnal a;
	a.doDepAnal("abc.xml");
	a.show();
}
#endif