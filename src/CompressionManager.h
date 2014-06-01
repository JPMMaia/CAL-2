#pragma once

#include "Huffman.h"
#include <string>

class CompressionManager
{
public:
	void run(std::string fileToCompress, std::string compressedFile, std::string decompressedFile);
	static int getFileSize(std::string file);
};