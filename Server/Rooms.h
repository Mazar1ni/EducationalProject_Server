#pragma once

#include <map>
#include <vector>
#include "Room.h"
#include "Log.h"

using namespace std;

class Rooms
{
public:
	Rooms(Log* log);
	~Rooms();

	Room* createRoom(string name, SOCKET socket, string nameClient);
	Room* openRoom(string name, SOCKET socket, string nameClient);
	void deleteRoom(string name);
	vector<string> getRooms();

private:
	map<string, Room*> rooms;
	Log* log;
};

