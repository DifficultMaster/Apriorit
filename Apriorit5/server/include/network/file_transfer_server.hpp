#ifndef APRIORIT5_NETWORK_FILE_TRANSFER_SERVER_HPP
#define APRIORIT5_NETWORK_FILE_TRANSFER_SERVER_HPP

#include <grpcpp/grpcpp.h>
#include <string>

#include "file_transfer.grpc.pb.h"

namespace apriorit5::network
{
    class FileTransferServer final : public FileTransferService::Service
	{
	public:
		grpc::Status UploadFile(
			grpc::ServerContext* context,
			grpc::ServerReader<UploadFileRequest>* reader,
			UploadFileResponse* response) override;
	};

	namespace bootstrapper
	{
		void RunServer(const std::string& address);
	}	
}

#endif // APRIORIT5_NETWORK_FILE_TRANSFER_SERVER_HPP