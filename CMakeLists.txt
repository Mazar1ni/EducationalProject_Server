cmake_minimum_required (VERSION 3.11)

set (PROJECT Server) 
 
project (${PROJECT}) 
 
set (HEADERS 
	Server/Log.h
	Server/Server.h
	Server/Client.h
	Server/Rooms.h
	Server/Room.h)
 
set (SOURCES 
	Server/Log.cpp
	Server/main.cpp
	Server/Server.cpp
	Server/Client.cpp
	Server/Rooms.cpp
	Server/Room.cpp) 
 
add_executable (${PROJECT} ${HEADERS} ${SOURCES})