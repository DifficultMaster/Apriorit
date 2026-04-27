#ifndef APRIORIT7_INCLUDE_CORE_LINKED_LIST_HPP
#define APRIORIT7_INCLUDE_CORE_LINKED_LIST_HPP

#include <functional>
#include <memory>

namespace apriorit7::core
{
	template<typename T>
	using SplitPredicate = std::function<bool(const T& value)>;

	template<typename T>
	class LinkedList
	{
	private:
		struct Iterator;
		struct ConstIterator;

	public:
		LinkedList() : head_(nullptr) {}

		LinkedList(std::initializer_list<T> elements) : LinkedList()
		{
			for (std::reverse_iterator<const T> iter = std::rbegin(elements); iter != std::rend(elements); ++iter)
			{
				push_front(*iter);
			}
		}

		LinkedList(const LinkedList&) = delete;

		LinkedList& operator=(const LinkedList&) = delete;

		LinkedList(LinkedList&& other) noexcept : head_(std::move(other.head_)) {}

		LinkedList& operator=(LinkedList&& other) noexcept
		{
			if (this != &other)
			{
				clear();
				head_ = std::move(other.head_);
			}
			return *this;
		}

		~LinkedList()
		{
			clear();
		}

		// Element access

		T& front()
		{
			return head_->value();
		}

		const T& front() const
		{
			return head_->value();
		}

		// Iterators		

		using iterator = Iterator;
		using const_iterator = ConstIterator;

		iterator begin()
		{
			return iterator(head_.get());
		}

		iterator end()
		{
			return iterator(nullptr);
		}

		const_iterator begin() const
		{
			return const_iterator(head_.get());
		}

		const_iterator end() const
		{
			return const_iterator(nullptr);
		}

		const_iterator cbegin() const
		{
			return const_iterator(head_.get());
		}

		const_iterator cend() const
		{
			return const_iterator(nullptr);
		}		

		// Capacity

		bool empty() const
		{
			return !head_;
		}

		size_t size() const
		{
			return std::distance(begin(), end());
		}

		size_t max_size() const
		{
			return std::numeric_limits<size_t>::max();
		}

		// Modifiers

		void clear()
		{
			while (head_)
			{
				std::unique_ptr<Node> temp = std::move(head_->next);
				head_ = std::move(temp);				
			}
		}	

		iterator insert_after(const_iterator pos, const T& value)
		{
			if (!pos.ptr) return end;

			Node* current = const_cast<Node*>(pos.ptr);
			std::unique_ptr<Node> new_node = std::make_unique<Node>(value);

			new_node->next = std::move(current->next);
			current->next = std::move(new_node);

			return iterator(current->next.get());
		}

		iterator insert_after(const_iterator pos, T&& value)
		{
			if (!pos.ptr) return end;

			Node* current = const_cast<Node*>(pos.ptr);
			std::unique_ptr<Node> new_node = std::make_unique<Node>(std::move(value));

			new_node->next = std::move(current->next);
			current->next = std::move(new_node);

			return iterator(current->next.get());
		}

		template <std::ranges::input_range R>
		iterator insert_range_after(const_iterator pos, R&& rg)
		{
			if (!pos.ptr) return end;

			iterator last_inserted(const_cast<Node*>(pos.ptr));

			for (auto&& val : std::forward<R>(rg))
			{
				last_inserted = insert_after(last_inserted, std::forward<decltype(val)>(val));
			}

			return last_inserted;
		}

		iterator erase_after(const_iterator pos)
		{
			if (!pos.ptr || !pos.ptr->next) return end;

			Node* current = const_cast<Node*>(pos.ptr);
			current->next = std::move(current->next->next);

			return iterator(current->next.get());
		}

		void push_front(const T& value)
		{
			std::unique_ptr<Node> new_node = std::make_unique<Node>(value);
			new_node->next = std::move(head_);
			head_ = std::move(new_node);
		}

		void push_front(T&& value)
		{
			std::unique_ptr<Node> new_node = std::make_unique<Node>(std::move(value));
			new_node->next = std::move(head_);
			head_ = std::move(new_node);
		}

		void pop_front()
		{
			if (head_)
			{
				head_ = std::move(head_->next);
			}
		}

		void resize(size_t count)
		{
			std::unique_ptr<Node>* current = &head_;
			size_t i = 0;

			while (*current && i < count)
			{
				current = &((*current)->next);
				++i;
			}

			if (i == count)
			{
				current->reset();
			}
			else
			{
				while (i < count)
				{
					*current = std::make_unique<Node>();
					current = &((*current)->next);
					++i;
				}
			}
		}

