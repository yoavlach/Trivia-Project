#pragma once
#include <iostream>
#include <vector>
#include "LoggedUser.h"

enum ROOM_STATUSES { ACTIVE, NOT_ACTIVE };

struct RoomData
{
	unsigned int id;
	std::string name;
	unsigned int maxPlayers;
	unsigned int numOfQuestionsInGame;
	unsigned int timePerQuestion;
	unsigned int roomStatus;
};

class Room
{
public:
	Room(const RoomData& metadata);
	void addUser(const LoggedUser& loggedUser);
	void removeUser(const LoggedUser& loggedUser);
	std::vector<std::string> getAllUsers() const;
	RoomData getRoomData() const;
private:
	RoomData m_metadata;
	std::vector<LoggedUser> m_users;
};

