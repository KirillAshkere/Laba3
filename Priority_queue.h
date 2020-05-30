#pragma once
#include <stdexcept>

constexpr bool LEFT = false;
constexpr bool RIGHT = true;

template<typename K>
class priority_queue
{
private:
	class Node
	{
	public:
		K item;
		uint64_t priority;
		Node* parent;
		Node* left;
		Node* right;

		Node(K item, uint64_t priority, Node* parent, Node* left, Node* right)
		{
			this->item = item;
			this->priority = priority;
			this->parent = parent;
			this->left = left;
			this->right = right;
		}

		~Node()
		{
			this->parent = nullptr;
			this->left = nullptr;
			this->right = nullptr;
		}
	};

	Node* root;
	Node* last;
	size_t size;

	//меняет местами значения родителя и сына в зависимости от приоритета
	void Balance(Node* current)
	{
		//сын меньше родителя
		while (current->parent != nullptr && current->priority < current->parent->priority)
		{
			uint64_t temp_priority = current->priority;
			K temp_item = current->item;

			current->priority = current->parent->priority;
			current->item = current->parent->item;

			current->parent->item = temp_item;
			current->parent->priority = temp_priority;

			current = current->parent;
		}

		//в случае если у текущего 2 детей и он меньше 1-го из них
		while ((current->right != nullptr && current->left != nullptr) && (current->priority > current->left->priority || current->priority > current->right->priority))
		{
			if (current->left->priority <= current->right->priority)
			{
				uint64_t temp_priority = current->priority;
				K temp_item = current->item;

				current->priority = current->left->priority;
				current->item = current->left->item;
				current->left->item = temp_item;
				current->left->priority = temp_priority;
				current = current->left;
			}
			else if (current->right->priority < current->left->priority)
			{
				uint64_t temp_priority = current->priority;
				K temp_item = current->item;

				current->priority = current->right->priority;
				current->item = current->right->item;
				current->right->item = temp_item;
				current->right->priority = temp_priority;
				current = current->right;
			}
		}
		//у текущего есть левый ребёнок и он меньше отца
		if (current->left != nullptr && current->priority > current->left->priority)
		{
			uint64_t temp_priority = current->priority;
			K temp_item = current->item;

			current->priority = current->left->priority;
			current->item = current->left->item;
			current->left->item= temp_item;
			current->left->priority = temp_priority;
			current = current->left;

		}

	}

	void delete_last()
	{
		if (last == last->parent->right)
		{
			Node* temp = last;
			last = last->parent->left;

			last->parent->right=nullptr;
			delete temp;

			size--;
		}
		else if (last == last->parent->left) 
		{
			//Номер предпоследнего элемента
			size_t penultimate = size - 2; 
			//Высота кучи
			const uint64_t height = log2(size) + 1; 
			//Путь от корня до предпоследнего элемента
			bool* path = new bool[height - 1]; 
			size_t i = height - 2;

			//Создание пути от корня до предпоследнего, который станет последним
			while (penultimate != 0)
			{
				if (penultimate % 2 == 0)
				{
					path[i] = RIGHT;
					penultimate = (penultimate - 2) / 2;
				}
				else if (penultimate % 2 != 0)
				{
					path[i] = LEFT;
					penultimate = (penultimate - 1) / 2;
				}
				i--;
			}

			last->parent->left=nullptr;
			delete last;
			size--;

			last = root;

			i = 0;
			//Перемещение указателя но новый последний элемент по созданному пути
			while (i < height - 1)
			{
				if (path[i] == LEFT)
					last = last->left;
				else if (path[i] == RIGHT)
					last = last->right;

				i++;
			}

			delete[] path;
		}
	}

	//поиск элемента, в случае успеха возвращает указатель на найденный
	Node* contains(K item, Node* node) const
	{
		Node* found = nullptr; 

		if (node == nullptr)
			return nullptr;

		if (node->item == item)
			found = node;

		if (found == nullptr)
			if (node->left != nullptr)
				found = contains(item, node->left);

		if (found == nullptr)
			if (node->right != nullptr)
				found = contains(item, node->right);

		return found;
	}

public:

	priority_queue()
	{
		this->root = nullptr;
		this->last = nullptr;
		size = 0;
	}

	~priority_queue()
	{
		if (size > 0)
		{
			while (last->parent != nullptr)
				delete_last();

			delete root;
			size = 0;
		}
	}

	void insert(K item, uint64_t priority)
	{
		if (size == 0)
		{
			root = new Node(item, priority, nullptr, nullptr, nullptr);
			last = root;
			size++;
			return;
		}

		//пока текущий - правый ребёнок
		Node* current = last;
		while (current != root && current == current->parent->right)
			current = current->parent;

		if (current != root)
		{
			//если у родителя есть правый ребёнок
			if (current->parent->right != nullptr)
			{
				current = current->parent->right;

				while (current->left != nullptr)
					current = current->left;
			}
			else
				current = current->parent; 
		}
		else
		{
			while (current->left != nullptr)
				current = current->left;
		}

		if (current->left == nullptr)
		{
			last = new Node(item, priority, current, nullptr, nullptr);
			current->left=last;
		}
		else if (current->left != nullptr)
		{
			last = new Node(item, priority, current, nullptr, nullptr);
			current->right=last;
		}

		size++;
		//Вызов балансировки
		Balance(last); 
	}

	void remove(K item)
	{
		Node* current = contains(item, root);

		if (current != nullptr)
		{
			//В случае если в очереди один элемент
			if (current == root && size == 1)
			{
				delete root;
				size = 0;
				current = nullptr;
				root = nullptr;
				return;
			}

			//Если удаляется не последний
			if (current != last)
			{
				uint64_t temp_priority = last->priority;
				K temp_item = last->item;
				last->priority = current->priority;
				last->item = current->item;
				current->priority = temp_priority;
				current->item = temp_item;

				delete_last();
				Balance(current);
			}
			else
				delete_last();

		}
		else
			throw out_of_range("Удаляемого элемента нет в очереди!!");
	}

	//Удаление корня
	K extract_min()
	{
		if (root != nullptr)
		{
			uint64_t temp_priority = root->priority;
			K temp_item = root->item;

			if (size == 1)
			{
				delete root;
				size = 0;
				root = nullptr;
			}
			else
			{
				root->priority = last->priority;
				root->item = last->item;
				last->priority = temp_priority;
				last->item = temp_item;

				delete_last();
				Balance(root);
			}

			return temp_item;
		}
		else
			throw out_of_range("Удаляемого элемента нет в очереди!!");
	}

	uint64_t get_priority(K item)
	{
		if (root == nullptr)
			throw out_of_range("Нельзя выполнить операцию, очередь пуста!!");

		Node* required = contains(item, root);

		if (required == nullptr)
			throw out_of_range("Нельзя выполнить операцию, элемента нет в очереди!!");

		return required->priority;
	}

	uint64_t get_priority() const
	{
		if (root == nullptr)
			throw out_of_range("Нельзя выполнить операцию, очередь пуста!!");

		return root->priority;
	}

	K get_min()
	{
		if (root == nullptr)
			throw out_of_range("Нельзя выполнить операцию, очередь пуста!!");

		return root->item;
	}

	//Поиск элемента, в случае успеха возвращает true
	bool contains(K item)
	{
		auto temp = contains(item, root);
		if (temp != nullptr)
			return true;
		else
			return false;
	}

	void correction(K item, uint64_t new_priority)
	{
		Node* updateable = contains(item, root);

		if (updateable == nullptr)
			return;

		updateable->priority = new_priority;
		Balance(updateable);
	}

	size_t getSize() const
	{
		return size;
	}
};