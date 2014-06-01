#include <exception>
#include <string>
#include <ostream>

class CompressionException : public std::exception
{
public:
	CompressionException(const std::string& reason);
	const char* what() const;

private:
	std::string mReason;
};

std::ostream& operator<<(std::ostream& os, const CompressionException& ex);