#include "Server.h"
#include "Log.h"
#include <iostream>

#pragma comment(lib, "WS2_32.lib")

int main(int argc, char** argv)
{
	Log* log = new Log;
	if (argc > 1)
	{
		if (strcmp(argv[1], "debug") == 0)
			log->setPriority(Log::debug);
		else if (strcmp(argv[1], "info") == 0)
			log->setPriority(Log::info);
		else if (strcmp(argv[1], "warning") == 0)
			log->setPriority(Log::warning);
		else if (strcmp(argv[1], "error") == 0)
			log->setPriority(Log::error);
	}

	Server server(log);
	server.connect();

	return 0;
}