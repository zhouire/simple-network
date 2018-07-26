#pragma once
//#include <winsock2.h>
//#include <Windows.h>
#include "ClientNetwork.h"
#include "NetworkData.h"

#include "ClientGameMinor.h"

class ClientGame
{
public:
	ClientGame(void);
	~ClientGame(void);

	ClientNetwork* network;
	ClientGameMinor* m;

	void sendActionPackets();

	void sendStringPackets(Model *);

	void addToModelString(std::string *);

	void addToModelVector(int);

	void sendFloat(float);

    char network_data[MAX_PACKET_SIZE];

    void update();

	void updateKeyPress();
};

