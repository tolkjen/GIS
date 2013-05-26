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

static void untanglePaths(const list<pair<int, int>> &path1,
		const list<pair<int, int>> &path2, list<pair<int, int>> &out)
{
	auto begin1 = path1.begin();
	auto begin2 = path2.begin();
	auto end1 = path1.end();
	auto end2 = path2.end();
	auto it1 = begin1;

	while (it1 != end1) {
		bool skip = false;
		for (auto it2 = begin2; it2 != end2; ++it2) {
			if (it2->first != it1->second)
				continue;
			if (it2->second != it1->first)
				continue;
			swap(begin1, begin2);
			swap(end1, end2);
			it1 = it2;
			++it1;
			skip = true;
			break;
		}
		if (skip)
			continue;
		out.push_back(*it1);
		++it1;
	}
}

static int shortestPair(BaseGraph &srcGraph, int from, int to)
{
	BaseGraph graph(srcGraph);

	// find shorthest path
	SearchResult path = dijkstra(graph, from, to);
	if (path.length < 0) {
		cerr << "Failed to find path from " << from << " to " << to << endl;
		return 1;
	}
#ifdef DEBUG
	cout << from << pathToString(path.path).c_str() << to << endl;
	cout << "Length: " << path.length << endl;
#endif
	print(graph, "graph1a.dot");

	// transform the graph
	int prev = from;
	for (int v : path.path) {
		graph.reverseEdge(prev, v);
		prev = v;
	}
	graph.reverseEdge(prev, to);

	print(graph, "graph1b.dot");

	map<int, int> clones;

	prev = from;
	int prev2 = path.path.front();
	for (int v : path.path) {
		int v2 = graph.splitVertex(v);
		graph.addEdge(v2, v, 0);
		graph.removeEdge(v2, prev);
		graph.removeEdge(prev2, v);
		clones[v2] = v;
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
#ifdef DEBUG
	cout << from << pathToString(path2.path).c_str() << to << endl;
	cout << "Length: " << path2.length << endl;
#endif
	// calculate final paths
	for (int &v : path2.path) {
		auto it = clones.find(v);
		if (it != clones.end())
			v = it->second;
	}

	list<pair<int, int>> tmp1, tmp2;
	list<pair<int, int>> p1, p2;

	prev = from;
	for (int v : path.path) {
		tmp1.emplace_back(prev, v);
		prev = v;
	}
	tmp1.emplace_back(prev, to);

	prev = from;
	for (int v : path2.path) {
		tmp2.emplace_back(prev, v);
		prev = v;
	}
	tmp2.emplace_back(prev, to);
#ifdef DEBUG
	cout << "Tmp1:" << endl;
	for (auto &e : tmp1)
		cout << "<" << e.first << ", " << e.second << ">; ";
	cout << endl;

	cout << "Tmp2:" << endl;
	for (auto &e : tmp2)
		cout << "<" << e.first << ", " << e.second << ">; ";
	cout << endl;
#endif
	untanglePaths(tmp1, tmp2, p1);
	untanglePaths(tmp2, tmp1, p2);

	double len = 0;

	for (auto &e : p1) {
		cout << e.first << " ";
		len += srcGraph.edges[e.first][e.second];
	}
	cout << to << " (" << len << ")" << endl;

	len = 0;

	for (auto &e : p2) {
		cout << e.first << " ";
		len += srcGraph.edges[e.first][e.second];
	}
	cout << to << " (" << len << ")" << endl;

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
