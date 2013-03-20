#pragma once

#include <vector>

using namespace std;

class BaseGraph {
public:

	class Edge {
	public:
		Edge(int d, double w);
		int dest;
		double weight;
	};

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

	vector<vector<Edge>> edges;

private:
	int _verticesCount, _edgesCount;
};

