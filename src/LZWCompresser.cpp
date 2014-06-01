#include "LZWCompresser.h"
#include "BitBuffer.h"
#include "CompressionManager.h"
#include "CompressionException.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <map>
#include <vector>

using namespace std;

LZWCompresser::LZWCompresser()
{
}

void LZWCompresser::compress(const std::string& fileIn, const std::string& fileOut)
{
	// Initializing dictionary:
	map<string, int> dictionary;
	for (unsigned int i = 0; i < 256; i++)
		dictionary[string(1, i)] = i;


	ifstream fin;
	int index = 256;
	string w;
	char k;
	list<int> result;

	fin.open(fileIn, ios::in);
	w = fin.get();
	while (fin.good())
	{
		k = fin.get();
		string wk = w + k;
		// If the key exists in the dictionary:
		if (dictionary.find(wk) != dictionary.end())
		{
			w = wk;
		}
		else
		{
			// Outputting code:
			result.push_back(dictionary[w]);

			// Adding new code to the dictionary:
			dictionary[wk] = index++;
			w = k;
		}
	}
	fin.close();

	// Outputting final code:
	result.push_back(dictionary[w]);

	// Write a compressed file:
	writeFile(fileOut, result);
}
void LZWCompresser::decompress(const std::string& fileIn, const std::string& fileOut)
{
	unsigned int fileSize = CompressionManager::getFileSize(fileIn);

	// Reading file and converting it to a bit buffer:
	BitBuffer* bitBuffer = new BitBuffer();
	unsigned char nBits = readFile(fileIn, bitBuffer);

	// Decompressing data from file:
	vector<int> result = vector<int>(fileSize * 8 / nBits);
	for (unsigned int i = 0; i < result.size(); i++)
		result[i] = bitBuffer->get(nBits);

	// Releasing buffer as it is no longer needed:
	delete bitBuffer;


	// Initializing dictionary:
	map<int, string> dictionary;
	for (unsigned int i = 0; i < 256; i++)
		dictionary[i] = string(1, i);

	ofstream fout;
	unsigned int index = 256;

	// Open file:
	fout.open(fileOut, ios::out);

	// Read current code:
	int current = result[0];
	// Outputting current code translation:
	fout << dictionary[current];
	for (unsigned int i = 1; i < result.size(); i++)
	{
		// Read next code:
		int next = result[i];
		// Get translation of the next code:
		string word = dictionary[next];
		// Outputting translation:
		fout << word;

		// Getting the first character in the next translation:
		char k = word[0];
		// Adding current translation + first character in the next translation to the dictionary:
		dictionary[index++] = dictionary[current] + k;
		current = next;
	}

	// Close file:
	fout.close();
}

void LZWCompresser::writeFile(const std::string& filename, const list<int>& result)
{
	// Finding the max index, so we can calculate the number of bits needed to represent all indices:
	int max = 0;
	list<int>::const_iterator it = result.begin();
	for (it = result.begin(); it != result.end(); it++)
		max = *it > max ? *it : max;

	unsigned char nBits = (unsigned char) (log((float) max) / log(2.0)) + 1;
	if (nBits <= 8 || nBits >= 16)
		throw CompressionException("File cannot be compressed using LZW algorithm");


	// Creating a bit buffer and output all data:
	BitBuffer bitBuffer;
	bitBuffer.initialize(result.size(), nBits);
	for (it = result.begin(); it != result.end(); it++)
		bitBuffer.add(*it, nBits);


	ofstream fout = ofstream(filename, ios::out | ios::binary);

	fout.put(nBits); // The file was compressed using nBits. This is important for decompressing
	fout << bitBuffer; // Writing all data to the file

	fout.close();
}
unsigned char LZWCompresser::readFile(const string& filename, BitBuffer* bitBuffer)
{
	int fileSize = CompressionManager::getFileSize(filename);

	ifstream fin = ifstream(filename, ios::in | ios::binary);

	unsigned char nBits = fin.get();
	if (nBits <= 8 || nBits >= 16)
		throw CompressionException("File cannot be decompressed using LZW algorithm");

	bitBuffer->initialize(fileSize - 1, nBits);
	fin >> *bitBuffer;

	fin.close();

	return nBits;
}