		void resize(size_t count, const T& value)
		{
			std::unique_ptr<Node>* current = &head_;
			size_t i = 0;

			while (*current && i < count)
			{
				current = &((*current)->next);
				++i;
			}

			if (i == count)
			{
				current->reset();
			}
			else
			{
				while (i < count)
				{
					*current = std::make_unique<Node>(value);
					current = &((*current)->next);
					++i;
				}
			}
		}

		void swap(LinkedList& other) noexcept
		{
			std::swap(head_, other.head_);
		}

		// Operations

		void splice_after(const_iterator pos, LinkedList& other)
		{
			if (!pos.ptr || !other.head_)
			{
				return;
			}

			Node* current = const_cast<Node*>(pos.ptr);
			Node* other_tail = other.head_.get();

			while (other_tail->next)
			{
				other_tail = other_tail->next.get();
			}

			other_tail->next = std::move(current->next);
			current->next = std::move(other.head_);
		}

		LinkedList<T> split_when(SplitPredicate<T> predicate)
		{
			LinkedList<T> new_list;

			if (!head_)
			{
				return new_list;
			}

			if (predicate(head_->value))
			{
				new_list.head_ = std::move(head_);
				return new_list;
			}

			Node* current = head_.get();
			while (current->next)
			{
				if (predicate(current->next->value))
				{
					new_list.head_ = std::move(current->next);
					return new_list;
				}
				current = current->next.get();
			}

			return new_list;
		}

		size_t remove(const T& value)
		{
			return remove_if([&value](const T& current_value) { return current_value == value; });
		}

		template<typename UnaryPredicate>
		size_t remove_if(UnaryPredicate p)
		{
			size_t removed_count = 0;

			while (head_ && p(head_->value))
			{
				pop_front();
				++removed_count;
			}

			if (head_)
			{
				Node* current = head_.get();
				while (current->next)
				{
					if (p(current->next->value))
					{
						current->next = std::move(current->next->next);
						++removed_count;
					}
					else
					{
						current = current->next.get();
					}
				}
			}

			return removed_count;
		}

		void reverse() noexcept
		{
			std::unique_ptr<Node> prev = nullptr;
			std::unique_ptr<Node> next = nullptr;
			std::unique_ptr<Node> current = std::move(head_);

			while (current)
			{
				next = std::move(current->next);
				current->next = std::move(prev);
				prev = std::move(current);
				current = std::move(next);
			}

			head_ = std::move(prev);
		}

	private:
		struct Node
		{
			T value;
			std::unique_ptr<Node> next;

			explicit Node(const T& value) : value(value), next(nullptr) {}
			explicit Node(T&& value) : value(std::move(value)), next(nullptr) {}			
		};

		struct Iterator
		{
			Node* ptr;

			friend class LinkedList;

			using iterator_category = std::forward_iterator_tag;
			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using pointer = T*;
			using reference = T&;
	
            explicit Iterator(Node* node) : ptr(node) {}

			pointer operator->() const
			{
				return &(ptr->value);
			}

			reference operator*() const
			{
				return ptr->value;
			}

			Iterator& operator++()
			{
				if (ptr)
				{
					ptr = ptr->next.get();
				}
				return *this;
			}

            Iterator operator++(int)
			{
              Iterator temp = *this;
				++(*this);
				return temp;
			}

            bool operator==(const Iterator& other) const
			{
				return ptr == other.ptr;
			}

            bool operator!=(const Iterator& other) const
			{
				return ptr != other.ptr;
			}
		};

        struct ConstIterator
		{
			const Node* ptr;

			friend class LinkedList;

			using iterator_category = std::forward_iterator_tag;
			using value_type = const T;
			using difference_type = std::ptrdiff_t;
			using pointer = const T*;
			using reference = const T&;

			explicit ConstIterator(const Node* node) : ptr(node) {}

			pointer operator->() const
			{
				return &(ptr->value);
			}

			reference operator*() const
			{
				return ptr->value;
			}

			ConstIterator& operator++()
			{
				if (ptr)
				{
					ptr = ptr->next.get();
				}
				return *this;
			}

            ConstIterator operator++(int)
			{
				ConstIterator temp = *this;
				++(*this);
				return temp;
			}

            bool operator==(const ConstIterator& other) const
			{
				return ptr == other.ptr;
			}

            bool operator!=(const ConstIterator& other) const
			{
				return ptr != other.ptr;
			}
		};

		std::unique_ptr<Node> head_;
	};

	template<typename T>
	bool operator==(const LinkedList<T>& lhs, const LinkedList<T>& rhs)
	{
		return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template<typename T>
	auto operator<=>(const LinkedList<T>& lhs, const LinkedList<T>& rhs)
	{
		return std::lexicographical_compare_three_way(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}
}

#endif // APRIORIT7_INCLUDE_CORE_LINKED_LIST_HPP