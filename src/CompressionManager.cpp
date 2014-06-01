#include "CompressionManager.h"
#include "LZWCompresser.h"
#include "Huffman.h"

#include <iostream>
#include <fstream>

using namespace std;

void CompressionManager::run(const std::string& fileToCompress, const std::string& compressedFile, const std::string& decompressedFile)
{
	int originalSize = getFileSize(fileToCompress);

	// Building filenames:
	string compressedLZW = compressedFile;
	compressedLZW.insert(compressedLZW.find_last_of('.'), "_lzw");
	string decompressedLZW = decompressedFile;
	decompressedLZW.insert(decompressedLZW.find_last_of('.'), "_lzw");

	string compressedHuff = compressedFile;
	compressedHuff.insert(compressedHuff.find_last_of('.'), "_huff");
	string decompressedHuff = decompressedFile;
	decompressedHuff.insert(decompressedHuff.find_last_of('.'), "_huff");

	cout << "Running LZW Compression..." << endl;
	LZWCompresser::compress(fileToCompress, compressedLZW);
	LZWCompresser::decompress(compressedLZW, decompressedLZW);
	int lzwSize = getFileSize(compressedLZW);
	float lzwRate = (float) lzwSize * 100 / (float) originalSize;

	cout << "Running Huffman Compression..." << endl;
	HuffmanCompressor huffmanCompressor;
	huffmanCompressor.compressFile(fileToCompress, compressedHuff);
	huffmanCompressor.decompressFile(compressedHuff, decompressedHuff);
	int huffSize = getFileSize(compressedHuff);
	float huffRate = (float) huffSize * 100 / (float) originalSize;


	// Outputting results:
	cout << endl << endl << "Original Size:\t" << originalSize << endl;

	cout << endl << "LZW Compression Results:" << endl;
	cout << "\tOutput size:\t" << lzwSize << endl;
	cout << "\tCompression:\t" << lzwRate << '%' << endl;

	cout << endl << "Huffman Compression Results:" << endl;
	cout << "\tOutput size:\t" << huffSize << endl;
	cout << "\tCompression:\t" << huffRate << '%' << endl;
}

int CompressionManager::getFileSize(const std::string& file)
{
	ifstream fin = ifstream(file, ios::binary | ios::ate);
	int size = (int) fin.tellg();
	fin.close();

	return size;
}