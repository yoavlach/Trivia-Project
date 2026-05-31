#include "Helper.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <string>
#include <exception>

#define MSG_CODE_SIZE 1
#define DATA_LEN_PART_SIZE 4

using std::string;
using std::vector;

vector<unsigned char> Helper::getPartFromSocket(const SOCKET sc, const int bytesNum, const int flags)
{
    if (bytesNum == 0)
    {
        return vector<unsigned char>();
    }

    vector<unsigned char> data(bytesNum);
    int totalRead = 0;

    while (totalRead < bytesNum)
    {
        int res = recv(sc, reinterpret_cast<char*>(data.data()) + totalRead, bytesNum - totalRead, flags);

        if (res == SOCKET_ERROR)
        {
            int err = WSAGetLastError();
            if (err == WSAECONNABORTED || err == WSAECONNRESET)
            {
                throw std::exception("Client disconnected");
            }
            if (err == WSAEWOULDBLOCK)
                continue;

            std::string s = "Error while receiving from socket: " + std::to_string(sc) + ". Error: " + std::to_string(WSAGetLastError());
            throw std::exception(s.c_str());
        }
        else if (res == 0)
        {
            throw std::exception("Client disconnected");
        }
        totalRead += res;
    }

    return data;
}

vector<unsigned char> Helper::getPartFromSocket(const SOCKET sc, const int bytesNum)
{
    return getPartFromSocket(sc, bytesNum, 0);
}

string Helper::getStringPartFromSocket(const SOCKET sc, const int bytesNum)
{
    vector<unsigned char> data = getPartFromSocket(sc, bytesNum, 0);
    return string(data.begin(), data.end());
}

void Helper::sendData(const SOCKET sc, const std::string& message)
{
    if (send(sc, message.c_str(), message.size(), 0) == INVALID_SOCKET)
        throw std::exception("Error while sending message to client");
}

std::vector<unsigned char> Helper::getMessage(const SOCKET sc)
{
    vector<unsigned char> header = getPartFromSocket(sc, MSG_CODE_SIZE + DATA_LEN_PART_SIZE);
    int dataLen = extractDataLen(header);
    vector<unsigned char> data = getPartFromSocket(sc, dataLen);

    vector<unsigned char> fullMessage;
    fullMessage.reserve(header.size() + data.size());
    fullMessage.insert(fullMessage.end(), header.begin(), header.end());
    fullMessage.insert(fullMessage.end(), data.begin(), data.end());

    return fullMessage;
}

int Helper::extractMessageCode(const std::vector<unsigned char>& message)
{
    if (message.size() >= MSG_CODE_SIZE)
    {
        return message[0];
    }
    return -1;
}

int Helper::extractDataLen(const std::vector<unsigned char>& message)
{
    if (message.size() >= MSG_CODE_SIZE + DATA_LEN_PART_SIZE)
    {
        int length = message[1] + (message[2] * 256) + (message[3] * 65536) + (message[4] * 16777216);
        return length;
    }
    return -1;
}

std::string Helper::join(const std::vector<std::string>& vec, const std::string& joinTxt)
{
    std::string formatted = "";
    int vectorSize = vec.size();
    for (int i = 0; i < vectorSize; i++)
    {
        formatted += vec[i];
        if (i + 1 < vectorSize)
            formatted += joinTxt;
    }
    return formatted;
}

std::string Helper::formatRoomData(const RoomData& roomData)
{
    return std::to_string(roomData.id) + "." + std::to_string(roomData.maxPlayers) + "." + roomData.name + "." 
    + std::to_string(roomData.numOfQuestionsInGame) + "." + std::to_string(roomData.roomStatus) + "." + std::to_string(roomData.timePerQuestion);
}
