///////////////////////////////////////////////////////////////
//  StrongComponent.cpp - Do Strong Component Analysis	     //
// ver 1.0                                                   //
// Arpit Shah,  CSE687 - Object Oriented Design, Spring 2017 //
///////////////////////////////////////////////////////////////
#include "StrongComponent.h"

StrongComponent::StrongComponent()
{
}

StrongComponent::~StrongComponent()
{
}

// analyse strong component
void StrongComponent::analyseStrongComponeent(NoSqlDb<DepElement> depAnalRef_)
{
	int i = 0;
	for (std::string key : depAnalRef_.keys()) {
		if (graph.findVertexIndexByName(key) == -1)
		{
			GraphLib::Vertex<std::string, std::string> vert(key);
			graph.addVertex(vert);
		}
		for (DepElement dependencies : depAnalRef_.value(key)) {
			if (graph.findVertexIndexByName(dependencies.getchild()) != -1) continue;
			GraphLib::Vertex<std::string, std::string> vertDepen(dependencies.getchild());
			graph.addVertex(vertDepen);
		}
	}
	for (std::string key : depAnalRef_.keys()) {
		for (DepElement dependencies : depAnalRef_.value(key)) {
			if ((graph.findVertexIndexByName(key)) != -1 && graph.findVertexIndexByName(dependencies.getchild()) != -1)
			{
				GraphLib::Vertex<std::string, std::string>& verKey = graph[(graph.findVertexIndexByName(key))];
				GraphLib::Vertex<std::string, std::string>& verDep = graph[graph.findVertexIndexByName(dependencies.getchild())];
				graph.addEdge(std::to_string(++i), verKey, verDep);
			}
		}
	}
	graph.SCC();
}

//convert analysis result into XML file
void StrongComponent::toXMLfile(string fileName)
{
	XmlDocument doc;
	SPtr pRoot = makeTaggedElement("StorngComponentsData");
	doc.docElement() = pRoot;
	for (list<string> listStrongCompo : graph.getstrongComponentsList()) {
		SPtr recordElem = makeTaggedElement("FileComponentes");
		for (string file : listStrongCompo) {
			SPtr valueElem = makeTaggedElement("File");
			SPtr pTextElem = makeTextElement(file);
			valueElem->addChild(pTextElem);
			recordElem->addChild(valueElem);
		}
		pRoot->addChild(recordElem);
	}
	std::ofstream out(fileName);
	out << doc.toString();
	out.close();
}

//get result List
vector<list<string>> StrongComponent::getstrongComponentsList()
{
	return graph.getstrongComponentsList();
}

#ifdef SC_TEST
int main()
{
	StrongComponent anal;
	anal.analyseStrongComponeent();
	vector<list<string>> list = anal.getstrongComponentsList();
	anal.toXMLfile("XYZ.xml");
}
#endif
