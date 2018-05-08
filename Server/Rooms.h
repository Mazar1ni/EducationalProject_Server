#pragma once

#include <map>
#include <vector>
#include "Room.h"
#include "Log.h"
#include "RoomDeleteListener.h"

using namespace std;

class Rooms : public RoomDeleteListener
{
public:
	Rooms();
	~Rooms();

	Room* createRoom(string name, SOCKET socket, string nameClient);
	Room* openRoom(string name, SOCKET socket, string nameClient);
	void deleteRoom(string name);
	vector<string> getRooms();

private:
	map<string, Room*> rooms;
};

