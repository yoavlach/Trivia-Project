#include "JsonResponsePacketSerializer.h"
#include "json.hpp"
#include "BinaryConverter.h"
#include "MessageCodes.h"
#include "Helper.h"
#include <string>
#include <bitset>
#define CHAR_BYTE_SIZE 8

using json = nlohmann::json;

enum MSG_PARTS { CODE_BYTES = 1, DATA_LEN_BYTES = 4 };

/**
 * Serializes an error response into a binary buffer by placing the error message into a JSON object
 * @param errorResponse the error response object containing the error message
 * @return a binary buffer representing the serialized error response
 */
Buffer JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& errorResponse)
{
    json jsonMsg;
    jsonMsg["message"] = errorResponse.message;
    return serialize(FAILURE, jsonMsg.dump());
}

/**
 * Serializes a login response into a binary buffer by placing the status into a JSON object
 * @param loginResponse the login response object containing the status
 * @return a binary buffer representing the serialized login response
 */
Buffer JsonResponsePacketSerializer::serializeResponse(const LoginResponse& loginResponse)
{
    json jsonMsg;
    jsonMsg["status"] = loginResponse.status;
    return serialize(LOG_IN, jsonMsg.dump());
}

/**
 * Serializes a signup response into a binary buffer by placing the status into a JSON object
 * @param signupResponse the signup response object containing the status
 * @return a binary buffer representing the serialized signup response
 */
Buffer JsonResponsePacketSerializer::serializeResponse(const SignupResponse& signupResponse)
{
    json jsonMsg;
    jsonMsg["status"] = signupResponse.status;
    return serialize(SIGN_UP, jsonMsg.dump());
}

/**
 * Serializes a logout response into a binary buffer by placing the status into a JSON object
 * @param logoutResponse the logout response object containing the status
 * @return a binary buffer representing the serialized logout response
 */
Buffer JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& logoutResponse)
{
    json jsonMsg;
    jsonMsg["status"] = logoutResponse.status;
    return serialize(LOG_OUT, jsonMsg.dump());
}

/**
 * Serializes a get rooms response into a binary buffer by extracting room names and joining them into a single string inside a JSON object
 * @param getRoomResponse the get rooms response object containing a list of rooms
 * @return a binary buffer representing the serialized get rooms response
 */
Buffer JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& getRoomsResponse)
{
    json jsonMsg;
    std::vector<std::string> rooms;
    for (const auto& i : getRoomsResponse.rooms)
        rooms.push_back(Helper::formatRoomData(i));
    jsonMsg["rooms"] = Helper::join(rooms, ",");
    jsonMsg["status"] = getRoomsResponse.status;
    return serialize(GET_ROOMS, jsonMsg.dump());
}

/**
 * Serializes a get players in room response into a binary buffer by joining the player names into a single string inside a JSON object
 * @param getPlayersInRoomResponse the response object containing the list of players
 * @return a binary buffer representing the serialized get players in room response
 */
Buffer JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& getPlayersInRoomResponse)
{
    json jsonMsg;
    jsonMsg["PlayersInRoom"] = Helper::join(getPlayersInRoomResponse.players, ", ");
    return serialize(GET_PLAYERS_IN_ROOM, jsonMsg.dump());
}

/**
 * Serializes a join room response into a binary buffer by placing the status into a JSON object
 * @param joinRoomResponse the join room response object containing the status
 * @return a binary buffer representing the serialized join room response
 */
Buffer JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& joinRoomResponse)
{
    json jsonMsg;
    jsonMsg["status"] = joinRoomResponse.status;
    return serialize(JOIN_ROOM, jsonMsg.dump());
}

/**
 * Serializes a create room response into a binary buffer by placing the status into a JSON object
 * @param createRoomResponse the create room response object containing the status
 * @return a binary buffer representing the serialized create room response
 */
Buffer JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& createRoomResponse)
{
    json jsonMsg;
    jsonMsg["status"] = createRoomResponse.status;
    jsonMsg["id"] = createRoomResponse.roomId;
    return serialize(CREATE_ROOM, jsonMsg.dump());
}

/**
 * Serializes a high score response into a binary buffer by converting the high score to a string and placing it into a JSON object
 * @param highScoreResponse the high score response object containing the numeric high score
 * @return a binary buffer representing the serialized high score response
 */
Buffer JsonResponsePacketSerializer::serializeResponse(const HighScoreResponse& highScoreResponse)
{
    json jsonMsg;
    jsonMsg["HighScore"] = highScoreResponse.highScores;
    jsonMsg["status"] = highScoreResponse.status;
    return serialize(HIGH_SCORE, jsonMsg.dump());
}

/**
 * Serializes a personal stats response into a binary buffer by joining the statistics into a single string inside a JSON object
 * @param getPersonalStatsResponse the personal stats response object containing the user statistics
 * @return a binary buffer representing the serialized personal stats response
 */
Buffer JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatsResponse& getPersonalStatsResponse)
{
    json jsonMsg;
    jsonMsg["UserStatistics"] = Helper::join(getPersonalStatsResponse.statistics, ",");
    jsonMsg["status"] = getPersonalStatsResponse.status;
    return serialize(GET_STATISTICS, jsonMsg.dump());
}

/**
 * Constructs a binary message buffer by converting the message code content length and content itself into binary strings and appending them sequentially
 * @param msgCode the integer code representing the message type
 * @param content the JSON string content to be serialized
 * @return a binary buffer containing the fully constructed message
 */
Buffer JsonResponsePacketSerializer::serialize(unsigned int msgCode, const std::string& content)
{
    Buffer buffer;

    buffer.buffer.push_back(msgCode);

    int length = content.length();
    buffer.buffer.push_back(length % 256);
    buffer.buffer.push_back((length / 256) % 256);
    buffer.buffer.push_back((length / 65536) % 256);
    buffer.buffer.push_back((length / 16777216) % 256);

    for (char ch : content)
    {
        buffer.buffer.push_back(ch);
    }

    return buffer;
}