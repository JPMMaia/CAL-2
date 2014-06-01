#include "CompressionManager.h"
#include "LZWCompresser.h"
#include "Huffman.h"
#include "CompressionException.h"

#include <iostream>
#include <fstream>

using namespace std;

CompressionManager::CompressionManager()
{
	mLzwException = false;
	mHuffmanException = false;
}

void CompressionManager::run(const std::string& fileToCompress, const std::string& compressedFile, const std::string& decompressedFile)
{
	mOriginalSize = getFileSize(fileToCompress);

	cout << "Running LZW Compression..." << endl;
	try
	{
		runLZW(fileToCompress, compressedFile, decompressedFile);
	}
	catch (const exception& e)
	{
		cout << "\tError: " << e.what() << endl;
		mLzwException = true;
	}

	cout << endl << "Running Huffman Compression..." << endl;
	try
	{
		runHuffman(fileToCompress, compressedFile, decompressedFile);
	}
	catch (const exception& e)
	{
		cout << "\tError: " << e.what() << endl;
		mHuffmanException = true;
	}

	// Outputting results:
	cout << endl << endl << "Original Size:\t" << mOriginalSize << endl;

	if (!mLzwException)
	{
		cout << endl << "LZW Compression Results:" << endl;
		cout << "\tOutput size:\t" << mLzwSize << endl;
		cout << "\tCompression:\t" << mLzwRate << '%' << endl;
	}

	if (!mHuffmanException)
	{
		cout << endl << "Huffman Compression Results:" << endl;
		cout << "\tOutput size:\t" << mHuffmanSize << endl;
		cout << "\tCompression:\t" << mHuffmanRate << '%' << endl;
	}
}

int CompressionManager::getFileSize(const std::string& file)
{
	ifstream fin = ifstream(file, ios::binary | ios::ate);
	int size = (int) fin.tellg();
	fin.close();

	return size;
}

void CompressionManager::runLZW(const std::string& fileToCompress, const std::string& compressedFile, const std::string& decompressedFile)
{
	// Building filenames:
	string compressedLZW = compressedFile;
	compressedLZW.insert(compressedLZW.find_last_of('.'), "_lzw");
	string decompressedLZW = decompressedFile;
	decompressedLZW.insert(decompressedLZW.find_last_of('.'), "_lzw");

	LZWCompresser::compress(fileToCompress, compressedLZW);
	LZWCompresser::decompress(compressedLZW, decompressedLZW);

	mLzwSize = CompressionManager::getFileSize(compressedLZW);
	mLzwRate = (float) mLzwSize * 100 / (float) mOriginalSize;
}
void CompressionManager::runHuffman(const std::string& fileToCompress, const std::string& compressedFile, const std::string& decompressedFile)
{
	// Building filenames:
	string compressedHuff = compressedFile;
	compressedHuff.insert(compressedHuff.find_last_of('.'), "_huff");
	string decompressedHuff = decompressedFile;
	decompressedHuff.insert(decompressedHuff.find_last_of('.'), "_huff");

	HuffmanCompressor huffmanCompressor;
	huffmanCompressor.compressFile(fileToCompress, compressedHuff);
	huffmanCompressor.decompressFile(compressedHuff, decompressedHuff);
	mHuffmanSize = getFileSize(compressedHuff);
	mHuffmanRate = (float) mHuffmanSize * 100 / (float) mOriginalSize;

	if (mHuffmanSize >= mOriginalSize)
		throw CompressionException("File cannot be compressed using Huffman algorithm");
}