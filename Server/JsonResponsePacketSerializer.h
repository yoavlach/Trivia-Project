#pragma once
#include "Buffer.h"
#include "Responses.h"

class JsonResponsePacketSerializer
{
public:
	static Buffer serializeResponse(const ErrorResponse& errorResponse);
	static Buffer serializeResponse(const LoginResponse& loginResponse);
	static Buffer serializeResponse(const SignupResponse& signupResponse);

	static Buffer serializeResponse(const LogoutResponse& logoutResponse);
	static Buffer serializeResponse(const GetRoomsResponse & getRoomResponse);
	static Buffer serializeResponse(const GetPlayersInRoomResponse& getPlayersInRoomResponse);
	static Buffer serializeResponse(const JoinRoomResponse& joinRoomResponse);
	static Buffer serializeResponse(const CreateRoomResponse& createRoomResponse);
	static Buffer serializeResponse(const HighScoreResponse& highScoreResponse);
	static Buffer serializeResponse(const GetPersonalStatsResponse& getPersonalStatsResponse);

private:
	static Buffer serialize(unsigned int msgCode, const std::string& content);
};
