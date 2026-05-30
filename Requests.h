#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Buffer.h"
#include "IRequestHandler.h"

struct LoginRequest
{
	std::string username;
	std::string password;
};

struct SignupRequest
{
	std::string username;
	std::string password;
	std::string email;
};

/*This struct is used in RequestResult so that when a user
logs in or signs up it will be able to return its username
so that communicator will be able to call logout when he disconnects*/
struct SuccessfulLoginInfo
{
	unsigned int success;
	std::string username;
};

struct RequestResult
{
	IRequestHandler* newHandler;
	Buffer response;
	SuccessfulLoginInfo successfulLoginInfo;
};

struct RequestInfo
{
	int id;
	time_t receivalTime;
	std::vector<unsigned char> buffer;
};

struct GetPlayersInRoomRequest
{
	unsigned int roomId;
};

struct JoinRoomRequest
{
	unsigned int roomId;
};

struct CreateRoomRequest
{
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
};