#include <iostream>
#include <string>
#include "BaseGraph.h"
#include "GraphOperations.h"

using namespace std;
using namespace GraphOperations;

// --------------------------------------------------------------------------
string pathToString(vector<int> &path) {
	if (path.empty()) {
		return string("None");
	}

	string result;
	for (int i = 0; i < path.size() - 1; i++) {
		result += to_string(path[i]) + " -> ";
	}
	return result + to_string(path[path.size() - 1]);
}

// --------------------------------------------------------------------------
int main(int argc, char *argv[]) {
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

	// perform algo
	SearchResult pathInfo = dijkstra_modified(graph, 0, graph.verticesCount() - 1);
	cout << pathToString(pathInfo.path).c_str() << endl;
	cout << "Length: " << pathInfo.length << endl;

	getchar();
	return 0;
}