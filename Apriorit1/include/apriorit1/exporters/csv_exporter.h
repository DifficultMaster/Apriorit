#ifndef APRIORIT1_INCLUDE_CSV_EXPORTER_H_
#define APRIORIT1_INCLUDE_CSV_EXPORTER_H_

#include "apriorit1/core/interfaces.h"

namespace apriorit1::exporters
{
	class CsvExporter : public core::IProcessExporter
	{
	public:
		void ExportData(const std::vector<core::ProcessInfo>& processes, std::ostream& out) const override;
	};
}

#endif 
