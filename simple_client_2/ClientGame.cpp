#include "StdAfx.h"
#include "ClientGame.h"

#include <vector>


std::vector<std::string> strDataBase{ "hello", "my", "name" };
std::string strPiece;
int iter = 0;
//Model * mPiece = new Model();

//Model * clientModel = new Model();



ClientGame::ClientGame(void)
{
	
    network = new ClientNetwork();
	m = new ClientGameMinor(network);

	//if the current received data should be a packet (first one is a Size object)
	//curPacket = new bool;
	curPacket = false;
	nextDataSize = sizeof(Size);

	clientModel = new Model(OVR::Vector3f(0, 0, 0));
	clientModel2 = new Model2;

	clientModel2->C = 'q';
	clientModel2->I = 0;
	Part clientPart;
	clientPart.N = 0;
	(clientModel2->P) = clientPart;

	clientModel->P = new Part();
	*(clientModel->P) = clientPart;

	//set console exit actions
	//SetConsoleCtrlHandler(ClientExitRoutine, true);

    // send init packet
    //const unsigned int packet_size = sizeof(Packet);
    //char packet_data[packet_size];

    //Packet * packet = new Packet();
    //packet->packet_type = INIT_CONNECTION;
	Packet * packet = new Packet();
	packet->packet_type = INIT_CONNECTION;

    //packet.serialize(packet_data);
	//char * packet_data = serializeToChar(packet);

	std::string buffer = serializeToChar(packet);
	char * packet_data = (char*)(buffer.data());
	//const unsigned int packet_size = buffer.size() + 1;
	const unsigned int packet_size = buffer.size();
	//const unsigned int packet_size = sizeof(buffer);

	sendSizeData(packet_size);
    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}


ClientGame::~ClientGame(void)
{
}


std::string ClientGame::serializeToChar(Packet * packet)
{
	/*
	Model m;
	m.P = new Part();
	m.P->N = 3;
	m.S = "hello";
	m.V = std::vector<int>{ 1,2,3 };

	Packet pack;
	pack.m = m;
	pack.packet_type = ACTION_EVENT;
	*/

	// serialize obj into an std::string
	std::string serial_str;
	boost::iostreams::back_insert_device<std::string> inserter(serial_str);
	boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
	boost::archive::binary_oarchive oa(s);

	oa << packet; //serializable object
	//oa & packet;

	// don't forget to flush the stream to finish writing into the buffer
	s.flush();

	//printf("%i %i\n", serial_str.size(),serial_str.length());

	//return (char*)(serial_str.data());
	return serial_str;
}

Packet * ClientGame::deserializeToPacket(const char * buffer, int buflen)
{
	Packet * packet;
	// wrap buffer inside a stream and deserialize serial_str into obj
	boost::iostreams::basic_array_source<char> device(buffer, buflen);
	boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
	boost::archive::binary_iarchive ia(s);
	ia >> packet;
	//ia & packet;

	return packet;
}

void ClientGame::sendSizeData(int packet_size) {
	Size s;
	s.size = packet_size;

	const unsigned int s_size = sizeof(Size);
	char s_data[s_size];

	s.serialize(s_data);

	//printf("client serializing size\n");

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
	
	Packet * packet = new Packet();
	packet->packet_type = MODEL2_ADD;
	packet->i = i;
	
	/*
	Packet * packet = new Packet();
	packet->packet_type = MODEL2_ADD;
	packet->i = i;
	*/

	std::string buffer = serializeToChar(packet);
	char * packet_data = (char*)(buffer.data());
	//const unsigned int packet_size = sizeof(buffer);
	//const unsigned int packet_size = buffer.size() + 1;
	const unsigned int packet_size = buffer.size();

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
	
	Packet * packet = new Packet();
	packet->packet_type = FLOAT_PACKET;
	packet->f = f;
	
	/*
	Packet * packet = new Packet();
	packet->packet_type = FLOAT_PACKET;
	packet->f = f;
	*/

	std::string buffer = serializeToChar(packet);
	char * packet_data = (char*)(buffer.data());
	//const unsigned int packet_size = buffer.size() + 1;
	//const unsigned int packet_size = sizeof(buffer);
	const unsigned int packet_size = buffer.size();

	sendSizeData(packet_size);
	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}


