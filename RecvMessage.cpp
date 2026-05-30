#include "RecvMessage.h"

/*
* Constructor for recover message with only the socket
* input: the socket with the client
* output: none
*/
RecvMessage::RecvMessage(const SOCKET sock)
{
	_sock = sock;
}

/*
* Constructor for recover message with the socket and the content
* of the message
* input: the socket with the client and the content of the message
* output: none
*/
RecvMessage::RecvMessage(const SOCKET sock, const std::string& content) : RecvMessage(sock)
{
	_content = content;
}

/*
* Gets the socket with the client
* input: none
* output: the socket with the client
*/
SOCKET RecvMessage::getSock()
{
	return _sock;
}

/*
* Gets the content of the message
* input: none
* output: the content of the message
*/
std::string& RecvMessage::getContent()
{
	return _content;
}