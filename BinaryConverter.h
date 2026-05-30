#pragma once
#include <iostream>

class BinaryConverter
{
public:
	static std::string binaryToStr(const std::string& binary);
	static std::string strToBinary(const std::string& str);
	static int binaryToInt(const std::string& binary);
	static std::string intToBinary(int num);
	static std::string intToFourByteBinary(int num);
};

