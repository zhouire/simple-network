// main.cpp : Defines the entry point for the console application.
//

// may need #include "stdafx.h" in visual studio
#include "stdafx.h"
#include "ServerGame.h"
#include "ClientGame.h"
// used for multi-threading
#include <process.h>

//void serverLoop(void *);
void clientLoop(void *);
//void clientKeyLoop();
BOOL WINAPI ClientExitRoutine(_In_ DWORD);

//ServerGame * server;
ClientGame * client;

//ClientGame * client2;

int main()
{

	// initialize the server
	//server = new ServerGame();

	client = new ClientGame();

	SetConsoleCtrlHandler(ClientExitRoutine, true);
	// create thread with arbitrary argument for the run function
    _beginthread( clientLoop, 0, (void*)12);

    // initialize the client 
    //client = new ClientGame();

	clientLoop((void*)12);
}

/*
void serverLoop(void * arg) 
{ 
    while(true) 
    {
        server->update();
    }
}
*/

void clientLoop(void * arg)
{
    while(true)
    {
        //do game stuff
        client->update();
		client->updateKeyPress();

		//client2->update();
    }
}

BOOL WINAPI ClientExitRoutine(_In_ DWORD dwCtrlType) {
	client->sendExitPacket();

	return false;
}

/*
void clientKeyLoop()
{
	while (true)
	{
		client->updateKeyPress();
	}
}
*/


/*
void clientLoop()
{
	while (true)
	{
		//do game stuff
		client->update();
		client->updateKeyPress();

		//client2->update();
	}
}
*/
