#pragma once

#include "StdAfx.h"


struct ClientGameMinor
{
	ClientNetwork * network1;
	ClientGameMinor(ClientNetwork * net) {
		network1 = net;
	}

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
};