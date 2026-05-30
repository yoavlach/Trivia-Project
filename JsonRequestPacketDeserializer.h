#pragma once
#include "Requests.h"
#include "Buffer.h"
#include "json.hpp"

using json = nlohmann::json;

struct GeneralRequest
{
	int msgCode;
	json data;
};

class JsonRequestPacketDeserializer
{
public:
	static LoginRequest deserializeLoginRequest(const Buffer& buffer);
	static SignupRequest deserializeSignupRequest(const Buffer& buffer);
	static GetPlayersInRoomRequest deserializeGetPlayersRequest(const Buffer& buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(const Buffer& buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(const Buffer& buffer);

private:
	static GeneralRequest parseRequest(const Buffer& requestBuffer);
};

