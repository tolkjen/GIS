#include <vector>
#include "FibHeap.h"
#include "GraphOperations.h"

namespace GraphOperations {

	using namespace std;

	SearchResult::SearchResult(vector<int> p, double l) :
		path(p), length(l)
	{
	}

	SearchResult dijkstra(BaseGraph &g, int vertexFrom, int vertexTo) {
		// If vertexTo equals vertexFrom, return sequence containing only one
		// vertex.
		if (vertexFrom == vertexTo) {
			return SearchResult(vector<int>(1, vertexFrom), 0.0);
		}

		FibHeap<int, double> heap;
		vector<FibHeap<int, double>::TreeNode*> verticles;
		vector<bool> visited;
		vector<double> distArray;
		vector<int> previousVertexArray;
		verticles.resize(g.verticesCount());
		visited.resize(g.verticesCount());
		distArray.resize(g.verticesCount());
		previousVertexArray.resize(g.verticesCount());
	
		// Fill structures with default values
		for (int i=0; i<g.verticesCount(); i++) {
			verticles[i] = NULL;
			distArray[i] = numeric_limits<double>::max();
			visited[i] = false;
		}

		int current = vertexFrom;
		distArray[current] = 0;
		previousVertexArray[current] = -1;
		verticles[current] = heap.insert(current, 0);

		while (true) {
			for (auto &edge: g.edges[current]) {
				int neighbor = edge.dest;
				double tentativeDist = distArray[current] + edge.weight;
				if (!visited[neighbor] && tentativeDist < distArray[neighbor]) {
					distArray[neighbor] = tentativeDist;
					previousVertexArray[neighbor] = current;
					if (verticles[neighbor] == NULL) {
						verticles[neighbor] = heap.insert(neighbor, distArray[neighbor]);
					} else {
						heap.decreaseKey(verticles[neighbor], tentativeDist);
					}
				}
			}

			visited[current] = true;
			// In order to remove a particular item from heap, decrease it's key
			// to minimum and remove minimum object from heap.
			heap.decreaseKey(verticles[current], numeric_limits<double>::min());
			heap.removeMin();

			if (!heap.empty()) {
				current = heap.min().value();
				if (current == vertexTo) {
					break;
				}
			} else {
				break;
			}
		}

		// If current doesn't equal vertexTo, algorithm wasn't able to create 
		// path from vertexFrom to vertexTo. Return empty sequence.
		if (current != vertexTo) {
			return SearchResult(vector<int>(), numeric_limits<double>::max());
		}

		// If path exists, get path vertices by backtracking from vertexTo to 
		// vertexFrom using previousVertexArray. Reverse it to get order from
		// vertexFrom to VertexTo.
		vector<int> shortestPath;
		do {
			shortestPath.push_back(current);
			current = previousVertexArray[current];
		} while (current != -1);
		std::reverse(shortestPath.begin(), shortestPath.end());
		return SearchResult(shortestPath, distArray[vertexTo]);
	}

	SearchResult dijkstra_modified(BaseGraph &g, int vertexFrom, int vertexTo) {
		// If vertexTo equals vertexFrom, return sequence containing only one
		// vertex.
		if (vertexFrom == vertexTo) {
			return SearchResult(vector<int>(1, vertexFrom), 0.0);
		}

		FibHeap<int, double> heap;
		vector<FibHeap<int, double>::TreeNode*> verticles;
		vector<double> distArray;
		vector<int> previousVertexArray;
		verticles.resize(g.verticesCount());
		distArray.resize(g.verticesCount());
		previousVertexArray.resize(g.verticesCount());
	
		// Fill structures with default values
		for (int i=0; i<g.verticesCount(); i++) {
			verticles[i] = NULL;
			distArray[i] = numeric_limits<double>::max();
		}

		int current = vertexFrom;
		distArray[current] = 0;
		previousVertexArray[current] = -1;
		verticles[current] = heap.insert(current, 0);

		while (true) {
			for (auto &edge: g.edges[current]) {
				int neighbor = edge.dest;
				double tentativeDist = distArray[current] + edge.weight;
				if (tentativeDist < distArray[neighbor]) {
					distArray[neighbor] = tentativeDist;
					previousVertexArray[neighbor] = current;
					if (verticles[neighbor] == NULL) {
						verticles[neighbor] = heap.insert(neighbor, distArray[neighbor]);
					} else {
						heap.decreaseKey(verticles[neighbor], tentativeDist);
					}
				}
			}

			// In order to remove a particular item from heap, decrease it's key
			// to minimum and remove minimum object from heap.
			heap.decreaseKey(verticles[current], numeric_limits<double>::min());
			heap.removeMin();
			verticles[current] = NULL;

			if (!heap.empty()) {
				current = heap.min().value();
				if (current == vertexTo) {
					break;
				}
			} else {
				break;
			}
		}

		// If current doesn't equal vertexTo, algorithm wasn't able to create 
		// path from vertexFrom to vertexTo. Return empty sequence.
		if (current != vertexTo) {
			return SearchResult(vector<int>(), numeric_limits<double>::max());
		}

		// If path exists, get path vertices by backtracking from vertexTo to 
		// vertexFrom using previousVertexArray. Reverse it to get order from
		// vertexFrom to VertexTo.
		vector<int> shortestPath;
		do {
			shortestPath.push_back(current);
			current = previousVertexArray[current];
		} while (current != -1);
		std::reverse(shortestPath.begin(), shortestPath.end());
		return SearchResult(shortestPath, distArray[vertexTo]);
	}
}
