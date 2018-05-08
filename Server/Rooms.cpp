#include "Rooms.h"

Rooms::Rooms()
{
}

Rooms::~Rooms()
{
}

Room* Rooms::createRoom(string name, SOCKET socket, string nameClient)
{
	// search similar names rooms
	if (rooms.find(name) != rooms.end())
		return nullptr;

	Room* room;
	// create room
	try
	{
		room = new Room(name, socket, nameClient, this);
	}
	catch (...) {
		Log::print(Log::warning, "Rooms::createRoom - Failed to create room class");
	}
	rooms.insert(make_pair(name, room));
	return room;                                 
}

Room* Rooms::openRoom(string name, SOCKET socket, string nameClient)
{
	// search similar names rooms and if exist then open her
	map<string, Room*>::iterator it;
	if ((it = rooms.find(name)) != rooms.end())
	{
		it->second->openRoom(socket, nameClient);
		return it->second;
	}
	return nullptr;	
}

vector<string> Rooms::getRooms()
{
	// return all names rooms
	vector<string> names;

	for (auto room : rooms)
	{
		names.push_back(room.second->getName());
	}

	return names;
}

void Rooms::deleteRoom(string name)
{
	// remove room
	auto it = rooms.find(name);
	rooms.erase(it);
}
