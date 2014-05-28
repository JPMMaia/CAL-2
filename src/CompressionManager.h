#pragma once

#include "LZWCompresser.h"

#include <string>

class CompressionManager
{
public:
	void run(std::string fileIn, std::string fileOut);


private:
	int getFileSize(std::string file);

private:
	LZWCompresser lzwCompresser;

};