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

	std::string serializeToChar(Packet packet)
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


	void sendSizeData(int packet_size) {
		Size s;
		s.size = packet_size;

		const unsigned int s_size = sizeof(Size);
		char s_data[s_size];

		s.serialize(s_data);

		NetworkServices::sendMessage(network1->ConnectSocket, s_data, s_size);
	}


	void sendGenericPacket()
	{
		//send action packet

		Packet packet;
		packet.packet_type = ACTION_EVENT;

		std::string buffer = serializeToChar(packet);
		char * packet_data = (char*)(buffer.data());
		const unsigned int packet_size = buffer.size();
		//const unsigned int packet_size = sizeof(buffer);

		sendSizeData(packet_size);
		NetworkServices::sendMessage(network1->ConnectSocket, packet_data, packet_size);
	}
};