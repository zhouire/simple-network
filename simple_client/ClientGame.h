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

	bool curPacket;
	//char tempBuf[MAX_PACKET_SIZE];
	std::string tempBuf;
	int nextDataSize;


	void sendActionPackets();

	void sendStringPackets(Model *);

	//void addToModelString(std::string, int);

	void addToModel2Int(int);

	void addToModelVector(int);

	void sendFloat(float);

	void addToModelPart(int);

	void changeModelString(std::string);

	void modifyModelQuat(int, int, int, int);

	void sendExitPacket();

    char network_data[MAX_PACKET_SIZE];

    void update();

	void updateKeyPress();

	BOOL WINAPI ClientExitRoutine(_In_ DWORD);

	//char * serializeToChar(Packet);

	//std::string serializeToChar(Packet);
	std::string serializeToChar(Packet&);
	
	Packet deserializeToPacket(const char *, int);

	void sendSizeData(int);
};

