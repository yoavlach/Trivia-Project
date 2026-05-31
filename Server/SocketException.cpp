#include "SocketException.h"

/*
* throws a custom socket exception in case a function was given
* an invalid socket
* input: none
* output: socket exception content
*/
const char* SocketException::what() const
{
	return "Invalid Socket!";
}