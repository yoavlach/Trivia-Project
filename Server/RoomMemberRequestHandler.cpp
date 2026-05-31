#include "RoomMemberRequestHandler.h"
#include "MessageCodes.h"
#include "MenuRequestHandler.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(const LoggedUser& user, RequestHandlerFactory& handlerFactory, const RoomData& metadata)
	: m_user(user), m_handlerFactory(handlerFactory), m_room(metadata)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	return requestInfo.id == LEAVE_ROOM_RESPONSE || requestInfo.id == GET_ROOM_STATE_RESPONSE;
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	RequestResult requestResult;
	switch (requestInfo.id)
	{
	case LEAVE_ROOM_RESPONSE:
		requestResult = leaveRoom(requestInfo);
		break;
	case GET_ROOM_STATE_RESPONSE:
		requestResult = getRoomState(requestInfo);
		break;
	}
	return requestResult;
}

RequestResult RoomMemberRequestHandler::leaveRoom(const RequestInfo& requestInfo)
{
	RequestResult requestResult;
	LeaveRoomResponse leaveRoomResponse{ SUCCESS };
	try
	{
		m_room.removeUser(m_user.getUsername());
		requestResult.newHandler = m_handlerFactory.createMenuRequestHandler(m_user);
	}
	catch (const std::exception& e)
	{
		leaveRoomResponse.status = FAILURE;
		requestResult.newHandler = nullptr;
	}
	return requestResult;
}

RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo& requestInfo)
{
	RequestResult requestResult;
	GetRoomStateResponse getRoomStateResponse;
	getRoomStateResponse.status = SUCCESS;
	try
	{
		RoomData roomData = m_room.getRoomData();
		getRoomStateResponse.players = m_room.getAllUsers();
		getRoomStateResponse.questionCount = roomData.numOfQuestionsInGame;
		getRoomStateResponse.questionTimeout = roomData.timePerQuestion;
		getRoomStateResponse.hasGameBegun
	}
}
