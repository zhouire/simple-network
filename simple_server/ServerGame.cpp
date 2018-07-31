#include "StdAfx.h"
#include "ServerGame.h"


unsigned int ServerGame::client_id; 
//Model * centralModel = new Model();



ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;

	centralModel = new Model();
	centralModel2 = new Model2;

	centralModel2->C = 'q';
	centralModel2->I = 0;
	Part centralPart;
	centralPart.N = 0;
	(centralModel2->P) = centralPart;

	//initializes values of centralModel
	//centralModel->S = "hellouniverse";
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

			//packet.deserialize(&(network_data[i]));
			packet = deserializeToPacket(&(network_data[i]), sizeof(Packet));
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

					/*
				case STRING_PACKET:
				{
					int pt = packet.packet_type;
					std::string str = *(packet.str);
					Model * m = packet.m;
					std::string str = m->S;
					int v = m->V[pt];

					printf("heyoi %s %i \n", str.c_str(), v);

					sendActionPackets();

					break;
				}
				*/

				case VECTOR_ADDITION:
				{
					(centralModel->V).push_back(packet.i);
					std::vector<int> v = centralModel->V;
					printf("added num %i, %i \n\n", v[v.size() - 1], v.size());

					sendActionPackets();

					break;
				}
				/*
				case STRING_APPEND:
				{
					
					//centralModel->S = *(packet.s);
					//centralModel->i = packet.i;

					printf("server received client keystroke %i\n", centralModel->i);

					sendModelUpdate();

					break;
				}
				*/

				case FLOAT_PACKET:
				{
					printf("Server got float %f \n", packet.f);

					sendActionPackets();

					break;
				}


				case MODEL2_ADD:
				{

					(centralModel2->I) += packet.i;
					printf("Server got int %i, final value %i \n", packet.i, centralModel2->I);

					sendModel2Update();

					break;

				}

				case ADD_TO_MODEL_PART:
				{

					(centralModel->P)->N += packet.i;
					printf("Server received int %i, Model Part int %i \n", packet.i, (centralModel->P)->N);

					sendModelUpdate();

					break;

				}

				case CHANGE_MODEL_STRING:
				{

					*(centralModel->S) = packet.s;
					printf("Server received string %s, Model string now %s \n", (packet.s).c_str(), (centralModel->S)->c_str());

					sendModelUpdate();

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

/*
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
*/

char * ServerGame::serializeToChar(Packet packet) 
{
	// serialize obj into an std::string
	std::string serial_str;
	boost::iostreams::back_insert_device<std::string> inserter(serial_str);
	boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
	boost::archive::binary_oarchive oa(s);

	oa << packet; //serializable object

	// don't forget to flush the stream to finish writing into the buffer
	s.flush();

	return (char*)(serial_str.data());
}

Packet ServerGame::deserializeToPacket(char * buffer, int buflen)
{
	Packet packet;
	// wrap buffer inside a stream and deserialize serial_str into obj
	boost::iostreams::basic_array_source<char> device(buffer, buflen);
	boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
	boost::archive::binary_iarchive ia(s);
	ia >> packet;

	return packet;
}


void ServerGame::sendActionPackets()
{
	Packet packet;
	packet.packet_type = ACTION_EVENT;

	const unsigned int packet_size = sizeof(Packet);

	char * packet_data = serializeToChar(packet);

	network->sendToAll(packet_data, packet_size);
}


/*
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
*/

/*
void ServerGame::sendModelUpdate()
{
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = MODEL_UPDATE;
	packet.m = new Model();
	*(packet.m) = *centralModel;
	//packet.m = centralModel;

	packet.serialize(packet_data);

	network->sendToAll(packet_data, packet_size);
}
*/

void ServerGame::sendModelUpdate()
{
	Packet packet;
	packet.packet_type = MODEL_UPDATE;
	packet.m = *centralModel;

	const unsigned int packet_size = sizeof(Packet);

	// serialize obj into an std::string
	std::string serial_str;
	boost::iostreams::back_insert_device<std::string> inserter(serial_str);
	boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
	boost::archive::binary_oarchive oa(s);

	oa << packet; //serializable object

	// don't forget to flush the stream to finish writing into the buffer
	s.flush();

	// now you get to const char* with serial_str.data() or serial_str.c_str()
	char * packet_data = (char*) serial_str.data();

	network->sendToAll(packet_data, packet_size);
}

/*
void ServerGame::sendModel2Update()
{
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = MODEL2_UPDATE;
	packet.m2 = *centralModel2;
	//packet.m = centralModel;

	packet.serialize(packet_data);

	network->sendToAll(packet_data, packet_size);
}
*/

void ServerGame::sendModel2Update()
{
	const unsigned int packet_size = sizeof(Packet);
	//char packet_data[packet_size];

	Packet packet;
	packet.packet_type = MODEL2_UPDATE;
	packet.m2 = *centralModel2;

	//packet.serialize(packet_data);
	char * packet_data = serializeToChar(packet);

	network->sendToAll(packet_data, packet_size);
}


