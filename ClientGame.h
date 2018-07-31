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
	Model * clientModel;

	Model2 * clientModel2;

	void sendActionPackets();

	void sendStringPackets(Model *);

	//void addToModelString(std::string, int);

	void addToModel2Int(int);

	void addToModelVector(int);

	void sendFloat(float);

	void addToModelPart(int);

	void changeModelString(std::string);

	void sendExitPacket();

    char network_data[MAX_PACKET_SIZE];

    void update();

	void updateKeyPress();

	BOOL WINAPI ClientExitRoutine(_In_ DWORD);

	char * serializeToChar(Packet);
	
	Packet deserializeToPacket(char *, int);
};

