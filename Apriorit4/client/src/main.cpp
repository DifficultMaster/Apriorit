#include <iostream>
#include <thread>

#include "network/unix_socket_service.h"

using namespace apriorit4::shared::network;

int main()
{
	try
	{
		std::string ip = "127.0.0.1";
		int port = 8080;

		std::cout << "Enter ip to connect to: ";
		std::cin >> ip;

		std::cout << "\nEnter port to connect to: ";
		std::cin >> port;

		UnixSocketService client;
		client.Connect(ip, port);

		std::cout << "\nClient is connected to " << ip << ":" << port << "...\n";
		std::cout << "Type 'disconnect' to end exit.\n";

		std::string message;
		while (true)
		{
			std::cout << "> ";
			std::getline(std::cin, message);

			if (message.empty()) continue;

			client.Send(message);

			if (message == "disconnect")
			{
				std::cout << "Disconnecting...\n";
				break;
			}

			std::string response = client.Receive();

			if (response.empty())
			{
				std::cout << "Server disconnected\n";
				break;
			}

			std::cout << "Server: " << response;
		}
	}
	catch (const UnixSocketException& ex)
	{
		std::cerr << "Network error: " << ex.what() << std::endl;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Unhandled error: " << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cerr << "Unknown error" << std::endl;
	}
}