#pragma once

#include <string>

class RoomDeleteListener
{
public:
	RoomDeleteListener();
	virtual ~RoomDeleteListener();

	virtual void deleteRoom(std::string name);
};

