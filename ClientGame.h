#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
#include "NetworkData.h"

class ClientGame
{
public:
	ClientGame(void);
	~ClientGame(void);

	ClientNetwork* network;

	void sendActionPackets();

	void sendStringPackets(Model *);

	void addToModelString(std::string *);

	void addToModelVector(int);

    char network_data[MAX_PACKET_SIZE];

    void update();

	void updateKeyPress();
};

