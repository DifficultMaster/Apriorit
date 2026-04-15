#ifndef APRIORIT1_INCLUDE_WINDOWS_PROCESS_PROVIDER_H_
#define APRIORIT1_INCLUDE_WINDOWS_PROCESS_PROVIDER_H_

#include <vector>

#include "apriorit1/core/interfaces.h"

namespace apriorit1::providers
{
	class WindowsProcessProvider : public core::IProcessProvider
	{
	public:
		[[nodiscard]] std::vector<core::ProcessInfo> GetProcesses() const override;
	};
}

#endif 
