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

	vector<map<int, double>> edges;
	vector<map<int, double>> edgesTo;

private:
	int _verticesCount, _edgesCount;
};
