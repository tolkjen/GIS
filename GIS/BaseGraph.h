#pragma once

#include <vector>
#include <map>
#include <cassert>
#include <iostream>

using namespace std;

class BaseGraph {
public:

	class InputFileAccessException: public exception {
		virtual const char* what() const throw() {
			return "Can't read input file";
		}
	};

	class InputFileFormatException: public exception {
		virtual const char* what() const throw() {
			return "Input file is not in the specified format";
		}
	};

	BaseGraph(void);
	int verticesCount(void);
	void loadFromFile(char *fname);

	int addVertex(void)
	{
		edges.emplace_back();
		edgesTo.emplace_back();
		return edges.size() - 1;
	}

	void removeVertex(int v)
	{
		assert((unsigned int)v == edges.size() - 1);

		for (auto &e : edgesTo[v])
			edges[e.first].erase(v);

		for (auto &e : edges[v])
			edgesTo[e.first].erase(v);

		edges.pop_back();
	}

	void mergeVertices(int a, int b)
	{
		for (auto &e : edges[b]) {
			edges[a][e.first] = e.second;
			edgesTo[e.first][a] = e.second;
		}

		for (auto &e : edgesTo[b]) {
			edgesTo[a][e.first] = e.second;
			edges[e.first][a] = e.second;
		}

		removeVertex(b);
	}

	void reverseEdge(int a, int b)
	{
		edges[b][a] = -edges[a][b];
		edgesTo[a][b] = -edges[a][b];

		edges[a].erase(b);
		edgesTo[b].erase(a);
	}

	int splitVertex(int v)
	{
		int v2 = addVertex();

		edges[v2] = edges[v];
		edgesTo[v2] = edgesTo[v];

		for (auto &e : edges[v])
			edgesTo[e.first][v2] = e.second;

		for (auto &e : edgesTo[v])
			edges[e.first][v2] = e.second;

		return v2;
	}

	void addEdge(int from, int to, double weight)
	{
		edges[from][to] = weight;
		edgesTo[to][from] = weight;
	}

	void removeEdge(int from, int to)
	{
		edges[from].erase(to);
		edges[to].erase(from);
	}

	void print(ostream &out)
	{
		out << "digraph G {" << endl;
		int i = 0;
		for (auto &from : edges) {
			for (auto &to : from)
				out << "\t" << i << " -> "
					<< to.first << " [label=\""
					<< to.second << "\"];" << endl;
			++i;
		}
		out << "}" << endl;
	}

	vector<map<int, double>> edges;
	vector<map<int, double>> edgesTo;

private:
	int _verticesCount, _edgesCount;
};
