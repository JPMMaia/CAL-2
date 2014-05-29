#include "CompressionManager.h"

#include <iostream>
#include <fstream>

using namespace std;

void CompressionManager::run(std::string fileIn, std::string fileOut)
{
	int originalSize = getFileSize(fileIn);

	cout << "Running LZW Compression..." << endl;
	lzwCompresser.compress(fileIn, fileOut);
	lzwCompresser.decompress(fileOut, "Text.txt");

	int lzwSize = getFileSize(fileOut);
	float lzwRate = (float) lzwSize * 100 / (float) originalSize;


	cout << endl << endl << "Original Size:\t" << originalSize << endl;

	cout << endl << "LZW Compression Results:" << endl;
	cout << "\tOutput size:\t" << lzwSize << endl;
	cout << "\tCompression:\t" << lzwRate << '%' << endl;

	huffmanCompressor.compressFile(fileIn, fileOut);
}

int CompressionManager::getFileSize(std::string file)
{
	ifstream fin = ifstream(file, ios::binary | ios::ate);
	int size = (int) fin.tellg();
	fin.close();

	return size;
}