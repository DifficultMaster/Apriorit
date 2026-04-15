#ifndef APRIORIT1_INCLUDE_PROCESS_CONTROLLER_H_
#define APRIORIT1_INCLUDE_PROCESS_CONTROLLER_H_

#include <memory>
#include <string>

#include "apriorit1/core/interfaces.h"

namespace apriorit1::controllers
{
	class ProcessController
	{
	public:
		explicit ProcessController(std::unique_ptr<core::IProcessProvider> provider);
		void ExportProcesses(const core::IProcessExporterFactory& factory, const std::string& filename) const;

	private:
		std::unique_ptr<core::IProcessProvider> provider_;
	};
}

#endif 
