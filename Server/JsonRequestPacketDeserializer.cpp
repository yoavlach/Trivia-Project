#include "JsonRequestPacketDeserializer.h"
#include <iostream>
#include <string>
#include "BinaryConverter.h"
#include "Helper.h"

#define MSG_CODE_SIZE 1
#define CONTENT_LEN_SIZE 4

/**
 * Deserializes a login request by parsing the buffer and returning a login object containing the username and password
 * @param buffer the buffer containing the request data
 * @return the deserialized login request object
 */
LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const Buffer& buffer)
{
    json parsedMsgData = parseRequest(buffer).data;
    return LoginRequest{ parsedMsgData["username"], parsedMsgData["password"] };
}

/**
 * Deserializes a signup request by parsing the buffer and returning a signup object containing the username password and email
 * @param buffer the buffer containing the request data
 * @return the deserialized signup request object
 */
SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const Buffer& buffer)
{
    json parsedMsgData = parseRequest(buffer).data;
    return SignupRequest{ parsedMsgData["username"], parsedMsgData["password"], parsedMsgData["email"] };
}

/**
 * Deserializes a request to get players in a room by parsing the buffer
 * @param buffer the buffer containing the request data
 * @return the deserialized get players in room request object
 */
GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(const Buffer& buffer)
{
    return GetPlayersInRoomRequest{ parseRequest(buffer).data["roomId"] };
}

/**
 * Deserializes a join room request by parsing the buffer
 * @param buffer the buffer containing the request data
 * @return the deserialized join room request object
 */
JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const Buffer& buffer)
{
    return JoinRoomRequest{ parseRequest(buffer).data["roomId"] };
}

/**
 * Deserializes a create room request by parsing the buffer and returning a create room object with the room details
 * @param buffer the buffer containing the request data
 * @return the deserialized create room request object
 */
CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const Buffer& buffer)
{
    json parsedMsgData = parseRequest(buffer).data;
    return CreateRoomRequest{ parsedMsgData["roomName"], parsedMsgData["maxUsers"], parsedMsgData["questionCount"], parsedMsgData["answerTimeout"] };
}

/**
 * Parses a raw buffer into a general request object containing the message code and the json data
 * @param requestBuffer the buffer with the binary request content
 * @return the parsed general request object
 */
GeneralRequest JsonRequestPacketDeserializer::parseRequest(const Buffer& requestBuffer)
{
    GeneralRequest generalRequest;
    std::string data = "";

    int msgCode = Helper::extractMessageCode(requestBuffer.buffer);
    int dataLength = Helper::extractDataLen(requestBuffer.buffer);

    for (int i = MSG_CODE_SIZE + CONTENT_LEN_SIZE; i < MSG_CODE_SIZE + CONTENT_LEN_SIZE + dataLength; i++)
        data += requestBuffer.buffer[i];

    generalRequest.msgCode = msgCode;
    generalRequest.data = json::parse(data);

    return generalRequest;
}