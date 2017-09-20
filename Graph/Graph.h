#ifndef GRAPH_H
#define GRAPH_H
///////////////////////////////////////////////////////////////
// Graph.h - Graph Library                                   //
// Ver 1.0                                                   //
// Language:    Visual C++ 2015                              //
// Application: Help for CSE687 - OOD,  Spring 2017          //
// Author:      Arpit Shah							         //
///////////////////////////////////////////////////////////////
/*
Package Operations:
===================
A graph is a collection of vertices, connected by edges.  Each
vertex holds some kind of information held in an instance of
a type V and an integer id, which should be unique with a given
graph.  Each edge connects a parent vertex with a child
vertex and it too holds information in an instance of a type E.

This package provides two classes that support creation of
instances of a graph: Vertex<V,E> and Graph<V,E>.  These are
template-based with parameters V and E, as described above.

The graph holds an aggregated collection of vertices.  Each
vertex may hold one or more edges.  The edge is implemented
as an instance of std::pair<int,E> where the first parameter
is the id of the child vertex and the second parameter is
an instance of the edge information type E.

Note:
It is important that vertex ids be unique.  If you set any of
them with the second argument of the Vertex constructor or with
the id() function it is up to you to ensure uniqueness.  If you
don't explicitly set any ids then the constructor insures that
vertices get sequential ids in the order constructed.

Maintanence Information:
========================
Required files:
---------------
Graph.h, Graph.cpp,
XmlReader.h, XmlReader.cpp,
XmlWriter.h, XmlWriter.cpp

Public Interface:
------------------
Vertex class
------------
typedef std::pair<int, E> Edge;  // graph index of target vertex, edge type
typename typedef std::vector<Edge>::iterator iterator;//define iterator
iterator begin();						//iterator begin
iterator end();							//iterator end
Vertex(V v, size_t id);					//construct vertex with name and size
Vertex(V v);							//constrcut vertx
void add(Edge& edge);					//add edge
Edge& operator[](size_t i);	//index operator
Edge operator[](size_t i) const;	//index operartor to read
V& value();							//get value from vertex
size_t& id();						//return id of given vertx
size_t size();						//return size of vertex collection
bool& mark();						//mark the vertex
V getValue();						//get the value of vertex

Graph class
-----------
typename typedef std::vector< Vertex<V, E> >::iterator iterator;  //itertaor for graph
iterator begin();													//itertaor begin
iterator end();														//itertaor end
Vertex<V, E>& operator[](size_t i);									//index operator
Vertex<V, E> operator[](size_t i) const;							//index operator to read
void addVertex(Vertex<V, E> v);										//add vertex
void addEdge(E eval, Vertex<V, E>& parent, Vertex<V, E>& child);	//add edge
int findVertexIndexByName(string name);								//find vertex by name
size_t findVertexIndexById(size_t id);								//find vertex by ID
vector<list<string>> getstrongComponentsList();						//get strin component list
size_t size();														//get size
template<typename F>
void dfs(Vertex<V, E>& v, F f);										//traverse the graph by dfs
void SCCUtil(int u, int disc[], int low[], std::stack<int>* st, bool stackMember[]); //get strong component analysis
vector<list<string>> SCC();											//Intialize trong component analysis

Build Process:
--------------
Using Visual Studio Command Prompt:
devenv Graph.sln /rebuild debug

Required Files:
---------------
- Graph.h, Graph.cpp

Revision History:
-----------------
ver 1.6 : 23 Feb 17
- added inlines to to helper functions at the end of this file
ver 1.5 : 16 Feb 13
- now supports dfs with both function pointers and functors
ver 1.4 : 11 Feb 13
- removed indent function, now part of XmlWriter,
replaced ctor with default param with two ctors,
one supplies sequential ids, the other accepts user defined id
ver 1.3 : 28 Jan 13
- added GraphToXmlString and GraphFromXmlString functions
ver 1.2 : 25 Jan 13
- changed id() return to reference so can be changed
ver 1.1 : 21 Jan 13
- added prologue comments, compile test directive
ver 1.0 : 14 Jan 13
- first release

*/
/*
* To Do:
* - move GraphToXml and GraphFromXml to separate package
* - Strong Components, Topological Sorting
* - Find disconnected graph partitions
*/

#include <vector>
#include <unordered_map>
#include<algorithm>
#include <string>
#include<stack>
#include<cmath>
#define NIL -1
#include <iostream>
#include <sstream>

