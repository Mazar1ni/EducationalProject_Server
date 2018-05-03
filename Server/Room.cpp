#include "Room.h"
#include "Rooms.h"
#include <iostream>

Room::Room(string nameRoom, SOCKET socket, string name, Rooms* rooms, Log* log) : nameRoom(nameRoom), parent(rooms), log(log)
{
	// added socket and name in array
	socketClients.insert(socket);
	namesClients.insert(name);
}


Room::~Room()
{
	log->print(Log::info, "Room::~Room - delete room");
}

void Room::openRoom(SOCKET socket, string name)
{
	// added socket and name in array
	socketClients.insert(socket);
	namesClients.insert(name);

	sendMessagesAllClients("user " + name + " connected room@send", socket);
}

void Room::leaveRoom(SOCKET socket, string name)
{
	// remove socket and name
	auto socketIter = socketClients.find(socket);
	socketClients.erase(socketIter);

	auto nameIter = namesClients.find(name);
	namesClients.erase(nameIter);

	send(socket, "@leave", sizeof("@leave"), NULL);

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
