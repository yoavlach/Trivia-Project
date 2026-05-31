#include "Room.h"

/**
 * Constructs a new Room object with the given metadata.
 * @param metadata The RoomData struct containing the room's ID, name, max players, time per question, and active status.
 */
Room::Room(const RoomData& metadata) : m_metadata(metadata)
{
}

/**
 * Adds a new user to the room.
 * @param loggedUser The user object representing the player to be added to the room.
 */
void Room::addUser(const LoggedUser& loggedUser)
{
	m_users.push_back(loggedUser);
}

/**
 * Removes a specific user from the room.
 * @param loggedUser The user object representing the player to be removed from the room.
 */
void Room::removeUser(const LoggedUser& loggedUser)
{
	for (int i = 0; i < m_users.size(); i++)
	{
		if (m_users[i].getUsername() == loggedUser.getUsername())
			m_users.erase(m_users.begin() + i);
	}
}

/**
 * Retrieves a list of all users currently in the room.
 * @return std::vector<std::string> A vector containing the usernames of all players in the room.
 */
std::vector<std::string> Room::getAllUsers() const
{
	std::vector<std::string> allUsers;
	for (auto i : m_users)
		allUsers.push_back(i.getUsername());
	return allUsers;
}

/**
 * Gets the metadata associated with the room.
 * @return RoomData The room's metadata details.
 */
RoomData Room::getRoomData() const
{
	return m_metadata;
}