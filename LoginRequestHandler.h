#pragma once
#include "IRequestHandler.h"
#include "MessageCodes.h"

class RequestHandlerFactory;

class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler(RequestHandlerFactory& handlerFactory);
	bool isRequestRelevant(const RequestInfo& requestInfo) override;
	RequestResult handleRequest(const RequestInfo& requestInfo) override;
private:
	RequestResult login(const RequestInfo& requestInfo);
	RequestResult signup(const RequestInfo& requestInfo);
	RequestHandlerFactory& m_handlerFactory;
};