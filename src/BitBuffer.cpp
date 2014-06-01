#include "BitBuffer.h"

using namespace std;

BitBuffer::BitBuffer()
{
	mBuffer = 0;
}

BitBuffer::~BitBuffer()
{
	shutdown();
}

void BitBuffer::initialize(int nTotalBits)
{
	int bytes = nTotalBits / 8;
	if (nTotalBits % 8 != 0)
		bytes += 1;

	mBuffer = new vector<char>(bytes);
	mBitIndex = 0;
	mEof = false;
}

void BitBuffer::initialize(int nWords, unsigned char nBitsPerWord)
{
	int bytes = (nWords * nBitsPerWord) / 8;
	if ((nWords * nBitsPerWord) % 8 != 0)
		bytes += 1;

	mBuffer = new vector<char>(bytes);
	mBitIndex = 0;
	mEof = false;
}

void BitBuffer::shutdown()
{
	if (mBuffer)
	{
		delete mBuffer;
		mBuffer = 0;
	}
}

void BitBuffer::add(int data, unsigned char nBits)
{
	unsigned int vectorIndex = mBitIndex / 8;
	char word = (*mBuffer)[vectorIndex];
	unsigned int charIndex = mBitIndex % 8;

	for (unsigned int i = 0; i < nBits; i++)
	{
		char bit = (data >> (nBits - 1 - i)) & 0x01;
		(*mBuffer)[vectorIndex] |= bit << (7 - charIndex++);
		mBitIndex++;

		if (charIndex == 8)
		{
			charIndex = 0;
			vectorIndex++;
		}
	}
}

int BitBuffer::get(unsigned char nBits)
{
	int output = 0;

	unsigned int vectorIndex = mBitIndex / 8;
	char word = (*mBuffer)[vectorIndex];
	unsigned int charIndex = mBitIndex % 8;

	for (unsigned int i = 0; i < nBits; i++)
	{
		// 0000 1001 001
		char bit = ((*mBuffer)[vectorIndex] >> (7 - charIndex++)) & 0x01;
		output |= bit << (nBits - 1 - i);
		mBitIndex++;

		if (charIndex == 8)
		{
			charIndex = 0;
			vectorIndex++;
		}
	}

	if (vectorIndex == mBuffer->size() - 1)
		mEof = true;

	return output;
}

void BitBuffer::write(std::ostream& os) const
{
	vector<char>::const_iterator byte;
	for (byte = mBuffer->begin(); byte != mBuffer->end(); byte++)
		os.put(*byte);
}

void BitBuffer::read(std::istream& is)
{
	vector<char>::iterator it;

	it = mBuffer->begin();
	while (is.good())
		*it++ = is.get();
}

bool BitBuffer::eof() const
{
	return mEof;
}

ostream& operator<<(ostream& os, const BitBuffer& bitBuffer)
{
	bitBuffer.write(os);

	return os;
}

istream& operator>>(istream& is, BitBuffer& bitBuffer)
{
	bitBuffer.read(is);

	return is;
}