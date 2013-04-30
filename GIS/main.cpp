#include <iostream>
#include <string>
#include "BaseGraph.h"
#include "GraphOperations.h"
#include <list>
#include <fstream>

using namespace std;
using namespace GraphOperations;

// --------------------------------------------------------------------------
string pathToString(list<int> &path)
{
	if (path.empty())
		return " -> ";

	string result;
	result += " -> ";
	for (int v : path)
		result += to_string(v) + " -> ";
	return result;
}

static void print(BaseGraph &graph, const char *filename)
{
	ofstream out(filename);

	graph.print(out);
}

static int shortestPair(BaseGraph graph, int from, int to)
{
	// find shorthest path
	SearchResult path = dijkstra(graph, from, to);
	if (path.length < 0) {
		cerr << "Failed to find path from " << from << " to " << to << endl;
		return 1;
	}

	cout << from << pathToString(path.path).c_str() << to << endl;
	cout << "Length: " << path.length << endl;

	print(graph, "graph1a.dot");

	// transform the graph
	int prev = from;
	for (int v : path.path) {
		graph.reverseEdge(prev, v);
		prev = v;
	}
	graph.reverseEdge(prev, to);

	print(graph, "graph1b.dot");

	list<pair<int, int>> clones;

	prev = from;
	int prev2 = path.path.front();
	for (int v : path.path) {
		int v2 = graph.splitVertex(v);
		graph.addEdge(v2, v, 0);
		graph.removeEdge(v2, prev);
		graph.removeEdge(prev2, v);
		clones.emplace_front(v, v2);
		prev = v;
		prev2 = v2;
	}
	graph.removeEdge(prev2, to);

	print(graph, "graph1c.dot");

	// find shorthest pair
	SearchResult path2 = dijkstra_modified(graph, from, to);
	if (path2.length < 0) {
		cerr << "Failed to find path from " << from << " to " << to << endl;
		return 1;
	}

	cout << from << pathToString(path2.path).c_str() << to << endl;
	cout << "Length: " << path2.length << endl;

	// normalize the graph
	for (auto &v : clones)
		graph.mergeVertices(v.first, v.second);

	prev = from;
	for (int v : path.path) {
		graph.reverseEdge(prev, v);
		prev = v;
	}

	// calculate final paths
	// TODO

	return 0;
}

// --------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	int from = 0;
	int to = 0;

	// Check argument number
	if (argc != 2) {
		cerr << "Wrong usage!" << endl;
		cerr << "Usage: disjoint <filename>" << endl;
		return 1;
	}

	// Read graph from a file
	BaseGraph graph;
	try {
		graph.loadFromFile(argv[1]);
	}
	catch (exception &e) {
		cerr << "Error: " << e.what() << endl;
		return 1;
	}

	while (scanf("%d %d", &from, &to) == 2)
		shortestPair(graph, from, to);

	getchar();
	return 0;
}
