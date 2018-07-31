#pragma once

#include "StdAfx.h"


struct ClientGameMinor
{
	ClientNetwork * network1;
	ClientGameMinor(ClientNetwork * net) {
		network1 = net;
	}
	/*
	void sendGenericPacket()
	{
		// send action packet

		const unsigned int packet_size = sizeof(Packet);
		//IZ: This creates an appropriately-sized char array to later fill with the converted pointer
		char packet_data[packet_size];

		Packet packet;
		packet.packet_type = ACTION_EVENT;

		//IZ: I think this actively changes packet_data from source
		packet.serialize(packet_data);

		NetworkServices::sendMessage(network1->ConnectSocket, packet_data, packet_size);
	}
	*/

	char * serializeToChar(Packet packet)
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

	void sendGenericPacket()
	{
		//send action packet

		Packet packet;
		packet.packet_type = ACTION_EVENT;

		const unsigned int packet_size = sizeof(Packet);

		char * packet_data = serializeToChar(packet);

		NetworkServices::sendMessage(network1->ConnectSocket, packet_data, packet_size);
	}
};