#pragma once
#include <iostream>
#include "Room.h"
#include <vector>

struct LoginResponse
{
	unsigned int status;
};

struct SignupResponse
{
	unsigned int status;
};

struct LogoutResponse
{
	unsigned int status;
};

struct JoinRoomResponse
{
	unsigned int status;
};

struct CreateRoomResponse
{
	unsigned int status;
	int roomId;
};

struct ErrorResponse
{
	std::string message;
};

struct GetRoomsResponse
{
	unsigned int status;
	std::vector<RoomData> rooms;
};

struct GetPlayersInRoomResponse
{
	std::vector<std::string> players;
};

struct HighScoreResponse
{
	unsigned int status;
	std::vector<std::string> highScores;
};

struct GetPersonalStatsResponse
{
	unsigned int status;
	std::vector<std::string> statistics;
};