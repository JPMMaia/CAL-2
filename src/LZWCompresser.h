#pragma once

#include <map>
#include <string>
#include <list>

class LZWCompresser
{
public:
	LZWCompresser();

	void compress(std::string fileIn, std::string fileOut);
	void decompress(std::string fileIn, std::string fileOut);

private:
	void writeFile(std::string filename);

private:
	std::list<int> mResult;
};