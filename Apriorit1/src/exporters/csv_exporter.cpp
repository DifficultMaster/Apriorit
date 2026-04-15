#include "apriorit1/exporters/csv_exporter.h"

#include <format>
#include <sstream>

namespace apriorit1::exporters
{
	void CsvExporter::ExportData(const std::vector<core::ProcessInfo>& processes, std::ostream& out) const
	{
		std::ostringstream buffer{};

		try
		{
			buffer << "PID,Name,Status,Memory(Bytes),CPU(%)\n";
			for (const core::ProcessInfo& process : processes)
			{
				buffer << std::format("{0},\"{1}\",{2},{3},{4:.2f}\n",
					process.pid, process.name, process.status, process.memory_usage_bytes, process.cpu_usage_percent);
			}
		}
		catch (const std::exception& ex)
		{
			throw core::ProcessExportException(std::format("Failed during CSV formatting: {0}", ex.what()));
		}

		out << buffer.str();
		if (!out)
		{
			throw core::ProcessExportException("Failed to write CSV data to the output stream");
		}
	}
}
