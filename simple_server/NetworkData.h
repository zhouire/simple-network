#pragma once
#include <string.h>
#include <string>

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

};

//this is a small segment sent before each Packet, defining the Packet's size for deserialization
struct Size {
	int size;

	void serialize(char * data) {
		memcpy(data, this, sizeof(Size));
	}

	void deserialize(char * data) {
		memcpy(this, data, sizeof(Size));
	}
};


struct Packet {

	Packet() {}

	unsigned int packet_type;
	int i;
	float f;
	//std::string * s;
	//Model * m;
	Model m;

	std::string s;
	char c;

	Model2 m2;

	/*
	void serialize(char * data) {
		memcpy(data, this, sizeof(Packet));
	}

	void deserialize(char * data) {
		memcpy(this, data, sizeof(Packet));
	}
	*/

//private:
	friend class boost::serialization::access;
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