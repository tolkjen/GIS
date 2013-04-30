#ifndef __GRAPH_OPERATIONS_H
#define __GRAPH_OPERATIONS_H

#include "BaseGraph.h"
#include <list>

namespace GraphOperations {

//  -------------------------------------------------------------------------
//  class SearchResult
//
//  Stores information about found path - vertices and path length.
	class SearchResult {
	public:
		SearchResult(const list<int> &p, double l);
		SearchResult(double l = 0) :
			length(l) {}

		std::list<int> path;
		double length;
	};

//  --------------------------------------------------------------------------
//  dijkstra
//
//  Returns the shortest path info from vertexFrom to vertexTo using original
//  Dijkstra algorithm. Works well for weights greater or equal zero. Doesn't 
//  work for weights less than zero. If path doesn't exist, returns empty
//  vector.
	SearchResult dijkstra(BaseGraph &g, int vertexFrom, int vertexTo);

//  --------------------------------------------------------------------------
//  dijkstra_modified
//
//  Returns the shortest path info between vertexFrom and vertexTo. Works for any
//  kind of weight on edge, but slower than original dijkstra. If path doesn't
//  exist, returns empty vector.
	SearchResult dijkstra_modified(BaseGraph &g, int vertexFrom, int vertexTo);

}

#endif /* __GRAPH_OPERATIONS_H */
