#pragma once

#include <string>
#include <list>

class BitBuffer;

class LZWCompresser
{
private:
	LZWCompresser();

public:
	static void compress(std::string fileIn, std::string fileOut);
	static void decompress(std::string fileIn, std::string fileOut);

private:
	static void writeFile(std::string filename, const std::list<int>& result);
	static unsigned char readFile(std::string filename, BitBuffer* bitBuffer);
};