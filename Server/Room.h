#pragma once

#include <vector>
#include <string>
#include <WinSock2.h>
#include <algorithm>
#include <set>
#include "Log.h"
#include "RoomDeleteListener.h"

class Rooms;

using namespace std;

class Room
{
public:
	Room(string nameRoom, SOCKET socket, string name, weak_ptr<RoomDeleteListener*> rooms);
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
	vector<string> historyMessage;
	weak_ptr<RoomDeleteListener*> roomsPtr;
};

