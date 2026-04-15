#include "apriorit1/exporters/factories/csv_exporter_factory.h"
#include "apriorit1/exporters/csv_exporter.h"

namespace apriorit1::exporters::factories
{
    std::unique_ptr<core::IProcessExporter> CsvExporterFactory::CreateExporter() const
    {
        return std::make_unique<CsvExporter>();
    }
}
