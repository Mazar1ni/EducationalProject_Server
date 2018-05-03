#pragma once

#include <thread>
#include <iostream>
#include <WinSock2.h>
#include <set>
#include "Rooms.h"
#include "Log.h"

using namespace std;

class Server
{
public:
	Server(Log* log);
	~Server();
	void connect();

private:
	SOCKET Socket;
	set<string> names;
	Rooms* rooms;
	Log* log;
};
