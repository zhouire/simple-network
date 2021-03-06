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
	std::string serializeToChar(Packet*);

	Packet * deserializeToPacket(const char *, int);

	void sendSizeData(int);

private:

   // IDs for the clients connecting for table in ServerNetwork 
    static unsigned int client_id;

	Model3 * centralModel;
	Model2 * centralModel2;

   // The ServerNetwork object 
    ServerNetwork* network;

	// data buffer
   char network_data[MAX_PACKET_SIZE];

   //just for keeping track of data received from the stream
   bool * curPacket;
   std::vector<std::string> tempBuf;
   int * nextDataSize;
};