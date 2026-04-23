#include "network/file_transfer_server.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <thread>

namespace apriorit5::network
{
	grpc::Status FileTransferServer::UploadFile(grpc::ServerContext* context, grpc::ServerReader<UploadFileRequest>* reader, UploadFileResponse* response)
	{
		UploadFileRequest request;
		std::ofstream outfile;
		bool metadata_received = false;
		bool data_received = false;

		while (reader->Read(&request))
		{
			switch (request.request_case())
			{
				case UploadFileRequest::kMetadata:
				{
					if (metadata_received || data_received)
					{
						return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Received metadata after data");
					}

					std::string filename = "server_incoming_" + request.metadata().filename();
					outfile.open(filename, std::ios::binary);

					if (!outfile.is_open())
					{
						return grpc::Status(grpc::StatusCode::INTERNAL, "Failed to open file for writing");
					}

					metadata_received = true;
					break;
				}

				case UploadFileRequest::kData:
				{
                    if (!metadata_received)
                    {
                    	return grpc::Status(grpc::StatusCode::FAILED_PRECONDITION, "Received data before metadata");
                    }

					const std::string& data = request.data().data();
					outfile.write(data.c_str(), data.size());
					if (!outfile)
					{
						return grpc::Status(grpc::StatusCode::INTERNAL, "Failed to write uploaded data");
					}

					data_received = true;

					break;
				}

				case UploadFileRequest::REQUEST_NOT_SET:
				default:
				{
					return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Empty message received");
				}
			}
		}

		if (!metadata_received)
		{
			return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "No metadata received");
		}
		if (!data_received)
		{
			return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "No file data received");
		}
		if (!outfile.flush())
		{
			return grpc::Status(grpc::StatusCode::INTERNAL, "Failed to flush uploaded file");
		}

		response->set_success(true);
		response->set_message("File uploaded successfully");

		return grpc::Status::OK;
	}

	namespace bootstrapper
	{
		void RunServer(const std::string& address)
		{
			FileTransferServer service;
			grpc::ServerBuilder builder;
			const int pollers = std::max(2u, std::thread::hardware_concurrency());

			builder.AddListeningPort(address, grpc::InsecureServerCredentials());
			builder.RegisterService(&service);
			builder.SetSyncServerOption(grpc::ServerBuilder::SyncServerOption::MIN_POLLERS, pollers);
			builder.SetSyncServerOption(grpc::ServerBuilder::SyncServerOption::MAX_POLLERS, pollers);

			std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
			if (!server)
			{
				throw std::runtime_error("Failed to start gRPC server");
			}
			std::cout << "Server listening on " << address << "\n";
			std::cout << "Press Enter to shut down...\n";

			std::cin.get();

			std::cout << "Shutting down...\n";

			server->Shutdown();
			server->Wait();

			std::cout << "Server terminated.\n";
		}
	}
}


