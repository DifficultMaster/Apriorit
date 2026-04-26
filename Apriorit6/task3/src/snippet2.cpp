#include <memory>

namespace apriorit6::task3::snippet2
{
	// Dummy class to silence compiler errors, please ignore
	class Texture {};
	inline Texture* loadTexture(const char* /*path*/) { return new Texture(); }
	inline bool isVisible() { return false; }
	inline void Draw(Texture* /*t*/) {}

	// This snippet is copy-pasted code with comments explaining the problems
	namespace before
	{
		void Render()
		{
			// if t throws anywhere before the end, the texture will never be deleted, causing a memory leak, a wrapper to take ownership is needed
			Texture* t = loadTexture("hero.png");
			if (!isVisible()) return;
			Draw(t);
			delete t;
		}

	}

	// This snippet is the same code, fixed to implement RAII correctly
	namespace after
	{
		void Render()
		{
			// unique_ptr is a modern RAII wrapper for raw pointers that will automatically delete the owned object when it goes out of scope
			std::unique_ptr<Texture> t(loadTexture("hero.png"));
			if (!isVisible()) return;
			Draw(t.get()); // get() is used to access the raw pointer without releasing ownership, ensuring that the texture will be properly deleted even if Draw throws
			
			// no need to manually delete t, it will be automatically cleaned up when it goes out of scope
		}
	}
}



