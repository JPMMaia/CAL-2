#include "CompressionManager.h"
#include "LZWCompresser.h"
#include "Huffman.h"

#include <iostream>
#include <fstream>

using namespace std;

void CompressionManager::run(std::string fileToCompress, std::string compressedFile, std::string decompressedFile)
{
	int originalSize = getFileSize(fileToCompress);

	cout << "Running LZW Compression..." << endl;
	LZWCompresser::compress(fileToCompress, compressedFile);
	LZWCompresser::decompress(compressedFile, decompressedFile);

	int lzwSize = getFileSize(compressedFile);
	float lzwRate = (float) lzwSize * 100 / (float) originalSize;


	cout << "Running Huffman Compression..." << endl;
	HuffmanCompressor huffman;
	huffman.compressFile(fileToCompress, compressedFile);
	// Falta o decompress file



	cout << endl << endl << "Original Size:\t" << originalSize << endl;

	cout << endl << "LZW Compression Results:" << endl;
	cout << "\tOutput size:\t" << lzwSize << endl;
	cout << "\tCompression:\t" << lzwRate << '%' << endl;

	
}

int CompressionManager::getFileSize(std::string file)
{
	ifstream fin = ifstream(file, ios::binary | ios::ate);
	int size = (int) fin.tellg();
	fin.close();

	return size;
}