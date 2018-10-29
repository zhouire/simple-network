#pragma once
#include <string.h>
#include <string>

//#define MAX_PACKET_SIZE 10000000
#define MAX_PACKET_SIZE 1000000

enum PacketTypes {

	INIT_CONNECTION = 0,

	ACTION_EVENT = 1,

	STRING_PACKET = 2,

	VECTOR_ADDITION = 3,

	STRING_APPEND = 4,

	MODEL_UPDATE = 5,

	FLOAT_PACKET = 6,

	CLIENT_EXIT = 7,

	MODEL2_ADD = 8,

	MODEL2_UPDATE = 9,

	ADD_TO_MODEL_PART = 10,

	CHANGE_MODEL_STRING = 11,

	MODIFY_MODEL_QUAT = 12,

};

//this is a small segment sent before each Packet, defining the Packet's size for deserialization
struct Size {
	Size() {}

	int size;

	void serialize(char * data) {
		memcpy(data, this, sizeof(Size));
	}

	void deserialize(char * data) {
		memcpy(this, data, sizeof(Size));
	}
};


struct Packet {
	friend class boost::serialization::access;

	unsigned int packet_type;
	int i;
	float f;
	//std::string * s;
	//Model * m;
	Model m;
	glm::quat q;
	//glm::vec3 vec;
	OVR::Vector3f OVRvec;
	DWORD u;
	OVR::Matrix4f Mat4;
	Part p;

	std::string s;
	char c;

	Model2 m2;

	Packet() :
		m(OVR::Vector3f(0, 0, 0))
	{}

	/*
	void serialize(char * data) {
	memcpy(data, this, sizeof(Packet));
	}

	void deserialize(char * data) {
	memcpy(this, data, sizeof(Packet));
	}
	*/

	//private:
	//friend class boost::serialization::access;

	// When the class Archive corresponds to an output archive, the
	// & operator is defined similar to <<.  Likewise, when the class Archive
	// is a type of input archive the & operator is defined similar to >>.
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		//ar & packet_type & i & f & m & s & c & m2;
		ar & packet_type;
		ar & i;
		ar & f;
		ar & m;
		ar & s;
		ar & c;
		ar & m2;
		ar & q;
		//ar & vec;
		ar & OVRvec;
		ar & u;
		ar & Mat4;
		ar & p;
	}

};

/*
struct StringPacket{

unsigned int packet_type;
std::string str;

void serialize(char * data) {
memcpy(data, this, sizeof(StringPacket));
}

void deserialize(char * data) {
memcpy(this, data, sizeof(StringPacket));
}
};
*/