#ifndef APRIORIT1_INCLUDE_MODELS_H_
#define APRIORIT1_INCLUDE_MODELS_H_

#include <cstdint>
#include <string>

namespace apriorit1::core
{
	struct ProcessInfo
	{
		uint32_t pid;
		std::string name;
		std::string status;
		size_t memory_usage_bytes;
		double cpu_usage_percent;
	};
}

#endif 
