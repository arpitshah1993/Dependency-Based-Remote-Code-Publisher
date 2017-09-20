/////////////////////////////////////////////////////////////////////
//  TypeAnal.cpp - Do type Analysis					               //
//  Language:      Visual C++                                      //
//  Application:   Prototype for CSE687 Pr1, Sp17                  //
//  Author:        Arpit Shah									   //
//  Instructed By: Jim Fawcett                                     //
/////////////////////////////////////////////////////////////////////
#include "TypeAnal.h"
using namespace CodeAnalysis;

//constuctor
TypeAnal::TypeAnal() :

	ASTref_(Repository::getInstance()->AST()),
	scopeStack_(Repository::getInstance()->scopeStack()),
	toker_(*(Repository::getInstance()->Toker())),
	typeTableRef_(Repository::getInstance()->getTT()),
	namespaceTableRef_(Repository::getInstance()->getNameSpaceTable())
{
}

//breadth first search on given node 
void TypeAnal::BFS(ASTNode* pNode)
{
	std::queue<ASTNode*> stl;
	stl.push(pNode);
	string parentName = pNode->name_;
	while (stl.size() != 0) {
		bool analysisEnable = false;
		ASTNode* pNode;
		pNode = stl.front();
		stl.pop();
		if (pNode->type_ == "namespace")
		{
			analysisEnable = true;
			parentName = pNode->name_;
			for (size_t i = 0; i < pNode->decl_.size(); i++) {
				DeclarationNode node = pNode->decl_[i];
				Scanner::SemiExp se;
				se.clone(*node.pTc);
				if (se[0] == "using" && se[2] == "=") {
					TypeElement elem(parentName, node.path_, "alias");
					typeTableRef_.saveOrUpdate(se[1], elem);
				}
				else if (se[0] == "typedef") {
					TypeElement elem(parentName, node.path_, "typedef");
					typeTableRef_.saveOrUpdate(se[se.length() - 2], elem);
				}
				else if (se[0] == "using" && se[1] == "namespace") {
					DepElement elem(se[2]);
					namespaceTableRef_.saveOrUpdate(node.path_, elem);
				}
			}
			for (auto pChild : pNode->children_)
			{
				stl.push(pChild);
				if (analysisEnable)
				{
						TypeElement elem(parentName, pChild->path_, pChild->type_);
						typeTableRef_.saveOrUpdate(pChild->name_, elem);
				}
			}
		}
	}
}

//doing type analysis
void TypeAnal::doTypeAnal()
{
	//std::cout << "\n  Starting type analysis:\n";
	ASTNode* pRoot = ASTref_.root();
	BFS(pRoot);
	//show();
}

//display result of type analysis
void TypeAnal::show()
{
	typeTableRef_.ExportDatabaseToXMLDocument("TypaAnalysis.xml");
	for (Key key : typeTableRef_.keys())
	{
		cout << "\n For Key:" << key;
		auto values = typeTableRef_.value(key);
		for (size_t i = 0; i < values.size(); i++) {
			cout << values[i].displayElementData() << "\n";
		}
	}

	std::cout << std::endl;
	std::cout << "\n Namespace Analysis:" << std::endl;

	for (Key key : namespaceTableRef_.keys())
	{
		cout << "\n For File:" << key;
		auto values = namespaceTableRef_.value(key);
		cout << "\n Used namespaces:";
		for (size_t i = 0; i < values.size(); i++) {
			cout << values[i].displayElementData() << "\n";
		}
	}
}

#ifdef TEST_TYPEANAL
int main()
{
	TypeAnal a;
	a.doTypeAnal();
	a.show();
}
#endif