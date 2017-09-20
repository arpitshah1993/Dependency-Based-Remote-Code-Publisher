///////////////////////////////////////////////////////////////
// Graph.cpp - Graph Library                                 //
// Ver 1.0                                                   //
// Language:    Visual C++ 2015                              //
// Application: Help for CSE687 - OOD,  Spring 2017          //
// Author:      Arpit Shah							         //
///////////////////////////////////////////////////////////////

#include "Graph.h"
#include <iostream>
#include <fstream>
#include "Graph.h"
using namespace GraphLib;

typedef Graph<std::string, std::string> graph;
typedef Vertex<std::string, std::string> vertex;

#ifdef TEST_GRAPHN
int main()
{
	std::cout << "\n  Testing Graph Library";
	std::cout << "\n =======================\n";

	std::cout << "\n  Constructing Graph instance";
	std::cout << "\n -----------------------------";
	graph g;
	g.addEdge("e1", v1, v2);
	g.addEdge("e2", v2, v3);
	g.addEdge("e3", v3, v1);
	g.addEdge("e4", v3, v4);
	g.addEdge("e5", v5, v2);
	std::cout << "Strong Component:\n";
	g.SCC();
	std::cout << "\n";
}
#endif