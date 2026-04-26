#include "guards/file_handle_guard.hpp"

#include <utility>

namespace apriorit6::task1::guards
{
	FileHandleGuard::FileHandleGuard() noexcept
		: handle_(INVALID_HANDLE_VALUE) {}

	FileHandleGuard::FileHandleGuard(HANDLE handle) noexcept
		: handle_(handle) {}

	FileHandleGuard::~FileHandleGuard()
	{
		Close();
	}

	FileHandleGuard::FileHandleGuard(FileHandleGuard&& other) noexcept
		: handle_(std::exchange(other.handle_, INVALID_HANDLE_VALUE)) {}

	FileHandleGuard& FileHandleGuard::operator=(FileHandleGuard&& other) noexcept
	{
		if (this != &other)
		{
			Close();
			handle_ = std::exchange(other.handle_, INVALID_HANDLE_VALUE);
		}

		return *this;
	}

	HANDLE FileHandleGuard::Get() const noexcept
	{
		return handle_;
	}

	HANDLE FileHandleGuard::Release() noexcept
	{
		return std::exchange(handle_, INVALID_HANDLE_VALUE);
	}

	void FileHandleGuard::Reset(HANDLE new_handle) noexcept
	{
		if (handle_ != new_handle)
		{
			Close();
			handle_ = new_handle;
		}
	}

	bool FileHandleGuard::IsValid() const noexcept
	{
		return handle_ != INVALID_HANDLE_VALUE && handle_ != NULL;
	}

	void FileHandleGuard::Close() noexcept
	{
		if (IsValid())
		{
			CloseHandle(handle_);
			handle_ = INVALID_HANDLE_VALUE;
		}
	}
}
