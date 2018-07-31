#pragma once

#include <string>
#include <vector>

struct Part {
	Part() {}

	int N;

//private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & N;
	}
};

struct Model {
	Model()
	{}

	Part * P;
	std::string * S;
	std::vector<int> V;

//private:
	friend class boost::serialization::access;
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & P & S & V;
	}
};

struct Model2 {
	Part P;
	char C;
	int I;

	friend class boost::serialization::access;
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & P & C & I;
	}
};

/*
struct Scene {
	Model * M;
	int N;
};
*/

