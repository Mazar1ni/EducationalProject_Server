#pragma once
#include <thread>
#include <iostream>
#include <WinSock2.h>
#include <set>
#include <mutex>
#include "Rooms.h"
#include "Room.h"
#include "Log.h"

using namespace std;

class Clients
{
public:
	Clients(SOCKET socket, Rooms& r, set<string>& names, Log* log);
	~Clients();

private:
	void readMessage();

	string name;
	SOCKET socket;
	Rooms& rooms;
	bool isAuth = false;
	set<string>& namesClients;
	Room* room = nullptr;
	const int lenght = 1024;
	Log* log;
};

