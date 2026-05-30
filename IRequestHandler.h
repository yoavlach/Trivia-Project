#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "Buffer.h"
#include <ctime>

class RequestInfo;
class RequestResult;

class IRequestHandler
{
public:
	virtual bool isRequestRelevant(const RequestInfo& requestInfo) = 0;
	virtual RequestResult handleRequest(const RequestInfo& requestInfo) = 0;
};