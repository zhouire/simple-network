#include "StdAfx.h"
#include "ServerGame.h"


unsigned int ServerGame::client_id; 
//Model * centralModel = new Model();



ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;
	curPacket = new bool;
	nextDataSize = new int;

	//centralModel = new Model(OVR::Vector3f(0, 0, 0));
	//centralModel = new Model3(OVR::Vector3f(0, 0, 0));
	centralModel = new Model3(glm::vec3(0, 0, 0));
	centralModel2 = new Model2;

	centralModel2->C = 'q';
	centralModel2->I = 0;
	Part centralPart;
	centralPart.N = 0;
	(centralModel2->P) = centralPart;

	//initializes values of centralModel
	//centralModel->S = "hellouniverse";
	centralModel->V = std::vector<int>{ 0,1,2,3 };

	centralModel->P = new Part();
	*(centralModel->P) = centralPart;

	centralModel->Q = glm::quat(0,0,0,0);
	//(centralModel->Q).x = 0;
	//(centralModel->Q).y = 0;
	//(centralModel->Q).z = 0;
	//(centralModel->Q).w = 0;

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

		curPacket[client_id] = false;

		nextDataSize[client_id] = sizeof(Size);
		tempBuf.push_back("");
        client_id++;
   }

   //if ((network->sessions).size()>0) {
   //if (client_id>0){
	   //printf("receiving\n");
	receiveFromClients();
   //}
}

