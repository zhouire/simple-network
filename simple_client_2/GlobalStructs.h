#pragma once

#include <string>
#include <vector>

struct Part {
	friend class boost::serialization::access;

	Part() {}

	int N;

//private:
	//friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar & N;
	}
};

struct Model {
	friend class boost::serialization::access;

	Part * P;
	//Part * P = new Part();
	//std::string * S;
	std::string S;
	std::vector<int> V;

	Model()
	{
		//P = new Part();
		P = nullptr;
	}

//private:
	//friend class boost::serialization::access;
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		//ar & P & S & V;
		ar & P;
		ar & S;
		ar & V;
	}
};


struct Model2 {
	friend class boost::serialization::access;

	Model2() {}

	Part P;
	char C;
	int I;

	//friend class boost::serialization::access;
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		//ar & P & C & I;
		ar & P;
		ar & C;
		ar & I;
	}
};

/*
struct Scene {
	Model * M;
	int N;
};
*/
