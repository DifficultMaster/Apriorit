#include "apriorit1/exporters/json_exporter.h"

#include <format>
#include <sstream>

namespace apriorit1::exporters
{
	void JsonExporter::ExportData(const std::vector<core::ProcessInfo>& processes, std::ostream& out) const
	{
		std::ostringstream buffer{};

		try
		{
			buffer << "[\n";
			for (size_t i = 0; i < processes.size(); i++)
			{
				const core::ProcessInfo& process = processes[i];
				buffer << std::format(
					"	{{\n"
					"		\"pid\": {0},\n"
					"		\"name\": \"{1}\",\n"
					"		\"status\": \"{2}\",\n"
					"		\"memory_bytes\": {3},\n"
					"		\"cpu_percent\": {4:.2f}\n"
					"	}}{5}\n",
					process.pid, process.name, process.status, process.memory_usage_bytes, process.cpu_usage_percent, (i == processes.size() - 1) ? "" : ","
				);
			}
			buffer << "]\n";
		}
		catch (const std::exception& ex)
		{
			throw core::ProcessExportException(std::format("Failed during JSON formatting: {0}", ex.what()));
		}

		out << buffer.str();
		if (!out)
		{
			throw core::ProcessExportException("Failed to write JSON data to the output stream");
		}
	}
}
