#pragma once

#include <string>
#include <vector>
#include <Windows.h>

class RecvMessage
{
public:
	RecvMessage(const SOCKET sock);

	RecvMessage(const SOCKET sock, const std::string& content);

	SOCKET getSock();

	std::string& getContent();

private:
	SOCKET _sock;
	std::string _content;
};

