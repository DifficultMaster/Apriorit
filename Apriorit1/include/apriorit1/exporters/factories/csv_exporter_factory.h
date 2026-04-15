#ifndef APRIORIT1_INCLUDE_CSV_FACTORY_H_
#define APRIORIT1_INCLUDE_CSV_FACTORY_H_

#include <memory>

#include "apriorit1/core/interfaces.h"

namespace apriorit1::exporters::factories
{
	class CsvExporterFactory : public core::IProcessExporterFactory
	{
	public:
		[[nodiscard]] std::unique_ptr<core::IProcessExporter> CreateExporter() const override;
	};
}

#endif 
