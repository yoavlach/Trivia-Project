#include "WSAInitializer.h"
#include <exception>

/*
* WSAInitializer constructor and initializes the WSA
*/
WSAInitializer::WSAInitializer()
{
	WSADATA wsa_data = { };
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) 
		throw std::exception("WSAStartup Failed");
}

/*
* WSAInitializer destructor and closes the WSA
*/
WSAInitializer::~WSAInitializer()
{
	try
	{
		WSACleanup();
	}
	catch (...) {}
}
