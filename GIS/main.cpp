#include <iostream>
#include "BaseGraph.h"
#include "FibHeap.h"

using namespace std;

vector<int> dijkstra(BaseGraph &g, int current) {
	FibHeap<int, int> heap;
	vector<FibHeap<int, int>::TreeNode*> verticles;
	vector<bool> visited;
	vector<int> distArray;
	verticles.resize(g.verticesCount());
	visited.resize(g.verticesCount());
	distArray.resize(g.verticesCount());

	for (int i=0; i<g.verticesCount(); i++) {
		verticles[i] = heap.insert(i, numeric_limits<int>::max());
		distArray[i] = numeric_limits<int>::max();
		visited[i] = false;
	}
	distArray[current] = 0;
	heap.decreaseKey(verticles[current], 0);

	while (true) {
		for (auto &edge: g.edges[current]) {
			int neighbor = edge.dest;
			int tentativeDist = distArray[current] + edge.weight;
			if (!visited[neighbor] && tentativeDist < distArray[neighbor]) {
				distArray[neighbor] = tentativeDist;
				heap.decreaseKey(verticles[neighbor], tentativeDist);
			}
		}

		visited[current] = true;
		// In order to remove a particular item from heap, decrease it's key
		// to minimum and remove minimum object from heap.
		heap.decreaseKey(verticles[current], numeric_limits<int>::min());
		heap.removeMin();

		if (!heap.empty()) {
			current = heap.min().value();
		} else {
			break;
		}
	}

	return distArray;
}

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

	// Perform algorithm
	vector<int> distances = dijkstra(graph, 0);

	// Print results
	for (int i = 0; i < distances.size(); i++) {
		cout << i << ": " << distances[i] << endl;
	}

	getchar();
	return 0;
}