#include <iostream>
#include "LoginRequestHandler.h"
#include "BinaryConverter.h"
#include "MessageCodes.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include "Responses.h"
#include "Requests.h"
#include "MenuRequestHandler.h"
#include "RequestHandlerFactory.h"

#define MSG_CODE_SIZE 8

/**
 * Initializes the login request handler
 *  Sets the handler factory reference
 * @param handlerFactory the factory used to create handlers and access managers
 */
LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& handlerFactory) : m_handlerFactory(handlerFactory)
{
}

/**
 * Checks if the message code is relevant to this handler
 *  Only login or signup requests are considered relevant for this handler
 * @param requestInfo the request info containing the message code
 * @return true if the request is a login or signup false otherwise
 */
bool LoginRequestHandler::isRequestRelevant(const RequestInfo& requestInfo)
{
	return requestInfo.id == LOG_IN || requestInfo.id == SIGN_UP;
}

/**
 * Handles a request by routing it to the appropriate function based on its code
 *  Returns an error response if the message code is unknown
 * @param requestInfo the request information to be handled
 * @return a struct containing the response buffer and the next handler
 */
RequestResult LoginRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	RequestResult requestResult;
	switch (requestInfo.id)
	{
	case LOG_IN:
		requestResult = login(requestInfo);
		break;
	case SIGN_UP:
		requestResult = signup(requestInfo);
		break;
	default:
		requestResult.response = JsonResponsePacketSerializer::serializeResponse(ErrorResponse{ "Unknown message code" });
		requestResult.newHandler = nullptr;
		break;
	}
	return requestResult;
}

/**
 * Processes a login request by deserializing it and checking the credentials
 *  Sets the new handler to a MenuRequestHandler if the login is successful
 * @param requestInfo the request information containing the login buffer
 * @return the result containing the response buffer and the next handler
 */
RequestResult LoginRequestHandler::login(const RequestInfo& requestInfo)
{
	RequestResult requestResult;
	LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(Buffer{ requestInfo.buffer });
	SuccessfulLoginInfo response = m_handlerFactory.getLoginManager().login(loginRequest.username, loginRequest.password);
	requestResult.successfulLoginInfo = response;
	requestResult.response = JsonResponsePacketSerializer::serializeResponse(LoginResponse{ response.success });
	requestResult.newHandler = response.success == SUCCESS ? m_handlerFactory.createMenuRequestHandler(LoggedUser{loginRequest.username}) : nullptr;
	return requestResult;
}

/**
 * Processes a signup request by deserializing it and registering the new user
 *  Sets the new handler to a MenuRequestHandler if the signup is successful
 * @param requestInfo the request information containing the signup buffer
 * @return the result containing the response buffer and the next handler
 */
RequestResult LoginRequestHandler::signup(const RequestInfo& requestInfo)
{
	RequestResult requestResult;
	SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignupRequest(Buffer{ requestInfo.buffer });
	SuccessfulLoginInfo response = m_handlerFactory.getLoginManager().signup(signupRequest.username, signupRequest.password, signupRequest.email);
	requestResult.successfulLoginInfo = response;
	requestResult.response = JsonResponsePacketSerializer::serializeResponse(SignupResponse{ response.success });
	requestResult.newHandler = response.success == SUCCESS ? m_handlerFactory.createMenuRequestHandler(LoggedUser{signupRequest.username}) : nullptr;
	return requestResult;
}