// simple_server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ServerGame.h"
//#include "ClientGame.h"
// used for multi-threading
#include <process.h>

//void serverLoop(void *);
void serverLoop();
ServerGame * server;

int main()
{
	server = new ServerGame();
	//_beginthread(serverLoop, 0, (void*)12);

	//serverLoop((void*)12);
	serverLoop();
	
	while (true) {
		Sleep(INFINITE);
	}
	
}

//void serverLoop(void * arg)
void serverLoop()
{
	while (true)
	{
		server->update();
	}
}