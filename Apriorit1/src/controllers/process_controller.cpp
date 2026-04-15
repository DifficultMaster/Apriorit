#include "apriorit1/controllers/process_controller.h"

#include <fstream>
#include <iostream>
#include <format>

#include "apriorit1/core/exceptions.h"

namespace apriorit1::controllers
{
	ProcessController::ProcessController(std::unique_ptr<core::IProcessProvider> provider) : provider_(std::move(provider))
	{
		if (provider_ == nullptr)
		{
			throw core::ProcessFetchException("Process provider is null");
		}
	}

	void ProcessController::ExportProcesses(const core::IProcessExporterFactory& factory, const std::string& filename) const
	{
		std::vector<core::ProcessInfo> processes = provider_->GetProcesses();
		std::unique_ptr<core::IProcessExporter> exporter = factory.CreateExporter();

		if (exporter == nullptr)
		{
			throw core::ProcessExportException("Exporter factory returned null exporter");
		}

		std::ofstream outfile(filename);
		if (!outfile.is_open())
		{
			throw core::ProcessExportException("Failed to open " + filename);
		}

		exporter->ExportData(processes, outfile);
		std::cout << "Exported to " << filename << std::endl;
	}
}
