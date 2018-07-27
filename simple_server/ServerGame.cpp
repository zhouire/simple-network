#include "StdAfx.h"
#include "ServerGame.h"


unsigned int ServerGame::client_id; 
Model * centralModel = new Model();


ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;

	//initializes values of centralModel
	centralModel->S = "hellouniverse";
	centralModel->V = std::vector<int>{ 0,1,2,3 };

    // set up the server network to listen 
    network = new ServerNetwork(); 
}

ServerGame::~ServerGame(void)
{
}

void ServerGame::update()
{
    // get new clients
   if(network->acceptNewClient(client_id))
   {
        printf("client %d has been connected to the server\n",client_id);

        client_id++;
   }

   receiveFromClients();
}

void ServerGame::receiveFromClients()
{

    //Packet packet;
	Packet packet;

    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;

    for(iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
    {
		int data_length = network->receiveData(iter->first, network_data);

        if (data_length <= 0) 
        {
            //no data recieved
            continue;
        }

        int i = 0;
        while (i < (unsigned int)data_length) 
        {
            /*packet.deserialize(&(network_data[i]));
            i += sizeof(Packet);*/

			packet.deserialize(&(network_data[i]));
			i += sizeof(Packet);

            switch (packet.packet_type) {

                case INIT_CONNECTION:

                    printf("server received init packet from client\n");

                    sendActionPackets();

                    break;

                case ACTION_EVENT:

                    printf("server received action event packet from client\n");
					//sendStringPackets();
					/*
					if (client_id % 2 == 1) {
						sendStringPackets();
					}
					else {
						sendActionPackets();
					}
					*/

					sendActionPackets();
					

                    break;

				case STRING_PACKET:
				{
					int pt = packet.packet_type;
					//std::string str = *(packet.str);
					Model * m = packet.m;
					std::string str = m->S;
					int v = m->V[pt];

					printf("heyoi %s %i \n", str.c_str(), v);

					sendActionPackets();

					break;
				}

				case VECTOR_ADDITION:
				{
					(centralModel->V).push_back(packet.i);
					std::vector<int> v = centralModel->V;
					printf("added num %i, %i \n\n", v[v.size() - 1], v.size());

					sendActionPackets();

					break;
				}

				case STRING_APPEND:
				{
					//std::string s = centralModel->S;
					//s.append(packet.s->c_str());

					//centralModel->S = s;
					centralModel->S = *(packet.s);

					printf("server received client keystroke\n");

					sendModelUpdate();

					break;
				}

				case FLOAT_PACKET:
				{
					printf("Server got float %f \n", packet.f);

					sendActionPackets();

					break;
				}

				case CLIENT_EXIT:
				{
					printf("Client %i is disconnecting. Closing the socket.", iter->first);
					
					closesocket(iter->second);
					WSACleanup();
					//exit(1);
					

					break;
				}

                default:

                    printf("error in packet types\n");

					printf("%i\n", (network->sessions).size());

                    break;
            }
        }
    }
}


void ServerGame::sendActionPackets()
{
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    network->sendToAll(packet_data,packet_size);
}


//fake string packet, just a tester
void ServerGame::sendStringPackets()
{
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = STRING_PACKET;

	packet.serialize(packet_data);

	network->sendToAll(packet_data, packet_size);
}


void ServerGame::sendModelUpdate()
{
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = MODEL_UPDATE;
	packet.m = centralModel;

	packet.serialize(packet_data);

	network->sendToAll(packet_data, packet_size);
}

