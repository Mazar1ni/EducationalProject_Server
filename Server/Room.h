#pragma once

#include <vector>
#include <string>
#include <WinSock2.h>
#include <algorithm>
#include <set>
#include "Log.h"

class Rooms;

using namespace std;

class Room
{
public:
	Room(string nameRoom, SOCKET socket, string name, Rooms* rooms, Log* log);
	~Room();
	void openRoom(SOCKET socket, string name);
	void leaveRoom(SOCKET socket, string name);
	void sendMessagesAllClients(string messages, SOCKET owner);
	vector<string> getHistoryMessges();
	set<string> getNameClients();
	string getName();

private:
	string nameRoom;
	set<SOCKET> socketClients;
	set<string> namesClients;
	Rooms* parent;
	vector<string> historyMessage;
	Log* log;
};

