#pragma once
#include <iostream>

class LoggedUser
{
public:
	LoggedUser(const std::string& username);
	std::string getUsername() const;
	void setIsInRoom(bool isInRoom);
	bool getIsInRoom();
private:
	std::string m_username;
	bool m_isInRoom = false;
};

