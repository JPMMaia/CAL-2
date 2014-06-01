#pragma once

#include <string>

class CompressionManager
{
public:
	void run(const std::string& fileToCompress, const std::string& compressedFile, const std::string& decompressedFile);
	static int getFileSize(const std::string& file);
};