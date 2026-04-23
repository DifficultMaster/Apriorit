#ifndef APRIORIT5_NETWORK_EXCEPTIONS_HPP
#define APRIORIT5_NETWORK_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace apriorit5::exceptions
{
	class GrpcNetworkException : public std::runtime_error
	{
	public:
		explicit GrpcNetworkException(const std::string& message) : std::runtime_error("Network error: " + message) {}
	};

	class FileIoException : public std::runtime_error
	{
	public:
		explicit FileIoException(const std::string& message) : std::runtime_error("File I/O error: " + message) {}
	};
}

#endif // APRIORIT5_NETWORK_EXCEPTIONS_HPP