using namespace std;
namespace GraphLib
{
	///////////////////////////////////////////////////////////////
	// Vertex class
	template<typename V, typename E>
	class Vertex
	{
	public:
		typedef std::pair<int, E> Edge;  // graph index of target vertex, edge type
		typename typedef std::vector<Edge>::iterator iterator;//define iterator
		iterator begin();						//iterator begin		
		iterator end();							//iterator end
		Vertex(V v, size_t id);					//construct vertex with name and size
		Vertex(V v);							//constrcut vertx
		void add(Edge& edge);					//add edge	
		Edge& operator[](size_t i);	//index operator
		Edge operator[](size_t i) const;	//index operartor to read
		V& value();							//get value from vertex
		size_t& id();						//return id of given vertx
		size_t size();						//return size of vertex collection				
		bool& mark();						//mark the vertex
		V getValue();						//get the value of vertex
	private:
		std::vector<Edge> _edges;
		V _v;
		size_t _id;
		static size_t count;
		bool _mark;
	};
	//----< reserve memory for, and initialize, static count >-----
	template<typename V, typename E>
	size_t Vertex<V, E>::count = 0;

	//----< set and return boolean mark, used for traversal >------
	template<typename V, typename E>
	bool& Vertex<V, E>::mark() { return _mark; }

	template<typename V, typename E>
	inline V Vertex<V, E>::getValue()
	{
		return _v;
	}

	//----< return iterator pointing to first edge >---------------
	template<typename V, typename E>
	typename Vertex<V, E>::iterator Vertex<V, E>::begin() { return _edges.begin(); }

	//----< return iterator pointing to one past last edge >-------
	template<typename V, typename E>
	typename Vertex<V, E>::iterator Vertex<V, E>::end() { return _edges.end(); }

	//----< construct instance, specifying id - must be unique >---
	template<typename V, typename E>
	Vertex<V, E>::Vertex(V v, size_t id) : _v(v), _id(id), _mark(false) {}

	//----< construct instance - creates id sequentially >-------
	template<typename V, typename E>
	Vertex<V, E>::Vertex(V v) : _v(v), _id(count++), _mark(false) {}

	//----< add edge to vertex edge collection >-------------------
	template<typename V, typename E>
	void Vertex<V, E>::add(Edge& edge) { _edges.push_back(edge); }

	//----< index non-const vertex's edges >-----------------------
	template<typename V, typename E>
	typename Vertex<V, E>::Edge& Vertex<V, E>::operator[](size_t i) { return _edges[i]; }

	//----< index const vertex's edges >---------------------------
	template<typename V, typename E>
	typename Vertex<V, E>::Edge Vertex<V, E>::operator[](size_t i) const { return _edges[i]; }

	//----< set and read value of vertex's held type, V >----------
	template<typename V, typename E>
	V& Vertex<V, E>::value() { return _v; }

	//----< return vertex's id >-----------------------------------
	template<typename V, typename E>
	size_t& Vertex<V, E>::id() { return _id; }

	//----< return number of edges >-------------------------------
	template<typename V, typename E>
	size_t Vertex<V, E>::size() { return _edges.size(); }
	template<typename V, typename E>
	class Graph
	{
	public:
		typename typedef std::vector< Vertex<V, E> >::iterator iterator;  //itertaor for graph
		iterator begin();													//itertaor begin
		iterator end();														//itertaor end
		Vertex<V, E>& operator[](size_t i);									//index operator
		Vertex<V, E> operator[](size_t i) const;							//index operator to read
		void addVertex(Vertex<V, E> v);										//add vertex
		void addEdge(E eval, Vertex<V, E>& parent, Vertex<V, E>& child);	//add edge
		int findVertexIndexByName(string name);								//find vertex by name
		size_t findVertexIndexById(size_t id);								//find vertex by ID
		vector<list<string>> getstrongComponentsList();						//get strin component list
		size_t size();														//get size
		template<typename F>
		void dfs(Vertex<V, E>& v, F f);										//traverse the graph by dfs
		void SCCUtil(int u, int disc[], int low[], std::stack<int>* st, bool stackMember[]); //get strong component analysis
		vector<list<string>> SCC();											//Intialize strong component analysis
	private:
		std::vector< Vertex<V, E> > adj;
		std::unordered_map<size_t, size_t> idMap; // id maps to graph index
		vector<list<string>> strongComponentsList;
		template<typename F>
		void dfsCore(Vertex<V, E>& v, F f);

	};
	template<typename V, typename E>
	void Graph<V, E>::SCCUtil(int u, int disc[], int low[], std::stack<int> *st,
		bool stackMember[])
	{
		static int time = 0;
		// Initialize discovery time and low value
		disc[u] = low[u] = ++time;
		st->push(u);
		stackMember[u] = true;
		list <string> strongComponents;
		Vertex<V, E> v = adj[u];
		for (size_t i = 0; i < v.size(); ++i)
		{
			Vertex<V, E>::Edge edge = v[i];
			int v = edge.first;
			if (disc[v] == -1)
			{
				SCCUtil(v, disc, low, st, stackMember);
				low[u] = min(low[u], low[v]);
			}

			else if (stackMember[v] == true)
				low[u] = min(low[u], disc[v]);
		}

		// head node found, pop the stack and print an SCC
		int w = 0;  // To store stack extracted vertices
		if (low[u] == disc[u])
		{
			while (st->top() != u)
			{
				w = (int)st->top();
				strongComponents.push_back(adj[w].getValue());
				stackMember[w] = false;
				st->pop();
			}
			w = (int)st->top();
			strongComponents.push_back(adj[w].getValue());
			strongComponentsList.push_back(strongComponents);
			stackMember[w] = false;
			st->pop();
		}
	}
	// initialize strong component analysis
	template<typename V, typename E>
	vector<list<string>> Graph<V, E>::SCC()
	{
		int *disc = new int[size()];
		int *low = new int[size()];
		bool *stackMember = new bool[size()];
		std::stack<int> *st = new std::stack<int>();

		// Initialize disc and low, and stackMember arrays
		for (size_t i = 0; i < size(); i++)
		{
			disc[i] = NIL;
			low[i] = NIL;
			stackMember[i] = false;
		}

		// Call the recursive helper function to find strongly
		// connected components in DFS tree with vertex 'i'
		for (size_t i = 0; i < size(); i++)
			if (disc[i] == NIL)
				SCCUtil(i, disc, low, st, stackMember);
		return strongComponentsList;
	}
	//----< return iterator pointing to first vertex >-------------
	template<typename V, typename E>
	typename Graph<V, E>::iterator Graph<V, E>::begin() { return adj.begin(); }

