#include <iostream>

#include <filesystem>

#include "network/file_transfer_client.hpp"
#include "exceptions/network_exceptions.hpp"

int main(int arg_count, char** arg_values)
{
  if (arg_count != 3)
	{
		std::string exe_name = std::filesystem::path(arg_values[0]).filename().string();
		std::cerr << "Usage: " << exe_name << " <server_ip:port> <path_to_file>\n";
		return 1;
	}

	try
	{
        const std::string target_address = arg_values[1];
		const std::string filename = arg_values[2];

		apriorit5::network::FileTransferClient client(target_address);
		client.SendFile(filename);		
	}
	catch (const apriorit5::exceptions::FileIoException& ex)
	{
		std::cerr << ex.what() << "\n";
		return 1;
	}
	catch (const apriorit5::exceptions::GrpcNetworkException& ex)
	{
		std::cerr << ex.what() << "\n";
		return 1;
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Unhandled error: " << ex.what() << "\n";
		return 1;
	}

	std::cout << "File uploaded successfully\n";

	return 0;
}