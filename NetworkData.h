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

};


struct Packet {

    unsigned int packet_type;
	int i;
	float f;
	//std::string * s;
	//Model * m;

	//std::string
	char c;

	Model2 m2;


    void serialize(char * data) {
        memcpy(data, this, sizeof(Packet));
    }

    void deserialize(char * data) {
        memcpy(this, data, sizeof(Packet));
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