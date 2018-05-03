#include "Server.h"
#include "Clients.h"

Server::Server(Log* log) : log(log)
{
	WSAData WSData;
	// initialize the use of the Winsock DLL process
	if (WSAStartup(MAKEWORD(2, 2), &WSData) != 0)
	{
		log->print(Log::error, "Server::Server - WSASratup no succes");
		return;
	}

	// create socket
	Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (Socket == SOCKET_ERROR)
	{
		log->print(Log::error, "Server::Server - Socket not created");
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
		log->print(Log::error, "Server::Server - Socket succed binded");
		return;
	}

	// listening port
	if (listen(Socket, SOMAXCONN) == SOCKET_ERROR) 
	{
		log->print(Log::error, "Server::Server - Don't start listenin at port");
		return;
	}

	rooms = new Rooms(log);
}

Server::~Server()
{
	closesocket(Socket);
}

void Server::connect()
{
	SOCKET NS;
	// catch all connections
	while ((NS = accept(Socket, 0, 0)) != INVALID_SOCKET)
	{
		try
		{
			Clients* client = new Clients(NS, *rooms, names, log);
		}
		catch (...) 
		{
			log->print(Log::warning, "Server::connect - Failed to create clients class");
		}
		NS = INVALID_SOCKET;
	}
}
