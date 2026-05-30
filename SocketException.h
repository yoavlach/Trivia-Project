#pragma once
#include <exception>

class SocketException : public std::exception
{
public:
	virtual const char* what() const;
};