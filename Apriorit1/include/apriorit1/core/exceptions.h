#ifndef APRIORIT1_INCLUDE_EXCEPTIONS_H_
#define APRIORIT1_INCLUDE_EXCEPTIONS_H_

#include <stdexcept>

namespace apriorit1::core
{
	class ProcessAppException : public std::runtime_error
	{
	public:
		using std::runtime_error::runtime_error;
	};

	class ProcessFetchException : public ProcessAppException
	{
	public:
		using ProcessAppException::ProcessAppException;
	};

	class ProcessExportException : public ProcessAppException
	{
	public:
		using ProcessAppException::ProcessAppException;
	};
}

#endif 
