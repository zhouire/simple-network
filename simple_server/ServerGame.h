#pragma once
#include "ServerNetwork.h"
#include "NetworkData.h"

class ServerGame
{

public:

    ServerGame(void);
    ~ServerGame(void);

    void update();

	void receiveFromClients();

	void sendActionPackets();

	void sendStringPackets();

	void sendModelUpdate();

	void sendModel2Update();

	//char * serializeToChar(Packet);
	std::string serializeToChar(Packet);

	Packet deserializeToPacket(char *, int);

	void sendSizeData(int);

private:

   // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

	Model * centralModel;
	Model2 * centralModel2;

   // The ServerNetwork object 
    ServerNetwork* network;

	// data buffer
   char network_data[MAX_PACKET_SIZE];
};