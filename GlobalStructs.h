#pragma once

#include <string>
#include <vector>

struct Part {
	int N;
};

struct Model {
	Model()
	{}

	Part * P;
	std::string * S;
	std::vector<int> V;
};

struct Model2 {
	Part P;
	char C;
	int I;
};

/*
struct Scene {
Model * M;
int N;
};
*/

