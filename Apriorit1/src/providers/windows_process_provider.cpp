#include "apriorit1/providers/windows_process_provider.h"

#if defined(_WIN32)
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>

#include <algorithm>
#include <format>

[[nodiscard]] static ULONGLONG FileTimeToUInt64(const FILETIME& ft)
{
    ULARGE_INTEGER value{};
    value.LowPart = ft.dwLowDateTime;
    value.HighPart = ft.dwHighDateTime;
    return value.QuadPart;
}

[[nodiscard]] static std::string WideToUtf8(const wchar_t* value)
{
    if (value == nullptr)
    {
        return {};
    }

    const int required = WideCharToMultiByte(CP_UTF8, 0, value, -1, nullptr, 0, nullptr, nullptr);
    if (required <= 1)
    {
        return {};
    }

    std::string result(static_cast<size_t>(required), '\0');
    const int converted = WideCharToMultiByte(CP_UTF8, 0, value, -1, result.data(), required, nullptr, nullptr);
    if (converted <= 0)
    {
        return {};
    }

    result.resize(static_cast<size_t>(converted - 1));
    return result;
}

constexpr DWORD kNoMoreFilesErrorCode = 18U;

namespace apriorit1::providers
{

    class WinHandleGuard
    {
    public:
        explicit WinHandleGuard(HANDLE h) : handle_(h) {}      
        WinHandleGuard(const WinHandleGuard&) = delete;
        WinHandleGuard& operator=(const WinHandleGuard&) = delete;

        ~WinHandleGuard()
        {
            if (handle_ && handle_ != INVALID_HANDLE_VALUE)
            {
                CloseHandle(handle_);
            }
        }

        [[nodiscard]] bool IsValid() const
        {
            return handle_ != INVALID_HANDLE_VALUE && handle_ != nullptr;
        }

        [[nodiscard]] HANDLE Get() const
        {
            return handle_;
        }

    private:
        HANDLE handle_{ nullptr };
    };

    std::vector<core::ProcessInfo> WindowsProcessProvider::GetProcesses() const
    {
        std::vector<core::ProcessInfo> processes{};
        SYSTEM_INFO system_info{};
        GetSystemInfo(&system_info);
        const auto processor_count = std::max<DWORD>(system_info.dwNumberOfProcessors, 1U);

        WinHandleGuard snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));
        if (!snapshot.IsValid())
        {
            throw core::ProcessFetchException("Failed to create process snapshot");
        }

        PROCESSENTRY32W pe{};
        pe.dwSize = sizeof(PROCESSENTRY32W);

        if (!Process32FirstW(snapshot.Get(), &pe))
        {
            const DWORD error_code = GetLastError();
            if (error_code == kNoMoreFilesErrorCode)
            {
                return processes;
            }

            throw core::ProcessFetchException(std::format("Failed to read first process from snapshot. Win32 error: {0}", error_code));
        }

        do
        {
            core::ProcessInfo process{};
            process.pid = pe.th32ProcessID;
            process.name = WideToUtf8(pe.szExeFile);
            process.status = "Running";
            process.memory_usage_bytes = 0;
            process.cpu_usage_percent = 0.0;

            WinHandleGuard proc_handle(OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process.pid));
            if (proc_handle.IsValid())
            {
                PROCESS_MEMORY_COUNTERS pmc{};
                if (GetProcessMemoryInfo(proc_handle.Get(), &pmc, sizeof(pmc)))
                {
                    process.memory_usage_bytes = pmc.WorkingSetSize;
                }

                FILETIME creation_time{};
                FILETIME exit_time{};
                FILETIME kernel_time{};
                FILETIME user_time{};
                if (GetProcessTimes(proc_handle.Get(), &creation_time, &exit_time, &kernel_time, &user_time))
                {
                    FILETIME now{};
                    GetSystemTimeAsFileTime(&now);

                    const auto now_ticks = FileTimeToUInt64(now);
                    const auto creation_ticks = FileTimeToUInt64(creation_time);
                    const auto elapsed_ticks = (now_ticks > creation_ticks) ? (now_ticks - creation_ticks) : 0ULL;
                    const auto used_ticks = FileTimeToUInt64(kernel_time) + FileTimeToUInt64(user_time);

                    if (elapsed_ticks > 0ULL)
                    {
                        process.cpu_usage_percent =
                            (static_cast<double>(used_ticks) * 100.0) /
                            (static_cast<double>(elapsed_ticks) * static_cast<double>(processor_count));
                    }
                }
            }

            processes.push_back(std::move(process));

        } while (Process32NextW(snapshot.Get(), &pe));

        const DWORD next_error_code = GetLastError();
        if (next_error_code != kNoMoreFilesErrorCode)
        {
            throw core::ProcessFetchException(std::format("Failed while enumerating process snapshot. Win32 error: {0}", next_error_code));
        }

        return processes;
    }
}

#else

namespace apriorit1::providers
{
    std::vector<core::ProcessInfo> WindowsProcessProvider::GetProcesses() const
    {
        return {{1024, "mock_process", "Running", 1024000, 5.5}};
    }
}

#endif
