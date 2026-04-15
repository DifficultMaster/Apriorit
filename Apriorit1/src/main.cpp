#include <iostream>

#include "apriorit1/controllers/process_controller.h"
#include "apriorit1/providers/windows_process_provider.h"
#include "apriorit1/exporters/factories/csv_exporter_factory.h"
#include "apriorit1/exporters/factories/json_exporter_factory.h"

int main()
{
	try
	{
		std::unique_ptr<apriorit1::providers::WindowsProcessProvider> provider = std::make_unique<apriorit1::providers::WindowsProcessProvider>();
		apriorit1::controllers::ProcessController controller(std::move(provider));

		apriorit1::exporters::factories::CsvExporterFactory csv_exporter_factory;
		apriorit1::exporters::factories::JsonExporterFactory json_exporter_factory;

		controller.ExportProcesses(csv_exporter_factory, "processes.csv");
		controller.ExportProcesses(json_exporter_factory, "processes.json");
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Unhandled error: " << ex.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cerr << "Unhandled error!" << std::endl;
		return 1;
	}

	return 0;
}
