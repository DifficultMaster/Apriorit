#ifndef APRIORIT6_TASK1_GUARDS_FILE_HANDLE_GUARD_HPP
#define APRIORIT6_TASK1_GUARDS_FILE_HANDLE_GUARD_HPP

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>

namespace apriorit6::task1::guards
{
	class FileHandleGuard
	{
	public:
		FileHandleGuard() noexcept;
		explicit FileHandleGuard(HANDLE handle) noexcept;
		~FileHandleGuard();

		FileHandleGuard(const FileHandleGuard&) = delete;
		FileHandleGuard& operator=(const FileHandleGuard&) = delete;

		FileHandleGuard(FileHandleGuard&& other) noexcept;
		FileHandleGuard& operator=(FileHandleGuard&& other) noexcept;

		[[nodiscard]] bool IsValid() const noexcept;
		[[nodiscard]] HANDLE Get() const noexcept;

		HANDLE Release() noexcept;
		void Reset(HANDLE new_handle = INVALID_HANDLE_VALUE) noexcept;

	private:
		HANDLE handle_;

		void Close() noexcept;
	};
}

#endif // APRIORIT6_TASK1_GUARDS_FILE_HANDLE_GUARD_HPP
