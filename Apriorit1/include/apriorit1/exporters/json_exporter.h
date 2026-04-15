#ifndef APRIORIT1_INCLUDE_JSON_EXPORTER_H_
#define APRIORIT1_INCLUDE_JSON_EXPORTER_H_

#include "apriorit1/core/interfaces.h"

namespace apriorit1::exporters
{
	class JsonExporter : public core::IProcessExporter
	{
	public:
		void ExportData(const std::vector<core::ProcessInfo>& processes, std::ostream& out) const override;
	};
}

#endif 
