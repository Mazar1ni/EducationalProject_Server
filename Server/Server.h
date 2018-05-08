#pragma once

#include <thread>
#include <iostream>
#include <WinSock2.h>
#include <set>
#include <vector>
#include "Rooms.h"
#include "Log.h"

using namespace std;

class Server
{
public:
	Server();
	~Server();
	void connect();
	void command();

private:
	SOCKET Socket;
	set<string> names;
	set<SOCKET> sockets;
	Rooms* rooms;
};
