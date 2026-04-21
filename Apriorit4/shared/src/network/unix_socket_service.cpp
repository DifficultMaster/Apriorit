#include "network/unix_socket_service.h"

#include <iostream>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>

#include "network/unix_socket_exception.h"

namespace apriorit4::shared::network
{
	constexpr int BUFFER_SIZE = 1024;

	UnixSocketService::UnixSocketService()
	{
		fd_ = socket(AF_INET, SOCK_STREAM, 0);
       if (fd_ < 0)
		{
			throw UnixSocketException("Failed to create socket: " + std::string(std::strerror(errno)));
		}
	}

	UnixSocketService::UnixSocketService(int raw_fd) : fd_(raw_fd) {}

	UnixSocketService::~UnixSocketService()
	{
		Close();
	}

	UnixSocketService::UnixSocketService(UnixSocketService&& other) noexcept : fd_(other.fd_)
	{
		other.fd_ = -1;
	}

	UnixSocketService& UnixSocketService::operator=(UnixSocketService&& other) noexcept
	{
		if (this != &other)
		{
			Close();
			fd_ = other.fd_;
			other.fd_ = -1;
		}

		return *this;
	}

	void UnixSocketService::BindAndListen(int port)
	{
		if (!IsValid()) throw UnixSocketException("Invalid socket");

		sockaddr_in server_address{};
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = INADDR_ANY;
		server_address.sin_port = htons(port);

		if (bind(fd_, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address)) < 0)
		{
			throw UnixSocketException("Failed to bind on port" + std::to_string(port) + ": " + std::string(std::strerror(errno)));
		}

		if (listen(fd_, SOMAXCONN) < 0)
		{	
			throw UnixSocketException("Failed to listen on port" + std::to_string(port) + ": " + std::string(std::strerror(errno)));
		}
	}

	UnixSocketService UnixSocketService::Accept()
	{
		if (!IsValid()) throw UnixSocketException("Invalid socket");

		sockaddr_in client_address{};
		socklen_t client_address_len = sizeof(client_address);
		int client_fd = accept(fd_, reinterpret_cast<struct sockaddr*>(&client_address), &client_address_len);

		if (client_fd < 0)
		{
			throw UnixSocketException("Failed to accept connection: " + std::string(std::strerror(errno)));
		}

		return UnixSocketService(client_fd);
	}

	void UnixSocketService::Connect(const std::string& ip, int port)
	{
		if (!IsValid()) throw UnixSocketException("Invalid socket");

		sockaddr_in server_address{};
		server_address.sin_family = AF_INET;
		server_address.sin_port = htons(port);

		if (connect(fd_, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address)) < 0)
		{
			throw UnixSocketException("Failed to connect to " + ip + ":" + std::to_string(port) + ": " + std::string(std::strerror(errno)));
		}
	}

	bool UnixSocketService::Send(const std::string& message)
	{
		if (!IsValid()) throw UnixSocketException("Invalid socket");
		return send(fd_, message.c_str(), message.length(), 0) >= 0;
	}

	std::string UnixSocketService::Receive()
	{
		if (!IsValid()) throw UnixSocketException("Invalid socket");

		std::string buffer(BUFFER_SIZE, '\0');
		ssize_t bytes_read = read(fd_, buffer.data(), buffer.size());
		
		if (bytes_read < 0)
		{
			return "";
		}

		buffer.resize(bytes_read);
		return buffer;
	}

	bool UnixSocketService::IsValid() const
	{
		return fd_ >= 0;
	}

	void UnixSocketService::Close()
	{
     if (IsValid())
		{
			close(fd_);
			fd_ = -1; // mark as closed
		}
	}
}
