#include "StdAfx.h"
#include "ClientGame.h"

#include <vector>

std::vector<std::string> strDataBase{ "hello", "my", "name" };
std::string strPiece;
int iter = 0;
Model * mPiece = new Model();

Model * clientModel = new Model();

ClientGame::ClientGame(void)
{
	
    network = new ClientNetwork();

	

    // send init packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = INIT_CONNECTION;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}


ClientGame::~ClientGame(void)
{
}

void ClientGame::sendActionPackets()
{
    // send action packet
    
	const unsigned int packet_size = sizeof(Packet);
	//IZ: This creates an appropriately-sized char array to later fill with the converted pointer
    char packet_data[packet_size];
	
    Packet packet;
    packet.packet_type = ACTION_EVENT;

	//IZ: I think this actively changes packet_data from source
    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::sendStringPackets(Model * m) {
	/*
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];
	*/

	Packet strPacket;
	strPacket.packet_type = STRING_PACKET;
	strPacket.m = m;

	const unsigned int packet_size = sizeof(strPacket);
	//IZ: This creates an appropriately-sized char array to later fill with the converted pointer
	char packet_data[packet_size];

	strPacket.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::addToModelString(std::string * s) {
	Packet packet;
	packet.packet_type = STRING_APPEND;
	packet.s = s;

	const unsigned int packet_size = sizeof(packet);
	char packet_data[packet_size];

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

//IZ: tester function to add an int to the end of the V vector of a central Model object
void ClientGame::addToModelVector(int i)
{
	Packet packet;
	packet.packet_type = VECTOR_ADDITION;
	packet.i = i;

	const unsigned int packet_size = sizeof(packet);
	char packet_data[packet_size];

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::update()
{
    Packet packet;

    int data_length = network->receivePackets(network_data);

    if (data_length <= 0) 
    {
        //no data recieved
        return;
    }

    int i = 0;
    while (i < (unsigned int)data_length) 
    {
		//added this part for testing
		iter++;
		int it = iter % 3;
		strPiece = strDataBase[it];

		mPiece->S = strPiece;
		mPiece->V = std::vector<int>{1,2,3};

		
		packet.deserialize(&(network_data[i]));
        i += sizeof(Packet);

        switch (packet.packet_type) {

            case ACTION_EVENT:

                printf("client received action event packet from server\n");

                //sendActionPackets();
				//added
				//sendStringPackets(mPiece);

                break;

			case STRING_PACKET:

				printf("client got stringpacket\n");

				//sendStringPackets(mPiece);
				addToModelVector(iter);

				break;

			case MODEL_UPDATE:
				clientModel = packet.m;

				printf("%s \n", (clientModel->S).c_str());

				break;

            default:

                printf("error in packet types\n");

                break;
        }
    }
}

void ClientGame::updateKeyPress()
{
	std::string * s = new std::string();

	if (GetKeyState('A') & 0x8000/*check if high-order bit is set (1 << 15)*/)
	{
		s->assign("after ");
		//*s = "after ";
		addToModelString(s);
	}

	else if (GetKeyState('S') & 0x8000/*check if high-order bit is set (1 << 15)*/)
	{
		s->assign("string ");
		//*s = "string ";
		addToModelString(s);
	}

	else if (GetKeyState('D') & 0x8000/*check if high-order bit is set (1 << 15)*/)
	{
		s->assign("data ");
		//*s = "data ";
		addToModelString(s);
	}
}
