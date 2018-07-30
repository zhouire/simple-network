#include "StdAfx.h"
#include "ClientGame.h"

#include <vector>


std::vector<std::string> strDataBase{ "hello", "my", "name" };
std::string strPiece;
int iter = 0;
Model * mPiece = new Model();

//Model * clientModel = new Model();



ClientGame::ClientGame(void)
{
	
    network = new ClientNetwork();
	m = new ClientGameMinor(network);

	clientModel = new Model();
	clientModel2 = new Model2;

	clientModel2->C = 'q';
	clientModel2->I = 0;
	Part clientPart;
	clientPart.N = 0;
	(clientModel2->P) = clientPart;

	//set console exit actions
	//SetConsoleCtrlHandler(ClientExitRoutine, true);

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
	m->sendGenericPacket();
}

/*
void ClientGame::sendStringPackets(Model * m) {

	Packet strPacket;
	strPacket.packet_type = STRING_PACKET;
	strPacket.m = m;

	const unsigned int packet_size = sizeof(strPacket);
	//IZ: This creates an appropriately-sized char array to later fill with the converted pointer
	char packet_data[packet_size];

	strPacket.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}
*/

/*
void ClientGame::addToModelString(std::string * s, int i) {
	Packet packet;
	packet.packet_type = STRING_APPEND;
	packet.s = s;
	//packet.i = i;

	const unsigned int packet_size = sizeof(packet);
	char packet_data[packet_size];

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}
*/

void ClientGame::addToModel2Int(int i) {
	Packet packet;
	packet.packet_type = MODEL2_ADD;
	packet.i = i;

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

void ClientGame::sendFloat(float f)
{
	Packet packet;
	packet.packet_type = FLOAT_PACKET;
	packet.f = f;

	const unsigned int packet_size = sizeof(packet);
	char packet_data[packet_size];

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::sendExitPacket()
{
	Packet packet;
	packet.packet_type = CLIENT_EXIT;

	const unsigned int packet_size = sizeof(packet);
	char packet_data[packet_size];

	packet.serialize(packet_data);

	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::update()
{
    Packet packet;

    int data_length = network->receivePackets(network_data);

	//mPiece->V = std::vector<int>{ 1,2,3 };

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

		//mPiece->S = strPiece;
		//mPiece->V = std::vector<int>{ 1,2,3 };

		
		packet.deserialize(&(network_data[i]));
        i += sizeof(Packet);

        switch (packet.packet_type) {

            case ACTION_EVENT:

                printf("client received action event packet from server\n");

                //sendActionPackets();
				//added
				//sendStringPackets(mPiece);

				sendFloat(iter + 0.5f);

                break;

			case STRING_PACKET:

				printf("client got stringpacket\n");

				//sendStringPackets(mPiece);
				addToModelVector(iter);

				break;
				/*
			case MODEL_UPDATE:
				//*clientModel = *(packet.m);
				clientModel->i = packet.m->i;
				clientModel->S = packet.m->S;
				clientModel->V = packet.m->V;

				//printf("%s \n", (clientModel->S).c_str());
				printf("%i \n", (clientModel->i));
				//printf("%i \n", (2));
				break;
				*/

			case MODEL2_UPDATE:
			{
				clientModel2->C = (packet.m2).C;
				clientModel2->I = (packet.m2).I;
				clientModel2->P = (packet.m2).P;

				printf("Model2 updated with char %c, int %i, Part %i \n", clientModel2->C, clientModel2->I, (clientModel2->P).N);

				break;
			}

            default:

                printf("error in packet types\n");

                break;
        }
    }
}

void ClientGame::updateKeyPress()
{
	//std::string * s = new std::string;

	static bool a_press = false;
	static bool s_press = false;
	static bool d_press = false;

	if (GetKeyState('A') & 0x8000/*check if high-order bit is set (1 << 15)*/)
	{
		if (!a_press) {
			//s->assign("after ");
			//addToModelString(s, 3);
			
			//sendActionPackets();
			
			addToModel2Int(1);

			a_press = true;
		}
	}

	else {
		a_press = false;
	}
	
	/*
	if (GetKeyState('S') & 0x8000)
	{
		if (!s_press) {
			s->assign("string ");
			//*s = "string ";
			addToModelString(s);
			s_press = true;
		}
	}

	else {
		s_press = false;
	}


	if (GetKeyState('D') & 0x8000)
	{
		if (!d_press) {
			s->assign("data ");
			//*s = "data ";
			addToModelString(s);
			d_press = true;
		}
	}

	else {
		d_press = false;
	}
	*/
}



BOOL WINAPI ClientGame::ClientExitRoutine(_In_ DWORD dwCtrlType) {
	sendExitPacket();

	return false;
}
