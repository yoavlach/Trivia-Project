#include "BinaryConverter.h"
#include <bitset>
#include <string>

#define CHAR_BYTE_SIZE 8
#define BIN_BASE 2

/*
* Converts binary string to string by converting each byte
* to int and then making the current char the ascii value of the int
* input: the binary string
* output: the string
*/
std::string BinaryConverter::binaryToStr(const std::string& binary)
{
    std::string str = "", currBinCh = "";
    for (int i = 0; i < binary.length(); i++)
    {
        if ((i % CHAR_BYTE_SIZE == 0 && i != 0) || i == binary.length() - 1)
        {
            if (i == binary.length() - 1)
                currBinCh += binary[i];
            str += (char)binaryToInt(currBinCh);
            currBinCh = "";
        }
        currBinCh += binary[i];
    }
    return str;
}

/*
* Converts string to binary string by converting each
* char to its binary byte value
* input: the string to convert
* output: the converted binary string
*/
std::string BinaryConverter::strToBinary(const std::string& str)
{
	std::string binary = "";
	for (char ch : str)
		binary += std::bitset<CHAR_BYTE_SIZE>(ch).to_string();
	return binary;
}

/*
* Converts binary string to int with std::stoi
* input: the binary string
* output: converted int value*/
int BinaryConverter::binaryToInt(const std::string& binary)
{
    return std::stoi(binary, nullptr, BIN_BASE);
}

/*
* Converts int to binary with std::bitset
* input: the number to convert
* output: the binary string
*/
std::string BinaryConverter::intToBinary(int num)
{
    return std::bitset<CHAR_BYTE_SIZE>(num).to_string();
}

/*
* Converts int to 4 byte binary with std::bitset but with
* 4 bytes instead of one
* input: the number to convert
* output: the binary string
*/
std::string BinaryConverter::intToFourByteBinary(int num)
{
    return std::bitset<CHAR_BYTE_SIZE * 4>(num).to_string();
}
