#include <cstdio>
#include <limits>
#include "BaseGraph.h"

BaseGraph::BaseGraph(void) : _verticesCount(0), _edgesCount(0)
{
}

void BaseGraph::loadFromFile( char *fname ) {
	FILE *fptr = fopen(fname, "r");
	if (fptr == NULL) {
		throw InputFileAccessException();
	}

	if (fscanf(fptr, "%d %d", &_verticesCount, &_edgesCount) != 2) {
		fclose(fptr);
		throw InputFileFormatException();
	}

	edges.clear();
	edges.resize(_verticesCount);

	int src, dest;
	double weight;
	for (int i=0; i<_edgesCount; i++) {
		if (fscanf(fptr, "%d %d %lf", &src, &dest, &weight) != 3) {
			fclose(fptr);
			throw InputFileFormatException();
		}
		edges[src][dest] = weight;
	}
}

int BaseGraph::verticesCount( void ) {
	return _verticesCount;
}