void ClientGame::addToModelPart(int i)
{
	
	Packet * packet = new Packet();
	packet->packet_type = ADD_TO_MODEL_PART;
	//packet.i = i;
	Part part;
	part.N = i;
	packet->p = part;
	
	/*
	Packet * packet = new Packet();
	packet->packet_type = ADD_TO_MODEL_PART;
	packet->i = i;
	*/

	std::string buffer = serializeToChar(packet);
	char * packet_data = (char*)(buffer.data());
	//const unsigned int packet_size = buffer.size() + 1;
	const unsigned int packet_size = buffer.size();
	//const unsigned int packet_size = sizeof(buffer);

	sendSizeData(packet_size);
	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}


void ClientGame::changeModelString(std::string str)
{
	
	Packet * packet = new Packet();
	packet->packet_type = CHANGE_MODEL_STRING;
	packet->s = str;
	
	/*
	Packet * packet = new Packet();
	packet->packet_type = CHANGE_MODEL_STRING;
	packet->s = str;
	*/

	std::string buffer = serializeToChar(packet);
	char * packet_data = (char*)(buffer.data());
	//const unsigned int packet_size = buffer.size() + 1;
	const unsigned int packet_size = buffer.size();
	//const unsigned int packet_size = sizeof(buffer);

	sendSizeData(packet_size);
	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}


void ClientGame::modifyModelQuat(int x, int y, int z, int w) {
	Packet * packet = new Packet();
	packet->packet_type = MODIFY_MODEL_QUAT;
	/*
	glm::quat quatf(0, 0, 0, 0);
	if (c == 'x') {
		quatf.x += i;
	}
	else if (c == 'y') {
		quatf.y += i;
	}
	else if (c == 'z'){
		quatf.z += i;
	}
	else if (c == 'w') {
		quatf.w += i;
	}
	*/
	//packet.q = glm::quat(w, x, y, z);
	//printf("%f, %f, %f, %f\n", (packet.q).x, (packet.q).y, (packet.q).z, (packet.q).w);
	//packet->vec = glm::vec3(x, y, z);
	//printf("%f,%f,%f\n", (packet->vec).x, (packet->vec).y, (packet->vec).z);
	packet->OVRvec = OVR::Vector3f(x, y, z);
	printf("%f,%f,%f\n", (packet->OVRvec).x, (packet->OVRvec).y, (packet->OVRvec).z);
	//packet.u = x * 1000;
	//printf("Client sending DWORD : %i\n", packet.u);

	//packet->Mat4 = OVR::Matrix4f(x, y, z, w, x, y, z, w, x, y, z, w, x, y, z, w);
	//printf("%f,%f,%f,%f\n", (packet->Mat4).M[0][0], (packet->Mat4).M[1][1], (packet->Mat4).M[2][2], (packet->Mat4).M[3][3]);

	std::string buffer = serializeToChar(packet);
	char * packet_data = (char*)(buffer.data());
	const unsigned int packet_size = buffer.size();

	sendSizeData(packet_size);
	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}


