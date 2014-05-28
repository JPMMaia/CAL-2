#include "LZWCompresser.h"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <math.h>

using namespace std;

LZWCompresser::LZWCompresser()
{
}

void LZWCompresser::compress(std::string fileIn, std::string fileOut)
{
	// Initializing dictionary:
	map<string, int> dictionary;
	for (unsigned int i = 0; i < 256; i++)
		dictionary[string(1, i)] = i;

	ifstream fin = ifstream(fileIn, ios::in);

	int index = 256;
	string w = "";
	char k;

	k = fin.get();
	while (fin.good())
	{
		string wk = w + k;
		// If the key exists in the dictionary:
		if (dictionary.find(wk) != dictionary.end())
		{
			w = wk;
		}
		else
		{
			// Outputting code:
			mResult.push_back(dictionary[w]);

			// Adding new code to the dictionary:
			dictionary[wk] = index++;
			w = k;
		}

		k = fin.get();
	}

	// Outputting final code:
	mResult.push_back(dictionary[w]);
	fin.close();

	// Write a compressed file:
	writeFile(fileOut);
}

void LZWCompresser::decompress(std::string fileIn, std::string fileOut)
{
	// Initializing dictionary:
	map<string, int> dictionary;
	for (unsigned int i = 0; i < 256; i++)
		dictionary[string(1, i)] = i;

	// Opening compressed file:
	ifstream fin = ifstream(fileIn, ios::in | ios::binary);

	// Reading the number of bits used for the compression process:
	unsigned char nBits = fin.get();

	/*
	01 02 03 04
	05 06 07 08
	09 10 11 12
	13 14 15 16
	17 18 19 20

	01 02 03 04 05
	06 07 08 09 10
	11 12 13 14 15
	*/

	int index = 0;
	char remainder = 0;
	list<int> result;

	while (fin.good())
	{
		unsigned short word;
		word = fin.get();
		word |= fin.get() << 8;

		index = (index + nBits) % 16;
		result.push_back((word & (0xFF >> 16 - index)) | remainder);
		remainder = word >> index;
	}

	fin.close();
}

void LZWCompresser::writeFile(std::string filename)
{
	// Finding the max index, so we can calculate the number of bits needed to represent all indices:
	int max = 0;
	list<int>::iterator it = mResult.begin();
	for (it = mResult.begin(); it != mResult.end(); it++)
	{
		max = *it > max ? *it : max;
	}

	unsigned char nBits = (unsigned char) (log(max) / log(2)) + 1;

	ofstream fout = ofstream(filename, ios::out | ios::binary);

	// The file was compressed using nBits. This is important for decompressing:
	fout.put(nBits);

	int index = 0;
	char out;
	char remainder = mResult.front();
	for (it = mResult.begin(); it != mResult.end(); it++)
	{
		out = ((*it << (nBits - index)) | remainder) & 0xFF;
		fout.put(out);

		index = (index + 8) % nBits;
		remainder = *it >> index;
	}

	if (remainder)
		fout.put(remainder);

	fout.close();
}