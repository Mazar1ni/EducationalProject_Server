#define _CRT_SECURE_NO_WARNINGS
#include "Log.h"
#include <time.h>

ofstream Log::fout;
Log::Priority Log::priority;
mutex Log::mtx;

Log::Log()
{
	//fout.open("log.txt");
}

Log::~Log()
{
	/*
	if (fout.is_open())
	{
		fout.close();
	}
	*/
}

void Log::print(Priority priorityMessage, string message)
{
	lock_guard<mutex> lock(mtx);
	bool isPrint = false;
	string priorit; 
	switch (priority)
	{
	case debug:
		if (priorityMessage == debug)
		{
			isPrint = true;
			priorit = "Debug";
		}
	case info:
		if (priorityMessage == info)
		{
			isPrint = true;
			priorit = "Info";
		}
	case warning:
		if (priorityMessage == warning)
		{
			isPrint = true;
			priorit = "Warning";
		}
	case error:
		if (priorityMessage == error)
		{
			isPrint = true;
			priorit = "Error";
		}
		break;
	}

	if (isPrint == true)
	{
		// getting a date
		time_t     now = time(0);
		struct tm  tstruct;
		tstruct = *localtime(&now);

		char* c_time = new char[80];

		strftime(c_time, 80, "%d-%m-%Y.%X", &tstruct);

		// structure the data
		string str = c_time;
		str += " - ";
		str += priorit;
		str += " : ";
		str += message;
		str += "\n";

		fout << str;

		cout << str << endl;
	}
}

void Log::setPriority(Priority prior)
{
	priority = prior;
	fout.open("log.txt", ios::app);
}
