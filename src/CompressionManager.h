#pragma once

#include <string>

class CompressionManager
{
public:
	CompressionManager();
	void run(const std::string& fileToCompress, const std::string& compressedFile, const std::string& decompressedFile);
	static int getFileSize(const std::string& file);

private:
	void runLZW(const std::string& fileToCompress, const std::string& compressedFile, const std::string& decompressedFile);
	void runHuffman(const std::string& fileToCompress, const std::string& compressedFile, const std::string& decompressedFile);

private:
	int mOriginalSize;
	
	int mLzwSize;
	float mLzwRate;

	int mHuffmanSize;
	float mHuffmanRate;

	bool mLzwException;
	bool mHuffmanException;
};