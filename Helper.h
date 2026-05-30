#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <WinSock2.h>
#include "Room.h"

using namespace std;

class Helper
{
public:
	static string getStringPartFromSocket(const SOCKET sc, const int bytesNum);
	static void sendData(const SOCKET sc, const std::string& message);
	static std::vector<unsigned char> getMessage(const SOCKET sc);
	static int extractMessageCode(const std::vector<unsigned char>& message);
	static int extractDataLen(const std::vector<unsigned char>& message);
	static std::string join(const std::vector<std::string>& vec, const std::string& joinTxt);
	static std::string formatRoomData(const RoomData& roomData);

private:
	static vector<unsigned char> getPartFromSocket(const SOCKET sc, const int bytesNum, const int flags);
	static vector<unsigned char> getPartFromSocket(const SOCKET sc, const int bytesNum);

};


#ifdef _DEBUG // vs add this define in debug mode
// Q: why do we need traces ?
// A: traces are a nice and easy way to detect bugs without even debugging
// or to understand what happened in case we miss the bug in the first time
#define TRACE(msg, ...) printf(msg "\n", __VA_ARGS__);
// for convenient reasons we did the traces in stdout
// at general we would do this in the error stream like that
// #define TRACE(msg, ...) fprintf(stderr, msg "\n", __VA_ARGS__);

#else // we want nothing to be printed in release version
#define TRACE(msg, ...) printf(msg "\n", __VA_ARGS__);
#define TRACE(msg, ...) // do nothing
#endif