#include "CompressionException.h"

using namespace std;

CompressionException::CompressionException(const string& reason)
{
	mReason = reason;
}

const char* CompressionException::what() const
{
	return mReason.c_str();
}

ostream& operator<<(ostream& os, const CompressionException& ex)
{
	return os << ex.what();
}