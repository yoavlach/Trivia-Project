#include "MenuRequestHandler.h"
#include "MessageCodes.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "LoginRequestHandler.h"
#include <iostream>
#include <string>

/**
 * Constructs a new MenuRequestHandler object
 * @param user the currently logged in user associated with this handler
 * @param handlerFactory a reference to the factory used to generate subsequent request handlers
 */
MenuRequestHandler::MenuRequestHandler(const LoggedUser& user, RequestHandlerFactory& handlerFactory) : m_user(user), m_handlerFactory(handlerFactory)
{
}

/**
 * Checks if the incoming request is relevant to the menu state
 * @param requestInfo the struct containing the request details
 * @return true if the request is relevant false otherwise
 */
bool MenuRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
    return requestInfo.id == CREATE_ROOM || requestInfo.id == GET_ROOMS || requestInfo.id == GET_PLAYERS_IN_ROOM || requestInfo.id == JOIN_ROOM || requestInfo.id == GET_STATISTICS || requestInfo.id == LOG_OUT || requestInfo.id == HIGH_SCORE;
}

/**
 * Handles requests when the user is in the menu state by routing them to the appropriate function
 * @param requestInfo the struct containing the request details
 * @return the request result object containing the next handler and the response
 */
RequestResult MenuRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    RequestResult requestResult;
    switch (requestInfo.id)
    {
    case CREATE_ROOM:
        requestResult = createRoom(requestInfo);
        break;
    case GET_ROOMS:
        requestResult = getRooms(requestInfo);
        break;
    case GET_PLAYERS_IN_ROOM:
        requestResult = getPlayersInRoom(requestInfo);
        break;
    case JOIN_ROOM:
        requestResult = joinRoom(requestInfo);
        break;
    case GET_STATISTICS:
        requestResult = getPersonalStats(requestInfo);
        break;
    case LOG_OUT:
        requestResult = signout();
        break;
    case HIGH_SCORE:
        requestResult = getHighScore(requestInfo);
        break;
    }
    return requestResult;
}

/**
 * Handles the signout request by creating a new login request handler
 * @param requestInfo the struct containing the request details
 * @return the request result object containing the new login handler
 */
RequestResult MenuRequestHandler::signout()
{
    RequestResult requestResult;
    LogoutResponse logoutResponse{ SUCCESS };
    try
    {
        m_handlerFactory.getRoomManager().removeUserFromRooms(m_user.getUsername());
        m_handlerFactory.getLoginManager().logout(m_user.getUsername());
    }
    catch(const std::exception & e)
    {
        logoutResponse.status = FAILURE;
    }
    requestResult.response = JsonResponsePacketSerializer::serializeResponse(logoutResponse);
    requestResult.newHandler = m_handlerFactory.createLoginRequestHandler();
    return requestResult;
}

/**
 * Retrieves all available rooms from the room manager and serializes the response
 * @param requestInfo the struct containing the request details
 * @return the request result object containing the serialized rooms response
 */
RequestResult MenuRequestHandler::getRooms(const RequestInfo& requestInfo)
{
    RequestResult requestResult;
    GetRoomsResponse getRoomsResponse;
    getRoomsResponse.status = SUCCESS;
    try
    {
        getRoomsResponse.rooms = m_handlerFactory.getRoomManager().getRooms();
    }
    catch (const std::exception& e)
    {
        getRoomsResponse.status = FAILURE;
    }
    requestResult.newHandler = nullptr;
    requestResult.response = JsonResponsePacketSerializer::serializeResponse(getRoomsResponse);
    return requestResult;
}

/**
 * Retrieves all players in a specific room by deserializing the request and querying the room manager
 * @param requestInfo the struct containing the request details
 * @return the request result object containing the serialized players response
 */
RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo& requestInfo)
{
    RequestResult requestResult;
    try
    {
        GetPlayersInRoomRequest getPlayersInRoomRequest = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(Buffer{ requestInfo.buffer });
        Room& room = m_handlerFactory.getRoomManager().getRoom(getPlayersInRoomRequest.roomId);
        requestResult.response = JsonResponsePacketSerializer::serializeResponse(GetPlayersInRoomResponse{ room.getAllUsers() });
    }
    catch (const std::exception& e)
    {
        requestResult.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ e.what() });
    }
    requestResult.newHandler = nullptr;
    return requestResult;
}

