#pragma once

#include <vector>
#include <fstream>

class BitBuffer
{
public:
	BitBuffer();
	~BitBuffer();

	void initialize(int nWords, unsigned char nBitsPerWord);
	void shutdown();

	void add(int data, unsigned char nBits);
	int get(unsigned char nBits);

	void write(std::ostream& os) const;
	void read(std::istream& is);

	bool eof() const;

private:
	bool mEof;
	unsigned int mBitIndex;
	std::vector<char>* mBuffer;
};

std::ostream& operator<<(std::ostream& os, const BitBuffer& bitBuffer);
std::istream& operator>>(std::istream& is, BitBuffer& bitBuffer);