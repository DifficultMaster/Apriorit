#include <iostream>
#include <filesystem>
#include <string>

#include "network/file_transfer_server.hpp"

int main(int arg_count, char** arg_values)
{
  if (arg_count != 2)
	{
		std::string exe_name = std::filesystem::path(arg_values[0]).filename().string();
		std::cerr << "Usage: " << exe_name << " <ip:port>\n";
		return 1;
	}

	try
	{
		const std::string full_address = arg_values[1];
	
		std::cout << "\nAttempting to run server...\n";
		apriorit5::network::bootstrapper::RunServer(full_address);
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Unhandled error: " << ex.what() << "\n";
		return 1;
	}

	return 0;
}