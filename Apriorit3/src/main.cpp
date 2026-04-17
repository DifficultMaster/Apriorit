#include <thread>
#include <vector>
#include <iostream>

#include "apriorit3/core/lru_cache.h"

int main()
{
	apriorit3::core::LRUCache<int, int> cache(5);

	std::vector<std::thread> threads;

	for (int t = 0; t < 4; ++t)
	{
		threads.emplace_back([&cache, t]()
		{
			for (int i = 0; i < 10000; i++)
			{
				cache.Put(i + t * 10000, i);
				
				int value = 0;
				cache.Get(i, value);
			}
		});
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	std::cout << "Finished\n";
}