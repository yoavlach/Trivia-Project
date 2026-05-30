#include "RoomManager.h"
#include <iostream>
#include <string>
#include <exception>

/**
 * Creates a new room and adds it to the manager.
 * @param loggedUser The user who created the room.
 * @param roomData The metadata of the room to be created.
 */
void RoomManager::createRoom(const LoggedUser& loggedUser, RoomData& roomData)
{
	roomData.id = nextFreeRoomID;
	Room room{ roomData };
	room.addUser(loggedUser.getUsername());
	m_rooms.insert({ roomData.id, room });
	nextFreeRoomID++;
}

/**
 * Deletes an existing room from the manager.
 * @param ID The ID of the room to be deleted.
 */
void RoomManager::deleteRoom(int ID)
{
	if(m_rooms.find(ID) == m_rooms.end())
		throw std::runtime_error(toExceptionString(ID));
	m_rooms.erase(m_rooms.find(ID));
}

/**
 * Retrieves the current state of a specific room.
 * @param ID The ID of the room.
 * @return unsigned int The status of the room (e.g., waiting for players or game in progress).
 */
unsigned int RoomManager::getRoomState(int ID)
{
	if (m_rooms.find(ID) == m_rooms.end())
		throw std::runtime_error(toExceptionString(ID));
	return m_rooms.find(ID)->second.getRoomData().roomStatus;
}

/**
 * Retrieves a list of all active rooms currently managed by the server.
 * @return std::vector<RoomData> A vector containing the metadata of all active rooms.
 */
std::vector<RoomData> RoomManager::getRooms()
{
	std::vector<RoomData> rooms;
	for (auto it : m_rooms)
		rooms.push_back(it.second.getRoomData());
	return rooms;
}

/**
 * Retrieves a specific room object by its ID.
 * @param id The ID of the room to retrieve.
 * @return Room The Room object associated with the given ID.
 */
Room& RoomManager::getRoom(int id)
{
	if (m_rooms.find(id) == m_rooms.end())
		throw std::runtime_error(toExceptionString(id));
	return m_rooms.find(id)->second;
}

int RoomManager::getNextFreeRoomID()
{
	return nextFreeRoomID;
}

/**
 * Generates an exception message indicating a room was not found.
 * @param ID The ID of the room that was not found.
 * @return const char* A C-style string containing the exception message.
 */
std::string RoomManager::toExceptionString(int ID)
{
	return "Room with ID: " + std::to_string(ID) + " not found.";
}

/**
 * Removes a user who logged out from all of the rooms
 * @param username the username to remove
 */
void RoomManager::removeUserFromRooms(const std::string& username)
{
	for (auto& IdRoomPair : m_rooms)
		IdRoomPair.second.removeUser(LoggedUser{ username });
}
