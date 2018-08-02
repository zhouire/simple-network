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

	//if the current received data should be a packet (first one is a Size object)
	curPacket = new bool;
	curPacket = false;
	nextDataSize = sizeof(Size);

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
    //const unsigned int packet_size = sizeof(Packet);
    //char packet_data[packet_size];

    Packet packet;
    packet.packet_type = INIT_CONNECTION;

    //packet.serialize(packet_data);
	//char * packet_data = serializeToChar(packet);

	std::string buffer = serializeToChar(packet);
	char * packet_data = (char*)(buffer.data());
	const unsigned int packet_size = buffer.size() + 1;
	//const unsigned int packet_size = sizeof(buffer);

	sendSizeData(packet_size);
    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}


ClientGame::~ClientGame(void)
{
}

std::string ClientGame::serializeToChar(Packet packet)
{
	// serialize obj into an std::string
	std::string serial_str;
	boost::iostreams::back_insert_device<std::string> inserter(serial_str);
	boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
	boost::archive::binary_oarchive oa(s);

	//oa << packet; //serializable object
	oa & packet;

	// don't forget to flush the stream to finish writing into the buffer
	s.flush();

	//return (char*)(serial_str.data());
	return serial_str;
}

Packet ClientGame::deserializeToPacket(char * buffer, int buflen)
{
	Packet packet;
	// wrap buffer inside a stream and deserialize serial_str into obj
	boost::iostreams::basic_array_source<char> device(buffer, buflen);
	boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
	boost::archive::binary_iarchive ia(s);
	//ia >> packet;
	ia & packet;

	return packet;
}

