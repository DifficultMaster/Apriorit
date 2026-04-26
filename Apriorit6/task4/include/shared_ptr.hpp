#ifndef APRIORIT6_TASK4_SHARED_PTR_HPP
#define APRIORIT6_TASK4_SHARED_PTR_HPP

#include <utility>

namespace apriorit6::task4
{
	template <typename Resource>
	class SharedPtr
	{
	public:
		SharedPtr() : m_ptr(nullptr), m_ref_count(nullptr) {}

		explicit SharedPtr(Resource* res) : m_ptr(res), m_ref_count(res ? new long(1) : nullptr) {}

		SharedPtr(const SharedPtr<Resource>& rhs) : m_ptr(rhs.m_ptr), m_ref_count(rhs.m_ref_count)
		{
			if (m_ref_count)
			{
				++(*m_ref_count);
			}
		}

		SharedPtr<Resource>& operator=(const SharedPtr<Resource>& rhs)
		{
			if (this != &rhs)
			{
				SharedPtr<Resource> temp(rhs);
				this->Swap(temp);
			}

			return *this;
		}

		~SharedPtr()
		{
			Release();
		}

		void Reset()
		{
			Release();
			m_ptr = nullptr;
			m_ref_count = nullptr;
		}

		void Reset(Resource* res)
		{
			Release();
			m_ptr = res;
			m_ref_count = res ? new long(1) : nullptr;
		}

		void Swap(SharedPtr<Resource>& rhs)
		{
			std::swap(m_ptr, rhs.m_ptr);
			std::swap(m_ref_count, rhs.m_ref_count);
		}

		Resource* Get() const
		{
			return m_ptr;
		}

		Resource& operator*() const
		{
			return *m_ptr;
		}

		Resource* operator->() const
		{
			return m_ptr;
		}

		long UseCount() const
		{
			return m_ref_count ? *m_ref_count : 0;
		}

	private:
		Resource* m_ptr;
		long* m_ref_count;

		void Release()
		{
			if (m_ref_count)
			{
				--(*m_ref_count);
				if (*m_ref_count == 0)
				{
					delete m_ptr;
					delete m_ref_count;
				}
			}
		}
	};

}

#endif // APRIORIT6_TASK4_SHARED_PTR_HPP