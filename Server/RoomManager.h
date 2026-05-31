#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "Room.h"
#include "LoggedUser.h"

class RoomManager
{
public:
	void createRoom(const LoggedUser& loggedUser, RoomData& roomData);
	void deleteRoom(int ID);
	unsigned int getRoomState(int ID);
	std::vector<RoomData> getRooms();
	Room& getRoom(int id);
	int getNextFreeRoomID();
	void removeUserFromRooms(const std::string& username);
private:
	std::string toExceptionString(int ID);
	std::map<int, Room> m_rooms;
	int nextFreeRoomID = 0;
};