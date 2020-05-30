#pragma once
#include <stdexcept>
#include <iostream>
using namespace std;

template<typename T>
class List
{
private:
	class Node
	{
	public:
		T data;
		Node* next, *prev;
		Node() : next(NULL), prev(NULL) {};
		Node(T data)
		{
			this->data = data;
			next = NULL;
			prev = NULL;
		}

		~Node()
		{
			next = NULL;
			prev = NULL;
		}

		void set_data(T data)
		{
			this->data = data;
		}

		T get_data()
		{
			return data;
		}

		Node* get_next()
		{
			return next;
		}

		Node* get_prev()
		{
			return prev;
		}

		void set_next(Node* Elem)
		{
			next = Elem;
		}

		void set_prev(Node* Elem)
		{
			prev = Elem;
		}
	};
	Node* head, *tail;
	size_t size;
	Node* get_Elem(size_t index) const
	{
		if (isEmpty() || (index > get_size() - 1))
		{
			throw out_of_range("Некорректный индекс");
		}
		else if (index == get_size() - 1)
			return tail;
		else if (index == 0)
			return head;
		else
		{
			Node* temp = head;
			while ((temp) && (index--))
			{
				temp = temp->get_next();
			}
			return temp;
		}
	}
public:
	List() : head(NULL), tail(NULL), size(0) {}

	List(size_t size, int value)
	{
		head = NULL;
		tail = NULL;
		while (size--)
		{
			push_back(value);
		}
	}

	List(const List& list)
		: size(list.size)
	{
		if (size > 0)
		{
			head = new Node;
			head->set_data(list.head->get_data());

			if (size > 1)
			{
				Node * there = list.head->get_next();
				Node * here = head;
				for (size_t i = 0; i < size - 2; i++)
				{
					Node * temp = new Node;
					temp->set_data(there->get_data());
					temp->set_prev(here);

					here->set_next(temp);
					there = there->get_next();
					here = temp;
				}

				tail = new Node;
				tail->set_data(list.tail->get_data());
				tail->set_prev(here);
				here->set_next(tail);
			}
			else
			tail = head;
		}
	}

	List(List&& list) noexcept
		: size(list.size)
	{
		if (size > 0)
		{
			head = new Node;
			head->set_data(list.head->get_data());

			if (size > 1)
			{
				Node * there = list.head->get_next();
				Node * here = head;

				for (size_t i = 0; i < size - 2; i++)
				{
					Node * temp = new Node;
					temp->set_data(there->get_data());
					temp->set_prev(here);

					here->set_next(temp);
					there = there->get_next();
					here = temp;
				}

				tail = new Node;
				tail->set_data(list.tail->get_data());
				tail->set_prev(here);
				here->set_next(tail);
			}
			else
				tail = head;

			list.clear();
		}
	}

	List& operator=(const List& list)
	{
		if (this != &list)
		{
			clear();
			size = list.size;

			if (size > 0)
			{
				head = new Node;
				head->set_data(list.head->get_data());

				if (size > 1)
				{
					Node * there = list.head->get_next();
					Node * here = head;

					for (size_t i = 0; i < size - 2; i++)
					{
						Node * temp = new Node;
						temp->set_data(there->get_data());
						temp->set_prev(here);

						here->set_next(temp);
						there = there->get_next();
						here = temp;
					}
					tail = new Node;
					tail->set_data(list.tail->get_data());
					tail->set_prev(here);
					here->set_next(tail);

//					last = new List_element(list.last->value, nullptr, here);
//					here->next = last;
				}
				else
					tail = head;
//					last = first;
			}
		}

		return *this;
	}

	List& operator=(List&& list) noexcept
	{
		if (this != &list)
		{
			clear();
			size = list.size;

			if (size > 0)
			{
				head = new Node;
				head->set_data(list.head->get_data());

				if (size > 1)
				{
					Node * there = list.head->get_next();
					Node * here = head;

					for (size_t i = 0; i < size - 2; i++)
					{
						Node * temp = new Node;
						temp->set_data(there->get_data());
						temp->set_prev(here);

						here->set_next(temp);
						there = there->get_next();
						here = temp;
					}
					tail = new Node;
					tail->set_data(list.tail->get_data());
					tail->set_prev(here);
					here->set_next(tail);
				}
				else
					tail = head;
				list.clear();
			}

		}

		return *this;
	}

	~List()
	{
		clear();
	}

