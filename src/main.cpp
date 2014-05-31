#include "CompressionManager.h"

#include <iostream>
#include <conio.h>

using namespace std;

int main(int argc, const char* argv [])
{
	if (argc != 3)
	{
		cout << "Usage: compresser fileToCompress fileOut" << endl;
		return 0;
	}

	/*
	CompressionManager manager;
	manager.run(argv[1], argv[2]);
	*/

	HuffmanCompressor compressor;
	compressor.compressFile(argv[1], argv[2]);

	_getch();

	return 0;
}