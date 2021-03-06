#include "Server.h"
#include "Client.h"

Server::Server()
{
	WSAData WSData;
	// initialize the use of the Winsock DLL process
	if (WSAStartup(MAKEWORD(2, 2), &WSData) != 0)
	{
		Log::print(Log::error, "Server::Server - WSASratup no succes");
		return;
	}

	// create socket
	Socket = socket(AF_INET, SOCK_STREAM, 6);

	if (Socket == SOCKET_ERROR)
	{
		Log::print(Log::error, "Server::Server - Socket not created");
		return;
	}

	// create struct for bind to her socket
	sockaddr_in SA;
	SA.sin_family = AF_INET;
	SA.sin_port = htons(7070);
	SA.sin_addr.S_un.S_addr = INADDR_ANY;

	// connection socket and struct
	if (::bind(Socket, (sockaddr*)&SA, sizeof(SA)) == SOCKET_ERROR)
	{
		Log::print(Log::error, "Server::Server - Socket succeed binded");
		return;
	}

	// listening port
	if (listen(Socket, SOMAXCONN) == SOCKET_ERROR) 
	{
		Log::print(Log::error, "Server::Server - Don't start listenin at port");
		return;
	}

	rooms = new Rooms;
}

Server::~Server()
{
	for (auto it = sockets.begin(); it != sockets.end(); ++it)
	{
		send(*it, "@close_server", sizeof("@close_server"), NULL);
	}
	closesocket(Socket);
}

void Server::connect()
{
	thread command(&Server::command, this);
	command.detach();

	SOCKET NS;
	// catch all connections
	while ((NS = accept(Socket, 0, 0)) != INVALID_SOCKET)
	{
		try
		{
			sockets.insert(NS);
			Client* client = new Client(NS, *rooms, names, sockets);
		}
		catch (...) 
		{
			Log::print(Log::warning, "Server::connect - Failed to create clients class");
		}
		NS = INVALID_SOCKET;
	}
}

void Server::command()
{
	string message;
	while (true)
	{
		getline(cin, message);

		if (message.find("@quit") != -1)
		{
			delete this;
		}

		message.clear();
	}
}
