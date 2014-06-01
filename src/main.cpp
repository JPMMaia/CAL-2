#include "CompressionManager.h"

#include <iostream>
#include <conio.h>

using namespace std;

int main(int argc, const char* argv [])
{
	if (argc != 4)
	{
		cout << "Usage: compresser fileToCompress compressedFile decompressedFile" << endl;
		return 0;
	}

	CompressionManager manager;
	manager.run(argv[1], argv[2], argv[3]);

	_getch();

	return 0;
}