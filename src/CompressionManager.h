#pragma once

#include "LZWCompresser.h"
#include "Huffman.h"

#include <string>

class CompressionManager
{
public:
	void run(std::string fileIn, std::string fileOut);


private:
	int getFileSize(std::string file);

private:
	LZWCompresser lzwCompresser;
	HuffmanCompressor huffmanCompressor;
};