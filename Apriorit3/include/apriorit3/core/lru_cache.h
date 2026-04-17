#ifndef APRIORIT3_INCLUDE_CORE_LRU_CACHE_H_
#define APRIORIT3_INCLUDE_CORE_LRU_CACHE_H_

#include <mutex>
#include <list>
#include <unordered_map>

namespace apriorit3::core
{
	template<typename K, typename V>
	class LRUCache
	{
	private:
		size_t capacity_;
		std::mutex mutex_;
		std::list<std::pair<K, V>> items_list_;
		std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> items_map_;

        void MoveMostRecentlyUsed(const typename std::list<std::pair<K, V>>::iterator& list_node_ptr)
		{
			items_list_.splice(items_list_.begin(), items_list_, list_node_ptr);
		}	  

		void RemoveLeastRecentlyUsed()
		{
			items_map_.erase(items_list_.back().first);
			items_list_.pop_back();
		}

	public:
		explicit LRUCache(size_t capacity) : capacity_(capacity) {}	

		bool Get(const K& key, V& value)
		{
			std::lock_guard<std::mutex> lock(mutex_);
			
			auto iterator = items_map_.find(key);
			if (iterator == items_map_.end())
			{
				return false;
			}

			auto list_node_ptr = iterator->second;
			value = list_node_ptr->second;
			
			MoveMostRecentlyUsed(list_node_ptr);

			return true;
		}

		void Put(const K& key, const V& value)
		{
			std::lock_guard<std::mutex> lock(mutex_);	

			auto iterator = items_map_.find(key);
			if (iterator != items_map_.end())
			{
				auto list_node_ptr = iterator->second;
				list_node_ptr->second = value;

				MoveMostRecentlyUsed(list_node_ptr);
				return;
			}

			if (items_list_.size() >= capacity_)
			{
				RemoveLeastRecentlyUsed();
			}

			items_list_.emplace_front(key, value);
			items_map_[key] = items_list_.begin();
		}
	};
}

#endif // APRIORIT3_INCLUDE_CORE_LRU_CACHE_H_