void ClientGame::sendExitPacket()
{
	
	Packet * packet = new Packet();
	packet->packet_type = CLIENT_EXIT;
	
	/*
	Packet * packet = new Packet();
	packet->packet_type = CLIENT_EXIT;
	*/

	std::string buffer = serializeToChar(packet);
	char * packet_data = (char*)(buffer.data());
	//const unsigned int packet_size = buffer.size() + 1;
	const unsigned int packet_size = buffer.size();
	//const unsigned int packet_size = sizeof(buffer);

	sendSizeData(packet_size);
	NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::update()
{
    Packet * packet = new Packet();
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
			if ((data_length - i + tempBuf.size()) < sizeof(Size)) {
				//just append the rest of the data, add to i, and skip everything else
				//tempBuf.append(&(network_data[i]));
				tempBuf.append(&(network_data[i]), (data_length - i - 1));
				//add enough to overflow the while condition
				i += sizeof(Size);

				continue;
			}

			//if we have enough data in the network_data and tempBuf, append enough data to tempBuf to
			//construct a Size object, deserialize tempBuf into Size size, use value in size to update
			//nextDataSize, clear tempBuf (because we have already deserialized), increment i, flip the 
			//curPacket switch (because we constructed a full object), and skip rest of loop
			else{
				//dynamic, so keep track of starting value for computations
				int s = tempBuf.size();

				tempBuf.append(&(network_data[i]), (sizeof(Size) - s));
				size.deserialize((char*)(tempBuf.data()));
				nextDataSize = size.size;
				curPacket = !curPacket;
				
				i += (sizeof(Size) - s);

				tempBuf.clear();

				continue;
			}
			//no matter what, we skip the rest of the loop if the current data is of Size type
			//continue;
		}

		else {
			//if the remaining data + data in tempBuf < enough data to construct the next Packet
			//add remaining data to tempBuf, increment i, skip rest of loop
			if ((data_length - i + tempBuf.size()) < nextDataSize) {
				//tempBuf.append(&(network_data[i]));
				tempBuf.append(&(network_data[i]), (data_length - i - 1));
				i += nextDataSize;
				continue;
			}

			//if we have enough data to construct the next full Packet, append enough data to tempBuf to
			//construct the Packet, deserialize tempBuf into Packet packet, clear tempBuf, flip the 
			//curPacket switch, increment i, and MOVE ON to rest of loop
			else {
				int s = tempBuf.size();

				tempBuf.append(&(network_data[i]), (nextDataSize - s));
				packet = deserializeToPacket((tempBuf.data()), nextDataSize);
				curPacket = !curPacket;
				
				i += (nextDataSize - s);

				tempBuf.clear();
			}
		}



		//packet.deserialize(&(network_data[i]));
		//packet = deserializeToPacket(&(network_data[i]), sizeof(Packet));
		
        //i += sizeof(Packet);
		//curPacket = !curPacket;

        switch (packet->packet_type) {

            case ACTION_EVENT:

                printf("client received action event packet from server\n");

                //sendActionPackets();
				//added
				//sendStringPackets(mPiece);

				//sendFloat(iter + 0.5f);

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
				clientModel2->C = (packet->m2).C;
				clientModel2->I = (packet->m2).I;
				clientModel2->P = (packet->m2).P;

				printf("Model2 updated with char %c, int %i, Part %i \n", clientModel2->C, clientModel2->I, (clientModel2->P).N);

				break;
			}

			case MODEL_UPDATE:
			{
				*clientModel = packet->m;

				//printf("Model updated with Part %i, string %s \n", (clientModel->P)->N, (clientModel->S).c_str());
				//printf("Model updated with Quat %f, %f, %f, %f \n", (clientModel->Q).x, (clientModel->Q).y, (clientModel->Q).z, (clientModel->Q).w);
				//printf("Model updated with vec3 %f, %f, %f \n", (clientModel->vec).x, (clientModel->vec).y, (clientModel->vec).z);
				printf("Model updated with Vector3f %f, %f, %f \n", (clientModel->OVRvec).x, (clientModel->OVRvec).y, (clientModel->OVRvec).z);
				//printf("Model updated with DWORD %i \n", clientModel->d);
				//printf("Model updated with Matrix4f %f, %f, %f, %f\n", (clientModel->Mat4).M[0][0], (clientModel->Mat4).M[1][1], (clientModel->Mat4).M[2][2], (clientModel->Mat4).M[3][3]);

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
			modifyModelQuat(1, 2, 3, 0);


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
			modifyModelQuat(4, 5, 6, 0);

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
			modifyModelQuat(7, 8, 9, 0);

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
