#ifndef APRIORIT4_SHARED_INCLUDE_NETWORK_UNIX_SOCKET_SERVICE_H_
#define APRIORIT4_SHARED_INCLUDE_NETWORK_UNIX_SOCKET_SERVICE_H_

#include <string>

#include "network/unix_socket_exception.h"

namespace apriorit4::shared::network
{
	class UnixSocketService
	{
	public:
		UnixSocketService();
		explicit UnixSocketService(int raw_fd);
		~UnixSocketService();

		UnixSocketService(const UnixSocketService&) = delete;
		UnixSocketService& operator=(const UnixSocketService&) = delete;

		UnixSocketService(UnixSocketService&& other) noexcept;
		UnixSocketService& operator=(UnixSocketService&& other) noexcept;

		// Server
		void BindAndListen(int port);
		UnixSocketService Accept();

		// Client
		void Connect(const std::string& ip, int port);
		// Shared 
		bool Send(const std::string& message);
		std::string Receive();

		// Utility
		bool IsValid() const;
		void Close();

	private:
		int fd_;
	};
}

#endif // APRIORIT4_SHARED_INCLUDE_NETWORK_UNIX_SOCKET_SERVICE_H_