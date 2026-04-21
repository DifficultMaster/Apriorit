#ifndef APRIORIT4_SHARED_INCLUDE_NETWORK_UNIX_SOCKET_EXCEPTION_H_
#define APRIORIT4_SHARED_INCLUDE_NETWORK_UNIX_SOCKET_EXCEPTION_H_

#include <string>
#include <stdexcept>

namespace apriorit4::shared::network
{
	class UnixSocketException : public std::runtime_error
	{
	public:
		explicit UnixSocketException(const std::string& message) : std::runtime_error(message) {}
	};
}

#endif // APRIORIT4_SHARED_INCLUDE_NETWORK_UNIX_SOCKET_EXCEPTION_H_