/**
 * Retrieves the personal statistics of the logged in user from the database
 * @param requestInfo the struct containing the request details
 * @return the request result object containing the serialized statistics response
 */
RequestResult MenuRequestHandler::getPersonalStats(const RequestInfo& requestInfo)
{
    RequestResult requestResult;
    GetPersonalStatsResponse getPersonalStatsResponse;
    IDataBase* database = m_handlerFactory.getDatabase();
    std::string username = m_user.getUsername();
    getPersonalStatsResponse.status = SUCCESS;
    try
    {
        getPersonalStatsResponse.statistics = m_handlerFactory.getStatisticsManager().getUserStatistics(m_user.getUsername());
        requestResult.response = JsonResponsePacketSerializer::serializeResponse(getPersonalStatsResponse);
    }
    catch (const std::exception& e)
    {
        getPersonalStatsResponse.status = FAILURE;
    }
    requestResult.newHandler = nullptr;
    return requestResult;
}

/**
 * Retrieves the high score of the logged in user from the database
 * @param requestInfo the struct containing the request details
 * @return the request result object containing the serialized high score response
 */
RequestResult MenuRequestHandler::getHighScore(const RequestInfo& requestInfo)
{
    RequestResult requestResult;
    HighScoreResponse highScoreResponse;
    highScoreResponse.status = SUCCESS;
    try
    {
        highScoreResponse.highScores = m_handlerFactory.getDatabase()->getHighScores();
    }
    catch (const std::exception& e)
    {
        highScoreResponse.status = FAILURE;
    }
    requestResult.newHandler = nullptr;
    requestResult.response = JsonResponsePacketSerializer::serializeResponse(highScoreResponse);
    return requestResult;
}

/**
 * Handles the join room request by adding the logged in user to the specified room
 * @param requestInfo the struct containing the request details
 * @return the request result object containing the serialized join room response
 */
RequestResult MenuRequestHandler::joinRoom(const RequestInfo& requestInfo)
{
    RequestResult requestResult;
    JoinRoomResponse joinRoomResponse{ SUCCESS };
    if (m_user.getIsInRoom())
        joinRoomResponse.status = USER_ALREADY_IN_ROOM;
    else
    {
        try
        {
            JoinRoomRequest joinRoomRequest = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(Buffer{ requestInfo.buffer });
            m_handlerFactory.getRoomManager().getRoom(joinRoomRequest.roomId).addUser(m_user.getUsername());
        }
        catch (const std::exception& e)
        {
            joinRoomResponse.status = FAILURE;
        }
    }
    if (joinRoomResponse.status == SUCCESS)
        m_user.setIsInRoom(true);
    requestResult.response = JsonResponsePacketSerializer::serializeResponse(joinRoomResponse);
    requestResult.newHandler = nullptr;
    return requestResult;
}

/**
 * Handles the create room request by creating a new room using the room manager
 * @param requestInfo the struct containing the request details
 * @return the request result object containing the serialized create room response
 */
RequestResult MenuRequestHandler::createRoom(const RequestInfo& requestInfo)
{
    RequestResult requestResult;
    CreateRoomResponse createRoomResponse{ SUCCESS, m_handlerFactory.getRoomManager().getNextFreeRoomID() };
    CreateRoomRequest createRoomRequest = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(Buffer{ requestInfo.buffer });
    RoomData roomData{
        0, //no need to set id since its being changed in the createRoom function to the next id available
        createRoomRequest.roomName,
        createRoomRequest.maxUsers,
        createRoomRequest.questionCount,
        createRoomRequest.answerTimeout,
        ACTIVE
    };
    try
    {
        m_handlerFactory.getRoomManager().createRoom(m_user, roomData);
    }
    catch (const std::exception& e)
    {
        createRoomResponse.status = FAILURE;
    }
    if (createRoomResponse.status == SUCCESS)
        m_user.setIsInRoom(true);
    requestResult.newHandler = nullptr;
    requestResult.response = JsonResponsePacketSerializer::serializeResponse(createRoomResponse);
    return requestResult;
}