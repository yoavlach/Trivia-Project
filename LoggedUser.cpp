#include "LoggedUser.h"

/**
 * Constructor that initializes a new logged user
 *  Sets the internal member variable to the provided username
 * @param username the username of the logged user
 */
LoggedUser::LoggedUser(const std::string& username)
{
	m_username = username;
}

/**
 * Retrieves the username of the logged user
 *  Returns the internal member variable where the username is stored
 * @return the username of the logged user
 */
std::string LoggedUser::getUsername() const
{
	return m_username;
}

void LoggedUser::setIsInRoom(bool isInRoom)
{
	m_isInRoom = isInRoom;
}

bool LoggedUser::getIsInRoom()
{
	return m_isInRoom;
}