	//----< return iterator pointing one past last vertex >--------
	template<typename V, typename E>
	typename Graph<V, E>::iterator Graph<V, E>::end() { return adj.end(); }

	//----< index non-const graph's vertex collection >------------
	template<typename V, typename E>
	typename Vertex<V, E>& Graph<V, E>::operator[](size_t i) { return adj[i]; }

	//----< index const graph's vertex collection >----------------
	template<typename V, typename E>
	typename Vertex<V, E> Graph<V, E>::operator[](size_t i) const { return adj[i]; }

	//----< add vertex to graph's vertex collection >--------------
	template<typename V, typename E>
	void Graph<V, E>::addVertex(Vertex<V, E> v)
	{
		adj.push_back(v);
		idMap[v.id()] = adj.size() - 1;
	}
	//----< return number of vertices in graph's collection >------
	template<typename V, typename E>
	size_t Graph<V, E>::size() { return adj.size(); }

	//----< return index of vertex with specified id >-------------
	template<typename V, typename E>
	size_t Graph<V, E>::findVertexIndexById(size_t id)
	{
		return idMap[id];
	}
	template<typename V, typename E>
	inline vector<list<string>> Graph<V, E>::getstrongComponentsList()
	{
		return strongComponentsList;
	}
	//----< add edge from specified parent to child vertices >-----
	template<typename V, typename E>
	void Graph<V, E>::addEdge(E eVal, Vertex<V, E>& parent, Vertex<V, E>& child)
	{
		size_t childIndex = findVertexIndexById(child.id());
		if (childIndex == adj.size())
			throw std::exception("no edge child");
		size_t parentIndex = findVertexIndexById(parent.id());
		if (parentIndex == adj.size())
			throw std::exception("no edge parent");
		Vertex<V, E>::Edge e;
		e.first = (int)childIndex;
		e.second = eVal;
		adj[parentIndex].add(e);
	}
	template<typename V, typename E>
	inline int Graph<V, E>::findVertexIndexByName(string name)
	{
		//Vertex<V, E> vert;
		for (Vertex<V, E> vert : adj) {
			if (vert.getValue() == name) {
				return (int)vert.id();
			}
		}
		return -1;
	}
	//----< recursive depth first search with action f >-----------
	template<typename V, typename E>
	template<typename F>
	void Graph<V, E>::dfsCore(Vertex<V, E>& v, F f)
	{
		f(v);
		v.mark() = true;
		for (auto edge : v)
		{
			if (adj[edge.first].mark() == false)
				dfsCore(adj[edge.first], f);
		}
		for (auto& vert : adj)
		{
			if (vert.mark() == false)
				dfsCore(vert, f);
		}
	}
	//----< depth first search, clears marks for next search >-----
	template<typename V, typename E>
	template<typename F>
	void Graph<V, E>::dfs(Vertex<V, E>& v, F f)
	{
		dfsCore(v, f);
		for (auto& vert : adj)
			vert.mark() = false;
	}
}
#endif