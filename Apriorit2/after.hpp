#ifndef CODEREVIEWTASK_MYVECTOR_HPP
#define CODEREVIEWTASK_MYVECTOR_HPP

#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <stdexcept>

/*
 * MyVector stores a collection of objects with their names.
 *
 * For each object T, MyVector stores T`s name as std::string.
 * Several objects can have similar name.
 * operator[](const std::string& name) should return the first object
 * with the given name.
 *
 * Your task is to find as many mistakes and drawbacks in this code
 * (according to the presentation) as you can.
 * Annotate these mistakes with comments.
 *
 * Once you have found all the mistakes, rewrite the code
 * so it would not change its original purpose
 * and it would contain no mistakes.
 * Try to make the code more efficient without premature optimization.
 *
 * You can change MyVector interface completely, but there are several rules:
 * 1) you should correctly and fully implement copy-on-write idiom.
 * 2) std::pair<const T&, const std::string&> operator[](int index) const must take constant time at worst.
 * 3) const T& operator[](const std::string& name) const should be present.
 * 4) both operator[] should have non-const version.
 * 5) your implementation should provide all the member types of std::vector.
 * 6) your implementation should provide the following functions:
 *    1) begin(), cbegin(), end(), cend()
 *    2) empty(), size()
 *    3) reserve(), clear()
 */

template <typename T>
class MyVector
{
private:
    // RAII-compliant wrapper
    struct SharedState
    {
        std::vector<T> elements;
        std::vector<std::string> names;
    };

    // this is the variable we'll actually be using, it is dealloc safe (thanks to smart ptr) 
    // and unifies within it both of the vectors for easily trackable states, also enabling copy-on-write due to shared ownerhsip
    std::shared_ptr<SharedState> m_data;

    // key copy-on-write logic, this here will be called to detach and copy object on write operations
    void detach()
    {
		if (m_data && m_data.use_count() > 1)
        {
            m_data = std::make_shared<SharedState>(*m_data);
        }
    }

public:
    using value_type = T;
    using allocator_type = std::allocator<T>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
    using iterator = typename std::vector<T>::iterator;
	using const_iterator = typename std::vector<T>::const_iterator;
	using reverse_iterator = typename std::vector<T>::reverse_iterator;
	using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;

    // this constructor avoids double init; 
    // m_data only takes ownership of either both vectors, or neither
    MyVector() : m_data(std::make_shared<SharedState>()) {}

    // copy constructor is no longer needed, since shared_ptr as well as vector can handle copying themselves

    // destructor is no longer needed since the variables we now use are memory-safe 
    // and will self-delete when out of scope

    [[nodiscard]] bool empty() const
    {
		return m_data->elements.empty();
    }

    [[nodiscard]] size_type size() const
    {
        return m_data->elements.size();
    }

    void reserve(size_type new_cap)
    {
		if (new_cap > m_data->elements.capacity())
        {
            detach();
            m_data->elements.reserve(new_cap);
            m_data->names.reserve(new_cap);
        }
    }

    void clear()
    {
        if (!empty())
        {
            detach();
            m_data->elements.clear();
			m_data->names.clear();
        }
    }

    void push_back(const T& obj, const std::string& name)
    {
        detach();
		m_data->elements.push_back(obj);
		m_data->names.push_back(name);
    }

	iterator begin() 
    { 
        detach();
        return m_data->elements.begin(); 
    }

    const_iterator begin() const        
    { 
        return m_data->elements.begin(); 
	}

    const_iterator cbegin() const       
    { 
        return m_data->elements.cbegin(); 
	}   

    iterator end() 
    { 
        detach();
        return m_data->elements.end(); 
	}

    const_iterator end() const
    {
        return m_data->elements.end();
    }

    const_iterator cend() const
    {
        return m_data->elements.cend();
	}   

    std::pair<const T&, const std::string&> operator[](int index) const
    {
        if (index < 0 || static_cast<size_type>(index) >= m_data->elements.size())
        {
            throw std::out_of_range("Index is out of range");
        }
       
        return std::pair<const T&, const std::string&>(m_data->elements[index], m_data->names[index]);
    }

    std::pair<T&, std::string&> operator[](int index)
    {
        if (index < 0 || static_cast<size_type>(index) >= m_data->elements.size())
        {
            throw std::out_of_range("Index is out of range");
        }
       
        detach();
        return std::pair<T&, std::string&>(m_data->elements[index], m_data->names[index]);
	}

    const T& operator[](const std::string& name) const
    {
        std::vector<std::string>::const_iterator iter = std::find(m_data->names.begin(), m_data->names.end(), name);
        if (iter == m_data->names.end())
        {
            throw std::invalid_argument(name + " is not found in the MyVector");
        }

        return m_data->elements[std::distance(m_data->names.begin(), iter)];
    }

    T& operator[](const std::string& name)
    {
        std::vector<std::string>::const_iterator iter = std::find(m_data->names.begin(), m_data->names.end(), name);
        if (iter == m_data->names.end())
        {
            throw std::invalid_argument(name + " is not found in the MyVector");
        }

		size_type dist = std::distance(m_data->names.begin(), iter);

        detach();
        return m_data->element_type[dist];
    }
};


#endif //CODEREVIEWTASK_MYVECTOR_HPP


