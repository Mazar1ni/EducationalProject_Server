#include "Server.h"
#include "Log.h"
#include <iostream>

#pragma comment(lib, "WS2_32.lib")

int main(int argc, char** argv)
{
	if (argc > 1)
	{
		if (strcmp(argv[1], "debug") == 0)
			Log::setPriority(Log::debug);
		else if (strcmp(argv[1], "info") == 0)
			Log::setPriority(Log::info);
		else if (strcmp(argv[1], "warning") == 0)
			Log::setPriority(Log::warning);
		else if (strcmp(argv[1], "error") == 0)
			Log::setPriority(Log::error);
	}

	Server* server = new Server;
	server->connect();

	return 0;
}