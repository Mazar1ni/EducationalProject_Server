#include "Clients.h"

static mutex namesMutex;
static mutex roomsMutex;
static mutex roomMutex;

Clients::Clients(SOCKET socket, Rooms& r, set<string>& names, Log* log) : socket(socket), rooms(r), namesClients(names), log(log)
{
	log->print(Log::info, "Clients::readMessage - Connected client");

	// create thread for listening to the client
	try
	{
		thread client(&Clients::readMessage, this);
		client.detach();	
	}
	catch (...) 
	{
		log->print(Log::warning, "Clients::Clients - Failed to create thread");
		return;
	}
}

Clients::~Clients()
{
	roomsMutex.lock();
	// if the client is in the room, to leave it
	if (room != nullptr)
	{
		room->leaveRoom(socket, name);
	}
	roomsMutex.unlock();

	// removing a client name from an array
	namesMutex.lock();
	set<string>::iterator it;
	if((it = namesClients.find(name)) != namesClients.end())
		namesClients.erase(it);
	namesMutex.unlock();

	closesocket(socket);

	string debug;
	debug = "Disconnected client ";
	name == "" ? debug += "not authorized" : debug += name;
	log->print(Log::info, "Clients::~Clients - " + debug);
}

void Clients::readMessage()
{
	// array for messages received from the client
	char* buffer = new char[lenght];
	memset(buffer, 0, lenght);

	// tell the client that the connection is established
	send(socket, "@connected", sizeof("@connected"), NULL);

	while (true)
	{
		// wait message from client
		int k = recv(socket, buffer, lenght, NULL);
		// if k <= 0 then connection loss
		if (k>0)
		{
			string message = buffer;

			string::size_type identificator;

			// authorization
			if (message.find("@auth") != -1)
			{
				// already authorization
				if (isAuth == true)
				{
					// write to the client about the error
					send(socket, "@alreadyAuth", sizeof("@alreadyAuth"), NULL);
					continue;
				}

				// remove identificator
				message.erase(0, strlen("@auth "));

				lock_guard<mutex> lock(namesMutex);

				// similar name not found
				if (namesClients.find(message) == namesClients.end())
				{
					// add name in array namesClients and etc
					isAuth = true;
					send(socket, "@logged", sizeof("@logged"), NULL);
					name = message;
					namesClients.insert(message);
				}
				// similar name found
				else
				{
					// write to the client about the error
					send(socket, "@not_logged", sizeof("@not_logged"), NULL);
				}
			}
			// view all rooms
			else if (message.find("@view_room") != -1)
			{
				// not yet authorized
				if (isAuth == false)
				{
					// write to the client about the error
					send(socket, "@not_auth", sizeof("@not_auth"), NULL);
					continue;
				}

				// send client all room
				lock_guard<mutex> lock(roomsMutex);
				vector<string> namesRooms = rooms.getRooms();

				for (string nameRoom : namesRooms)
				{
					nameRoom += "@view_room";
					send(socket, nameRoom.c_str(), sizeof(nameRoom), 0);
				}
			}
			// create room
			else if (message.find("@create") != -1)
			{
				// not yet authorized
				if (isAuth == false)
				{
					// write to the client about the error
					send(socket, "@not_auth", sizeof("@not_auth"), NULL);
					continue;
				}

				// remove identificator
				message.erase(0, strlen("@create "));

				// if you leave the room already in the room
				lock_guard<mutex> lock(roomsMutex);
				if (room != nullptr)
				{
					room->leaveRoom(socket, name);
					room = nullptr;
					Sleep(50);
				}

				room = rooms.createRoom(message, socket, name);

				// created room
				if (room == nullptr)
				{
					send(socket, "@not_create", sizeof("@not_create"), NULL);
					log->print(Log::debug, "Clients::readMessage - not created room");
				}
				// not create room
				else
				{
					// write to the client about the error
					send(socket, "@create", sizeof("@create"), NULL);
				}
			}
			// open room
			else if (message.find("@open") != -1)
			{
				// not yet authorized
				if (isAuth == false)
				{
					// write to the client about the error
					send(socket, "@not_auth", sizeof("@not_auth"), NULL);
					continue;
				}

				// remove identificator
				message.erase(0, strlen("@open "));

				// if you leave the room already in the room
				lock_guard<mutex> lock(roomsMutex);
				if (room != nullptr)
				{
					room->leaveRoom(socket, name);
					room = nullptr;
					Sleep(50);
				}

				room = rooms.openRoom(message, socket, name);

				// opened room
				if (room != nullptr)
				{
					send(socket, "@open", sizeof("@open"), NULL);
				}
				else
				{
					// write to the client about the error
					send(socket, "@not_open", sizeof("@not_open"), NULL);
					log->print(Log::debug, "Clients::readMessage - not opened room");
				}
			}
			// leave room
			else if ((identificator = message.find("@leave")) != -1)
			{
				// not yet authorized
				if (isAuth == false)
				{
					// write to the client about the error
					send(socket, "@not_auth", sizeof("@not_auth"), NULL);
					continue;
				}

				// room exist
				if (room == nullptr)
				{
					// write to the client about the error
					send(socket, "@not_room", sizeof("@not_room"), NULL);
				}
				else
				{
					// leave room
					lock_guard<mutex> lock(roomsMutex);
					room->leaveRoom(socket, name);
					room = nullptr;
					send(socket, "@leave", sizeof("@leave"), NULL);
				}
			}
			// send messages in room
			else if (message.find("@send") != -1)
			{
				// not yet authorized
				if (isAuth == false)
				{
					// write to the client about the error
					send(socket, "@not_auth", sizeof("@not_auth"), NULL);
					continue;
				}
				lock_guard<mutex> lock(roomMutex);

				if (room != nullptr)
				{
					room->sendMessagesAllClients(name + ": " + message, socket);
				}
				else
				{
					// write to the client about the error
					send(socket, "@not_room", sizeof("@not_room"), NULL);
				}
			}
			// history messages in room
			else if (message.find("@history") != -1)
			{
				// not yet authorized
				if (isAuth == false)
				{
					// write to the client about the error
					send(socket, "@not_auth", sizeof("@not_auth"), NULL);
					continue;
				}
				lock_guard<mutex> lock(roomMutex);

				if (room != nullptr)
				{
					// send client history message in room
					vector<string> historyMessages = room->getHistoryMessges();
					for(auto it = historyMessages.begin(); it != historyMessages.end(); ++it)
					{
						string mess = *it;
						mess += "@history";
						send(socket, mess.c_str(), mess.length(), 0);
						Sleep(20);
					}
				}
				else
				{
					// write to the client about the error
					send(socket, "@not_room", sizeof("@not_room"), NULL);
				}
			}
			// view clients in room
			else if (message.find("@view_clients") != -1)
			{
				// not yet authorized
				if (isAuth == false)
				{
					// write to the client about the error
					send(socket, "@not_auth", sizeof("@not_auth"), NULL);
					continue;
				}
				lock_guard<mutex> lock(roomMutex);
				if (room != nullptr)
				{
					// send client all name clients staying in the room
					set<string> clients = room->getNameClients();
					for (string client : clients)
					{
						string mess = client;
						mess += "@view_clients";
						send(socket, mess.c_str(), mess.length(), 0);
						Sleep(20);
					}
				}
				else
				{
					// write to the client about the error
					send(socket, "@not_room", sizeof("@not_room"), NULL);
				}
			}

			log->print(Log::debug, "Clients::readMessage - " + name + " : " + message);

			memset(buffer, 0, lenght);
		}
		else
		{
			delete this;
			return;
		}
		Sleep(100);
	}
}
