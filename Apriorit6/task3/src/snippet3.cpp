#include <mutex>

namespace apriorit6::task3::snippet3
{
	// Dummy class to silence compiler errors, please ignore
	class Database
	{
	public:
		bool isReady() { return false; }
		void write(int /*data*/) {};
	};

	inline int getData() { return 0; }

	// This snippet is copy-pasted code with comments explaining the problems
	namespace before
	{
		// if anything throws before m.unlock, the mutex will remain locked creating a resource leak and potential deadlocks, a wrapper is needed
		void Update(std::mutex& m, Database& db)
		{
			m.lock();
			if (!db.isReady()) return;
			db.write(getData());
			m.unlock();
		}
	}

	// This snippet is the same code, fixed to implement RAII correctly
	namespace after
	{
		void Update(std::mutex& m, Database& db)
		{			
			std::unique_lock<std::mutex> lock(m); // unique_lock is a modern RAII wrapper for mutexes that will automatically release one when it goes out of scope
			if (!db.isReady()) return;
			db.write(getData());
		}
	}
}



