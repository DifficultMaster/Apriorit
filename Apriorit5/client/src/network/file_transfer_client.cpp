#include "network/file_transfer_client.hpp"

#include <grpcpp/grpcpp.h>
#include <fstream>

#include "exceptions/network_exceptions.hpp"

namespace apriorit5::network
{
	FileTransferClient::FileTransferClient(const std::string& target_address) 
	: stub_(FileTransferService::NewStub(grpc::CreateChannel(target_address, grpc::InsecureChannelCredentials()))) {}

	void FileTransferClient::SendFile(const std::string& filename)
	{
		if (!std::filesystem::exists(filename))
		{
			throw exceptions::FileIoException("File to send does not exist");
		}

		std::ifstream infile(filename, std::ios::binary);
		if (!infile.is_open())
		{
			throw exceptions::FileIoException("Failed to open file for reading");
		}

		grpc::ClientContext context;
		UploadFileResponse response;
		std::unique_ptr<grpc::ClientWriter<UploadFileRequest>> writer(stub_->UploadFile(&context, &response));
		if (!writer)
		{
			throw exceptions::GrpcNetworkException("Failed to create gRPC upload stream");
		}

		UploadFileRequest request;
		request.mutable_metadata()->set_filename(std::filesystem::path(filename).filename().string());

		if (!writer->Write(request))
		{
			throw exceptions::GrpcNetworkException("Failed to send file metadata");
		}

        std::vector<char> buffer(kChunkSize);
		while (infile)
		{
			infile.read(buffer.data(), buffer.size());
			const std::streamsize bytes_read = infile.gcount();

			if (bytes_read <= 0)
			{
				break;
			}

			request.clear_request();
			request.mutable_data()->set_data(buffer.data(), bytes_read);

			if (!writer->Write(request))
			{
				throw exceptions::GrpcNetworkException("Failed to send file data");
			}
		}

		if (!infile.eof())
		{
			throw exceptions::FileIoException("Failed while reading file data");
		}
		if (!writer->WritesDone())
		{
			throw exceptions::GrpcNetworkException("Failed to finalize upload stream");
		}

		grpc::Status status = writer->Finish();

		if (!status.ok())
		{
			throw exceptions::GrpcNetworkException("gRPC error: " + status.error_message());
		}
		if (!response.success())
		{
            throw exceptions::GrpcNetworkException("Server rejected file: " + response.message());
		}
	}
}
