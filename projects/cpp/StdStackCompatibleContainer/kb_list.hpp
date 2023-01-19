#pragma once

namespace KB
{
	// minimalistic but not fully working STL compatible sequence container for using in std::stack
	template <class T>
	class list
	{
	private:
		class Node
		{
		public:
			T value;
			Node* next;
			Node* prev;
		};
	public:
		using value_type = typename T;
		using reference = typename T&;
		using const_reference = typename const T&;
		using size_type = typename size_t;

		bool empty() const noexcept { return size <= 0; }
		reference back() const noexcept { return tail->value; }
		void pop_back() noexcept
		{
			if (size > 0)
			{
				removeLast();
				--size;
			}
		}
		void push_back(value_type newElement) noexcept
		{
			addLast(newElement);
			++size;
		}
		list()
		{
			size = 0;
			tail = head = nullptr;
		}
		~list()
		{
			Node* current = head;
			while (current != nullptr)
			{
				Node* copy = current;
				current = current->next;
				delete copy;
			}
		}
	private:
		Node* head;
		Node* tail;
		size_t size = 0;
		void removeLast() noexcept
		{
			if (tail != nullptr)
			{
				Node* tmp = tail->prev;
				delete tail;
				tail = tmp;
			}
			if (tail == nullptr)
				head = nullptr;
		}
		void addLast(value_type newElement) noexcept
		{
			if (tail == nullptr)
				tail = new Node{ newElement,nullptr,nullptr };
			else
			{
				tail->next = new Node{ newElement,nullptr,tail };
				tail = tail->next;
			}
			if (head == nullptr)
			{
				head = tail;
				head->next = tail;
			}
		}
	};
}