#ifndef APRIORIT6_TASK2_DELETERS_SOCKET_DELETERS_HPP
#define APRIORIT6_TASK2_DELETERS_SOCKET_DELETERS_HPP

#include <functional>
#include <winsock2.h>

namespace apriorit6::task2::deleters
{
	// implementation is in .hpp to avoid unnecessary overhead

	// Lambda deleter
	constexpr auto LambdaDeleter = [](SOCKET* socket) noexcept
		{
			if (socket && *socket != INVALID_SOCKET)
			{
				closesocket(*socket);
			}

			delete socket;
		};

	// Functor deleter
	struct SocketFunctor
	{
		void operator()(SOCKET* socket) const noexcept
		{
			if (socket && *socket != INVALID_SOCKET)
			{
				closesocket(*socket);
			}

			delete socket;
		}
	};

	// std::function deleter
	inline std::function<void(SOCKET*)> FunctionDeleter = [](SOCKET* socket) noexcept
	{
		if (socket && *socket != INVALID_SOCKET)
		{
			closesocket(*socket);
		}

		delete socket;
	};
}

#endif // APRIORIT6_TASK2_DELETERS_SOCKET_DELETERS_HPP

