#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>

using namespace std;

class Log
{
public:
	enum Priority {null, error, warning, info, debug};
	Log();
	~Log();

	static void print(Priority priorityMessage, string message);
	static void setPriority(Priority prior);

private:
	static ofstream fout;
	static Priority priority;
	static mutex mtx;
};
