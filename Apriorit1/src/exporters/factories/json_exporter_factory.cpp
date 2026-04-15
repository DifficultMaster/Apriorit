#include "apriorit1/exporters/factories/json_exporter_factory.h"
#include "apriorit1/exporters/json_exporter.h"

namespace apriorit1::exporters::factories
{
    std::unique_ptr<core::IProcessExporter> JsonExporterFactory::CreateExporter() const
    {
        return std::make_unique<JsonExporter>();
    }
}