	void push_back(T data)
	{
		Node* temp = new Node;
		temp->set_data(data);
		if (head)
		{
			temp->set_prev(tail);
			tail->set_next(temp);
			tail = temp;
		}
		else
		{
			head = temp;
			tail = head;
		}
		size++;
	}

	void push_front(T data)
	{
		Node* temp = new Node;
		temp->set_data(data);
		if (head)
		{
			temp->set_next(head);
			head->set_prev(temp);
			head = temp;
		}
		else
		{
			head = temp;
			tail = head;
		}
		size++;
	}

	void push_front(List& list)
	{
		if (!isEmpty() && !list.isEmpty())
		{
			for (size_t i = 0; i < list.getSize(); i++)
				insert(list.at(i), i);
		}
		else
			throw out_of_range("Нельзя добавить в пустой список!");

	}

	void pop_back()
	{
		if (head != tail)
		{
			Node* temp = tail;
			tail = tail->get_prev();
			tail->set_next(NULL);
			delete temp;
			size--;
		}
		else if (!isEmpty())
		{
			Node* temp = tail;
			tail = head = NULL;
			delete temp;
			size = 0;
		}
		else
			throw out_of_range("Список пуст!!!");
	}

	void pop_front()
	{
		if (head != tail)
		{
			Node* temp = head;
			head = head->get_next();
			head->set_prev(NULL);
			delete temp;
			size--;
		}
		else if (!isEmpty())
		{
			Node* temp = head;
			head = tail = NULL;
			delete temp;
			size = 0;
		}
		else
			throw out_of_range("Список пуст!!!");
	}

	void insert(T data, const size_t index)
	{
		if (index <= size && index >= 0)
		{
			Node* temp;
			temp = get_Elem(index);
			if (temp == head)
				push_front(data);
			else if (index == size)
				push_back(data);
			else
			{
				Node* newElem = new Node;
				newElem->set_data(data);
				temp->get_prev()->set_next(newElem);
				newElem->set_prev(temp->get_prev());
				newElem->set_next(temp);
				temp->set_prev(newElem);
				size++;
			}
		}
		else throw out_of_range("Некорректный индекс!!");
	}

	T at(const size_t index) const
	{
		if (index < size && index >= 0)
		{
			Node* temp = get_Elem(index);

			return temp->get_data();
		}
		else throw out_of_range("Некорректный индекс!!");
	}

	void remove(const size_t index)
	{
		if (index < size && index >= 0)
		{
			auto temp = get_Elem(index);
			if (temp == head)
				pop_front();
			else if (temp == tail)
				pop_back();
			else
			{
				temp->get_prev()->set_next(temp->get_next());
				temp->get_next()->set_prev(temp->get_prev());
				delete temp;
			}
		}
		else throw out_of_range("Некорректный индекс!!");
	}

	void remove(T data)
	{
		Node* temp = head;
		while (temp && temp->get_data() != data)
			temp = temp->get_next();
		if (!temp)
			throw out_of_range("Недопустимый аргумент!!!");
		if (temp == head)
			pop_front();
		else if (temp == tail)
			pop_back();
		else
		{
			temp->get_prev()->set_next(temp->get_next());
			temp->get_next()->set_prev(temp->get_prev());
			delete temp;
		}
	}

	size_t get_size() const
	{
		return size;
	}

	void print_to_console() const
	{
		Node* temp = head;
		while (temp)
		{
			cout << temp->get_data() << " ";
			temp = temp->get_next();
		}
	}

	int find(T element) const
	{
		Node* temp = head;

		for (size_t pos = 0; pos < size; pos++)
		{
			if (temp->get_data() == element)
				return pos;

			temp = temp->get_next();
		}

		return -1;
	}

	void clear()
	{
		while (head)
		{
			tail = head->get_next();
			delete head;
			head = tail;
		}
		head = NULL;
	}

	void set(T data, const size_t index)
	{
		if (index < size && index >= 0)
		{
		Node* temp;
		temp = get_Elem(index);
		temp->set_data(data);
		}
		else throw out_of_range("Некорректный индекс!!");
	}

	bool isEmpty() const
	{
		if (!head)
			return true;
		else
			return false;
	}

	bool contains(T element) const
	{
		Node* temp = head;
		while (temp) 
		{

			if (temp->get_data()== element)
				return true;
			temp = temp->get_next();
		}
		return false;
	}

};
