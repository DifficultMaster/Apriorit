#ifndef APRIORIT5_NETWORK_FILE_TRANSFER_CLIENT_HPP
#define APRIORIT5_NETWORK_FILE_TRANSFER_CLIENT_HPP

#include <string>

#include "file_transfer.grpc.pb.h"

namespace apriorit5::network
{
	constexpr size_t kChunkSize = 64 * 1024; // 64KB

	class FileTransferClient
	{
	public:
		explicit FileTransferClient(const std::string& target_address);
		void SendFile(const std::string& filename);

	private:
		std::unique_ptr<FileTransferService::Stub> stub_;
	};
}

#endif // APRIORIT5_NETWORK_FILE_TRANSFER_CLIENT_HPP