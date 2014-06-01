#pragma once

#include <string>
#include <list>

class BitBuffer;

class LZWCompresser
{
private:
	LZWCompresser();

public:
	static void compress(const std::string& fileIn, const std::string& fileOut);
	static void decompress(const std::string& fileIn, const std::string& fileOut);

private:
	static void writeFile(const std::string& filename, const std::list<int>& result);
	static unsigned char readFile(const std::string& filename, BitBuffer* bitBuffer);
};