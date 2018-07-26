#pragma once

#include <string>
#include <vector>

struct Model {
	Model()
	{}

	std::string S;
	std::vector<int> V;
};

struct Scene {
	Model * M;
	int N;
};