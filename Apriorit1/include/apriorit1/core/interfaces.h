#ifndef APRIORIT1_INCLUDE_INTERFACES_H_
#define APRIORIT1_INCLUDE_INTERFACES_H_

#include <memory>
#include <ostream>
#include <vector>

#include "apriorit1/core/models.h"
#include "apriorit1/core/exceptions.h"

namespace apriorit1::core
{
	class IProcessExporter
	{
	public:
		virtual ~IProcessExporter() = default;
		virtual void ExportData(const std::vector<ProcessInfo>& processes, std::ostream& out) const = 0;
	};

	class IProcessExporterFactory
	{
	public:
		virtual ~IProcessExporterFactory() = default;
		[[nodiscard]] virtual std::unique_ptr<IProcessExporter> CreateExporter() const = 0;
	};

	class IProcessProvider
	{
	public:
		virtual ~IProcessProvider() = default;
		[[nodiscard]] virtual std::vector<ProcessInfo> GetProcesses() const = 0;
	};
}

#endif 
