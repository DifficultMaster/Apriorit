#ifndef APRIORIT1_INCLUDE_JSON_FACTORY_H_
#define APRIORIT1_INCLUDE_JSON_FACTORY_H_

#include <memory>

#include "apriorit1/core/interfaces.h"

namespace apriorit1::exporters::factories
{
	class JsonExporterFactory : public core::IProcessExporterFactory
	{
	public:
		[[nodiscard]] std::unique_ptr<core::IProcessExporter> CreateExporter() const override;
	};
}

#endif 