void ClientGame::sendSizeData(int packet_size) {
	Size s;
	s.size = packet_size;

	const unsigned int s_size = sizeof(Size);
	char s_data[s_size];

	s.serialize(s_data);

	NetworkServices::sendMessage(network->ConnectSocket, s_data, s_size);
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

	std::string buffer = serializeToChar(packet);
	char * packet_data = (char*)(buffer.data());
	//const unsigned int packet_size = sizeof(buffer);
	const unsigned int packet_size = buffer.size() + 1;

	sendSizeData(packet_size);
	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

/*
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
*/

void ClientGame::sendFloat(float f)
{
	Packet packet;
	packet.packet_type = FLOAT_PACKET;
	packet.f = f;

	std::string buffer = serializeToChar(packet);
	char * packet_data = (char*)(buffer.data());
	const unsigned int packet_size = buffer.size() + 1;
	//const unsigned int packet_size = sizeof(buffer);

	sendSizeData(packet_size);
	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}


void ClientGame::addToModelPart(int i)
{
	Packet packet;
	packet.packet_type = ADD_TO_MODEL_PART;
	packet.i = i;

	std::string buffer = serializeToChar(packet);
	char * packet_data = (char*)(buffer.data());
	const unsigned int packet_size = buffer.size() + 1;
	//const unsigned int packet_size = sizeof(buffer);

	sendSizeData(packet_size);
	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}


void ClientGame::changeModelString(std::string str)
{
	Packet packet;
	packet.packet_type = CHANGE_MODEL_STRING;
	packet.s = str;

	std::string buffer = serializeToChar(packet);
	char * packet_data = (char*)(buffer.data());
	const unsigned int packet_size = buffer.size() + 1;
	//const unsigned int packet_size = sizeof(buffer);

	sendSizeData(packet_size);
	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}


void ClientGame::sendExitPacket()
{
	Packet packet;
	packet.packet_type = CLIENT_EXIT;

	std::string buffer = serializeToChar(packet);
	char * packet_data = (char*)(buffer.data());
	const unsigned int packet_size = buffer.size() + 1;
	//const unsigned int packet_size = sizeof(buffer);

	sendSizeData(packet_size);
	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::update()
{
    Packet packet;
	Size size;

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

		//this part deals with receiving data of varying sizes

		if (!curPacket) {
			//if the remaining data + data in tempBuf < enough data to construct Size object
			//just add the remaining data to the tempBuf, increment i, skip rest of loop
			if ((data_length - i - 1 + tempBuf.size()) < sizeof(Size)) {
				//just append the rest of the data, add to i, and skip everything else
				tempBuf.append(&(network_data[i]));
				//add enough to overflow the while condition
				i += sizeof(Size);
			}

			//if we have enough data in the network_data and tempBuf, append enough data to tempBuf to
			//construct a Size object, deserialize tempBuf into Size size, use value in size to update
			//nextDataSize, clear tempBuf (because we have already deserialized), increment i, flip the 
			//curPacket switch (because we constructed a full object), and skip rest of loop
			else{
				tempBuf.append(&(network_data[i]), (sizeof(Size) - tempBuf.size()));
				size.deserialize((char*)(tempBuf.data()));
				nextDataSize = size.size;
				curPacket = !curPacket;
				tempBuf.clear();
				i += (sizeof(Size) - tempBuf.size());
			}
			//no matter what, we skip the rest of the loop if the current data is of Size type
			continue;
		}

		else {
			//if the remaining data + data in tempBuf < enough data to construct the next Packet
			//add remaining data to tempBuf, increment i, skip rest of loop
			if ((data_length - i - 1 + tempBuf.size()) < nextDataSize) {
				tempBuf.append(&(network_data[i]));
				i += nextDataSize;
				continue;
			}

			//if we have enough data to construct the next full Packet, append enough data to tempBuf to
			//construct the Packet, deserialize tempBuf into Packet packet, clear tempBuf, flip the 
			//curPacket switch, increment i, and MOVE ON to rest of loop
			else {
				tempBuf.append(&(network_data[i]), (nextDataSize - tempBuf.size()));
				packet = deserializeToPacket((char*)(tempBuf.data()), nextDataSize);
				curPacket = !curPacket;
				tempBuf.clear();
				i += (nextDataSize - tempBuf.size());
			}
		}



		//packet.deserialize(&(network_data[i]));
		//packet = deserializeToPacket(&(network_data[i]), sizeof(Packet));
		
        //i += sizeof(Packet);
		//curPacket = !curPacket;

        switch (packet.packet_type) {

            case ACTION_EVENT:

                printf("client received action event packet from server\n");

                //sendActionPackets();
				//added
				//sendStringPackets(mPiece);

				sendFloat(iter + 0.5f);

                break;

				/*
			case STRING_PACKET:

				printf("client got stringpacket\n");

				//sendStringPackets(mPiece);
				addToModelVector(iter);

				break;
				*/

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

			case MODEL_UPDATE:
			{
				*clientModel = packet.m;

				printf("Model updated with Part %i, string %s \n", (clientModel->P)->N, (clientModel->S).c_str());

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
			
			//addToModel2Int(1);
			addToModelPart(1);
			changeModelString("after");


			a_press = true;
		}
	}

	else {
		a_press = false;
	}


	if (GetKeyState('S') & 0x8000/*check if high-order bit is set (1 << 15)*/)
	{
		if (!s_press) {
			//s->assign("after ");
			//addToModelString(s, 3);

			//sendActionPackets();

			//addToModel2Int(2);

			addToModelPart(2);
			changeModelString("super");

			s_press = true;
		}
	}

	else {
		s_press = false;
	}


	if (GetKeyState('D') & 0x8000/*check if high-order bit is set (1 << 15)*/)
	{
		if (!d_press) {
			//s->assign("after ");
			//addToModelString(s, 3);

			//sendActionPackets();

			//addToModel2Int(3);

			addToModelPart(3);
			changeModelString("durian");

			d_press = true;
		}
	}

	else {
		d_press = false;
	}
}



BOOL WINAPI ClientGame::ClientExitRoutine(_In_ DWORD dwCtrlType) {
	sendExitPacket();

	return false;
}
