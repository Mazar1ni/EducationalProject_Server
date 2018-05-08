#include "Room.h"
#include "Rooms.h"
#include <iostream>

Room::Room(string nameRoom, SOCKET socket, string name, Rooms* rooms) : nameRoom(nameRoom), parent(rooms)
{
	// added socket and name in array
	socketClients.insert(socket);
	namesClients.insert(name);
	Log::print(Log::info, "Room::Room - created room");
}

Room::~Room()
{
	Log::print(Log::info, "Room::~Room - delete room");
}

void Room::openRoom(SOCKET socket, string name)
{
	// added socket and name in array
	socketClients.insert(socket);
	namesClients.insert(name);

	sendMessagesAllClients("user " + name + " connected room@send", socket);
	Log::print(Log::info, "Room::openRoom - connected room");
}

void Room::leaveRoom(SOCKET socket, string name)
{
	// remove socket and name
	socketClients.erase(socketClients.find(socket));
	namesClients.erase(namesClients.find(name));

	send(socket, "@leave", sizeof("@leave"), NULL);

	Log::print(Log::info, "Room::leaveRoom - leave room");

	// if name count = 0 then delete room
	if (namesClients.size() == 0)
	{
		parent->deleteRoom(nameRoom);
		delete this;
		return;
	}

	sendMessagesAllClients("user " + name + " leave room@send", socket);
}

void Room::sendMessagesAllClients(string message, SOCKET owner)
{
	// send message all clients except owner
 	for (SOCKET socket : socketClients)
	{
		if(owner != socket)
			send(socket, message.c_str(), message.length(), 0);
	}

	message.erase(message.size() - strlen("@send"), strlen("@send"));

	Log::print(Log::info, "Room::sendMessagesAllClients - send message room: " + message);

	// add message in history
	historyMessage.push_back(message);

	// if history count > 20 then delete first message
	if (historyMessage.size() > 20)
	{
		historyMessage.front().erase();
	}
}

vector<string> Room::getHistoryMessges()
{
	return historyMessage;
}

set<string> Room::getNameClients()
{
	return namesClients;
}

string Room::getName()
{
	return nameRoom;
}
