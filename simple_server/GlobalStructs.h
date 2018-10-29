#pragma once

#include <string>
#include <vector>

/*
namespace boost {
	namespace serialization {

		template<class Archive>
		void serialize(Archive & ar, glm::detail::tquat<float> & g, const unsigned int version)
		{
			ar & g.x;
			ar & g.y;
			ar & g.z;
			ar & g.w;
		}

	} // namespace serialization
} // namespace boost
*/
namespace boost {
	namespace serialization {
		
		template<class Archive>
		void serialize(Archive & ar, glm::quat & g, const unsigned int version)
		{
			ar & g.x;
			ar & g.y;
			ar & g.z;
			ar & g.w;
		}
		

		template<class Archive>
		void serialize(Archive & ar, glm::vec3 & vec, const unsigned int version)
		{
			ar & boost::serialization::make_nvp("x", vec.x);
			ar & boost::serialization::make_nvp("y", vec.y);
			ar & boost::serialization::make_nvp("z", vec.z);
		}

		template<class Archive>
		void serialize(Archive & ar, OVR::Vector3f & v, const unsigned int version)
		{
			ar & v.x;
			ar & v.y;
			ar & v.z;
		}

		template<class Archive>
		void serialize(Archive & ar, OVR::Matrix4f & m, const unsigned int version)
		{
			ar & m.M;
		}

	} // namespace serialization
} // namespace boost


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
	glm::quat Q;
	glm::vec3 vec;
	OVR::Vector3f OVRvec;
	DWORD d;
	OVR::Matrix4f Mat4;

	Model(OVR::Vector3f v) :
		OVRvec(v)
	{
		//P = new Part();
		P = nullptr;
	}

	void append(std::string s) {
		S.append(s);
	}

//private:
	//friend class boost::serialization::access;
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		//ar & P & S & V;
		append("hi");

		ar & P;
		ar & S;
		//ar & V;
		ar & Q;
		ar & vec;
		ar & OVRvec;
		ar & d;
		ar & Mat4;
	}
};



struct Model3 {
	friend class boost::serialization::access;

	Part * P;
	//Part * P = new Part();
	//std::string * S;
	std::string S;
	std::vector<int> V;
	glm::quat Q;
	//glm::vec3 vec;
	glm::vec3 OVRvec;
	//OVR::Vector3f OVRvec;
	DWORD d;
	OVR::Matrix4f Mat4;

	Model3(glm::vec3 v) :
		OVRvec(v)
	{
		//P = new Part();
		P = nullptr;
	}

	
	void append(std::string s) {
		//S.append(s);
		S.insert(0, s);
	}

	//private:
	//friend class boost::serialization::access;
	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		//ar & P & S & V;
		append("hi");

		ar & P;
		ar & S;
		//ar & V;
		ar & Q;
		//ar & vec;
		ar & OVRvec;
		ar & d;
		ar & Mat4;
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