void ServerGame::receiveFromClients()
{
    //Packet packet;
	Packet * packet;
	Size size;

    // go through all clients
    //std::map<unsigned int, SOCKET>::iterator iter;

//reset:
	//printf("resetting on client exit");

    //for(iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
	//for (auto iter = network->sessions.cbegin(), next_it = network->sessions.cbegin(); iter != network->sessions.cend(); iter = next_it)
	for (auto iter = network->sessions.begin(); iter != network->sessions.end() /* not hoisted */; /* no inc. */)
	{
		//next_it = iter; ++next_it;
		bool client_exit = false;

		int data_length = network->receiveData(iter->first, network_data);

        if (data_length <= 0) 
        {
            //no data recieved
			//printf("nothing\n");
            continue;
        }

        int i = 0;
        while (i < (unsigned int)data_length) 
        {
			//this part deals with receiving data of varying sizes
			
			if (!(curPacket[iter->first])) {
				//if the remaining data + data in tempBuf < enough data to construct Size object
				//just add the remaining data to the tempBuf, increment i, skip rest of loop
				if ((data_length - i + tempBuf[iter->first].size()) < sizeof(Size)) {
					//printf("too short\n");
					//just append the rest of the data, add to i, and skip everything else
					//tempBuf[client_id].append(&(network_data[i]));
					tempBuf[iter->first].append(&(network_data[i]), (data_length - i - 1));
					//add enough to overflow the while condition
					i += sizeof(Size);

					continue;
				}

				//if we have enough data in the network_data and tempBuf, append enough data to tempBuf to
				//construct a Size object, deserialize tempBuf into Size size, use value in size to update
				//nextDataSize, clear tempBuf (because we have already deserialized), increment i, flip the 
				//curPacket switch (because we constructed a full object), and skip rest of loop
				else {
					//tempBuf[client_id].append(&(network_data[i]), (sizeof(Size) - tempBuf.size()));
					int s = tempBuf[iter->first].size();

					tempBuf[iter->first].append(&(network_data[i]), (sizeof(Size) - s));
					size.deserialize((char*)(tempBuf[iter->first].data()));
					nextDataSize[iter->first] = size.size;
					//curPacket[iter->first] = !(curPacket[iter->first]);
					curPacket[iter->first] = true;
					
					i += (sizeof(Size) - s);

					tempBuf[iter->first].clear();

					continue;
				}
				//no matter what, we skip the rest of the loop if the current data is of Size type
				
			}

			else {
				//if the remaining data + data in tempBuf < enough data to construct the next Packet
				//add remaining data to tempBuf, increment i, skip rest of loop
				if ((data_length - i + tempBuf[iter->first].size()) < nextDataSize[iter->first]) {
					//tempBuf[iter->first].append(&(network_data[i]));
					tempBuf[iter->first].append(&(network_data[i]), (data_length - i - 1));
					i += nextDataSize[iter->first];
					continue;
				}

				//if we have enough data to construct the next full Packet, append enough data to tempBuf to
				//construct the Packet, deserialize tempBuf into Packet packet, clear tempBuf, flip the 
				//curPacket switch, increment i, and MOVE ON to rest of loop
				else {
					int s = tempBuf[iter->first].size();

					tempBuf[iter->first].append(&(network_data[i]), (nextDataSize[iter->first] - s));
					packet = deserializeToPacket((tempBuf[iter->first].data()), (nextDataSize[iter->first]));
					//packet = deserializeToPacket(&((tempBuf[iter->first])[0]), nextDataSize[iter->first]);
					//curPacket[iter->first] = !(curPacket[iter->first]);
					curPacket[iter->first] = false;
					
					i += (nextDataSize[iter->first] - s);

					tempBuf[iter->first].clear();
				}
			}


			//packet = deserializeToPacket(&(network_data[i]), sizeof(Packet));
			//i += sizeof(Packet);

            switch (packet->packet_type) {

                case INIT_CONNECTION:

                    printf("server received init packet from client %i\n", iter->first);

                    sendActionPackets();

                    break;

                case ACTION_EVENT:

                    printf("server received action event packet from client %i\n", iter->first);
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
					(centralModel->V).push_back(packet->i);
					std::vector<int> v = centralModel->V;
					printf("%i : added num %i, %i \n\n", iter->first, v[v.size() - 1], v.size());

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
					printf("%i : Server got float %f \n", iter->first, packet->f);

					sendActionPackets();

					break;
				}


				case MODEL2_ADD:
				{

					(centralModel2->I) += packet->i;
					printf("%i : Server got int %i, final value %i \n", iter->first, packet->i, centralModel2->I);

					sendModel2Update();

					break;

				}

				case ADD_TO_MODEL_PART:
				{

					//(centralModel->P)->N += packet.i;
					(centralModel->P)->N += (packet->p).N;
					printf("%i : Server received int %i, Model Part int %i \n", iter->first, packet->p.N, (centralModel->P)->N);

					sendModelUpdate();

					break;

				}

				case CHANGE_MODEL_STRING:
				{

					centralModel->S = packet->s;
					printf("%i : Server received string %s, Model string now %s \n", iter->first, (packet->s).c_str(), (centralModel->S).c_str());

					sendModelUpdate();

					break;

				}

				case MODIFY_MODEL_QUAT:
				{
					/*
					(centralModel->Q).x = (centralModel->Q).x + (packet.q).x;
					(centralModel->Q).y = (centralModel->Q).y + (packet.q).y;
					(centralModel->Q).z = (centralModel->Q).z + (packet.q).z;
					(centralModel->Q).w = (centralModel->Q).w + (packet.q).w;
					
					//centralModel->Q = packet.q;

					printf("%i : Server received quat %f, %f, %f, %f \n", iter->first, (centralModel->Q).x, (centralModel->Q).y, (centralModel->Q).z, (centralModel->Q).w);
					*/

					
					centralModel->OVRvec = packet->OVRvec;
					//centralModel->d = packet.u;
					//centralModel->Mat4 = packet->Mat4;

					printf("%i : Server received Vector3f %f, %f, %f, \n", iter->first, (centralModel->OVRvec).x, (centralModel->OVRvec).y, (centralModel->OVRvec).z);
					//printf("%i : Server got DWORD %i\n", iter->first, centralModel->d);
					//printf("%i : Server received Matrix4f %f, %f, %f, %f, \n", iter->first, (centralModel->Mat4).M[0][0], (centralModel->Mat4).M[1][1], (centralModel->Mat4).M[2][2], (centralModel->Mat4).M[3][3]);

					sendModelUpdate();

					break;
				}

				case CLIENT_EXIT:
				{
					printf("Client %i is disconnecting. Closing the socket.", iter->first);

					closesocket(iter->second);
					WSACleanup();
					//iter = (network->sessions).erase(iter);
					i += data_length;
					client_exit = true;
					(network->sessions).erase(iter++);
					
					//iter = (network->sessions).begin();
					//goto reset;

					break;
				}

                default:

                    printf("error in packet types\n");

					printf("%i\n", (network->sessions).size());

                    break;
            }

        }

		if (!client_exit) {
			++iter;
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

std::string ServerGame::serializeToChar(Packet * packet) 
{
	// serialize obj into an std::string
	std::string serial_str;
	boost::iostreams::back_insert_device<std::string> inserter(serial_str);
	boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
	boost::archive::binary_oarchive oa(s);

	oa << packet; //serializable object
	//oa & packet;

	// don't forget to flush the stream to finish writing into the buffer
	s.flush();

	//return (char*)(serial_str.data());
	return serial_str;
}

Packet * ServerGame::deserializeToPacket(const char * buffer, int buflen)
{
	Packet * packet;
	// wrap buffer inside a stream and deserialize serial_str into obj
	boost::iostreams::basic_array_source<char> device(buffer, buflen);
	boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
	boost::archive::binary_iarchive ia(s);
	ia >> packet;
	//ia & packet;
	//printf("deserializing...");

	return packet; 
}

void ServerGame::sendSizeData(int packet_size) {
	Size s;
	s.size = packet_size;

	const unsigned int s_size = sizeof(Size);
	char s_data[s_size];

	s.serialize(s_data);

	network->sendToAll(s_data, s_size);
}


void ServerGame::sendActionPackets()
{
	Packet * packet = new Packet();
	packet->packet_type = ACTION_EVENT;

	//const unsigned int packet_size = sizeof(Packet);

	//char * packet_data = serializeToChar(packet);
	std::string buffer = serializeToChar(packet);
	char * packet_data = (char*)(buffer.data());
	//const unsigned int packet_size = buffer.length() + 1;
	//const unsigned int packet_size = sizeof(buffer);
	const unsigned int packet_size = buffer.size();

	sendSizeData(packet_size);
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
	Packet * packet = new Packet();
	packet->packet_type = MODEL_UPDATE;
	packet->m = *centralModel;

	//const unsigned int packet_size = sizeof(Packet);

	//char * packet_data = serializeToChar(packet);

	std::string buffer = serializeToChar(packet);
	char * packet_data = (char*)(buffer.data());
	//const unsigned int packet_size = buffer.size() + 1;
	const unsigned int packet_size = buffer.size();
	//const unsigned int packet_size = sizeof(buffer);

	sendSizeData(packet_size);
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
	//const unsigned int packet_size = sizeof(Packet);
	//char packet_data[packet_size];

	Packet * packet = new Packet();
	packet->packet_type = MODEL2_UPDATE;
	packet->m2 = *centralModel2;

	//packet.serialize(packet_data);
	//char * packet_data = serializeToChar(packet);

	std::string buffer = serializeToChar(packet);
	char * packet_data = (char*)(buffer.data());
	//const unsigned int packet_size = buffer.size() + 1;
	const unsigned int packet_size = buffer.size();
	//const unsigned int packet_size = sizeof(buffer);

	sendSizeData(packet_size);
	network->sendToAll(packet_data, packet_size);
}


