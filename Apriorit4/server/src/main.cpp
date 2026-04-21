#include <iostream>
#include <thread>

#include "network/unix_socket_service.h"

using namespace apriorit4::shared::network;

void HandleClient(UnixSocketService client)
{
	std::cout << "New client connected!\n";

	while (true)
	{
		std::string message = client.Receive();

		if (message.empty())
		{
			std::cout << "Client disconnected\n";
			break;
		}

		message.erase(message.find_last_not_of(" \n\r\t") + 1); // trims newline characters

		if (message == "disconnect")
		{
			std::cout << "Client request disconnect\n";
			break;
		}

		// sends 'word' back if 'hello' is received, otherwise parrots the message back
		if (message == "hello")
		{
			client.Send("world\n");
		}
		else
		{
			client.Send(message + "\n");
		}
	}
}

int main()
{
	try
	{
		int port = 8080;

		std::cout << "Enter port to listen on: ";
		std::cin >> port;

		UnixSocketService server;
		server.BindAndListen(port);

		std::cout << "\nServer is listening on port " << port << "..." << std::endl;	

		while (true)
		{
			UnixSocketService client = server.Accept();
			std::thread(HandleClient, std::move(client)).detach();
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