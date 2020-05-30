#pragma once
#include <string>
#include <iostream>
#include <list>
#include <stdexcept>
#include "List.h"

using namespace std;
typedef bool Color;

template <typename Key, typename Val>
class Map
{
	enum _colors { BLACK = 0, RED };

	class Node
	{
	public:
		Key key;
		Val value;
		Color color;
		Node* parent;
		Node* left;
		Node* right;

		Node(Key key, Val value, Color color, Node* parent, Node* left, Node* right)
		{
			this->key = key;
			this->value = value;
			this->color = color;
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
	size_t size;

	Node* get_father(Node* current)
	{
		if (current == nullptr)
			return nullptr;

		return current->parent;
	}

	Node* get_grandfather(Node* current)
	{
		if (current == nullptr || get_father(current) == nullptr)
			return nullptr;

		return current->parent->parent;
	}

	Node* get_uncle(Node* current)
	{
		Node* temp = get_grandfather(current);

		if (temp == nullptr)
			return nullptr;

		if (get_father(current) == temp->left)
			return temp->right;

		if (get_father(current) == temp->right)
			return temp->left;

		return nullptr;
	}

	Node* get_brother(Node* current)
	{
		if (current == get_father(current)->left)
			return get_father(current)->right;
		else
			return get_father(current)->left;
	}

	void rotation_left(Node* rotatable)
	{
		Node* worker = rotatable->right;

		worker->parent = rotatable->parent;

		if (rotatable->parent != nullptr)
		{
			if (rotatable->parent->left == rotatable)
				rotatable->parent->left = worker;
			else
				rotatable->parent->right = worker;
		}
		else
			root = worker;

		rotatable->right = worker->left;
		if (worker->left != nullptr)
			worker->left->parent = rotatable;

		rotatable->parent = worker;
		worker->left = rotatable;

	}

	void rotation_right(Node* rotatable)
	{
		Node* worker = rotatable->left;

		worker->parent = rotatable->parent;

		if (rotatable->parent != nullptr)
		{
			if (rotatable->parent->left == rotatable)
				rotatable->parent->left = worker;
			else
				rotatable->parent->right = worker;
		}
		else
			root = worker;

		rotatable->left = worker->right;
		if (worker->right != nullptr)
			worker->right->parent = rotatable;

		rotatable->parent = worker;
		worker->right = rotatable;
	}

	void insert_balance(Node* current)
	{
		//У элемента нет отца!
		if (get_father(current) == nullptr)
			current->color = BLACK;
		else
		{
			//Вариант при котором у отца "чёрный" цвет 
			if (get_father(current)->color == BLACK)
				return;
			else
			{
				//Если у дяди красный цвет
				if (get_uncle(current) != nullptr && get_uncle(current)->color == RED)
				{
					get_father(current)->color = BLACK;
					get_uncle(current)->color = BLACK;
					get_grandfather(current)->color = RED;
					insert_balance(get_grandfather(current));
				}
				else
				{
					//Элемент правый сын своего отца, а отец - левый своего отца
					if (current == get_father(current)->right && get_father(current) == get_grandfather(current)->left)
					{
						rotation_left(get_father(current));
						current = current->left;
					}
					//Элемент правый сын своего отца, а отец - правый своего отца
					else if (current == get_father(current)->left && get_father(current) == get_grandfather(current)->right)
					{
						rotation_right(get_father(current));
						current = current->right;
					}
					get_father(current)->color = BLACK;
					get_grandfather(current)->color = RED;

					if (current == get_father(current)->left && get_father(current) == get_grandfather(current)->left)
						rotation_right(get_grandfather(current));
					else
						rotation_left(get_grandfather(current));
				}
			}
		}
	}

	void remove_balance(Node* current)
	{
		//У элемента есть родитель
		if (get_father(current) != nullptr)
		{
			//Брат элемента "красный"
			if (get_brother(current)->color == RED)
			{
				get_father(current)->color = RED;
				get_brother(current)->color = BLACK;

				if (current == get_father(current)->left)
					rotation_left(get_father(current));
				else
					rotation_right(get_father(current));
			}

			if (get_brother(current)->color == BLACK && get_father(current)->color == BLACK &&
				get_brother(current)->right->color == BLACK && get_brother(current)->left->color == BLACK)
			{
				get_brother(current)->color = RED;
				remove_balance(get_father(current));
			}
			else
			{
				if (get_brother(current)->color == BLACK && get_father(current)->color == RED &&
					get_brother(current)->right->color == BLACK && get_brother(current)->left->color == BLACK)
				{
					get_brother(current)->color = RED;
					get_father(current)->color = BLACK;
				}
				else
				{
					//Сын брата красный
					if (current == get_father(current)->left && get_brother(current)->right->color == BLACK && get_brother(current)->left->color == RED)
					{
						get_brother(current)->color = RED;
						get_brother(current)->left->color = BLACK;
						rotation_right(get_brother(current));
					}
					else if (current == get_father(current)->right && get_brother(current)->right->color == RED && get_brother(current)->left->color == BLACK)
					{
						get_brother(current)->color = RED;
						get_brother(current)->right->color = BLACK;
						rotation_left(get_brother(current));
					}

					get_brother(current)->color = get_father(current)->color;
					get_father(current)->color = BLACK;

					if (current == get_father(current)->left)
					{
						get_brother(current)->right->color = BLACK;
						rotation_left(get_father(current));
					}
					else
					{
						get_brother(current)->left->color = BLACK;
						rotation_right(get_father(current));
					}
				}
			}
		}
	}

	void insert(Key key, Val value, Node* current, Node* parent)
	{
		if (current == nullptr)
		{
			Node* node = new Node(key, value, RED, parent, nullptr, nullptr);

			if (node->key < parent->key)
				parent->left = node;
			else if (node->key >= parent->key)
				parent->right = node;

			insert_balance(node);
		}
		else
		{
			if (key < current->key)
				insert(key, value, current->left, current);
			else if (key >= current->key)
				insert(key, value, current->right, current);
		}
	}

	void clear(Node* node)
	{
		if (node != nullptr)
		{
			clear(node->left);
			clear(node->right);
			delete node;
		}
	}

	void remove(Node* removable)
	{
		//У элемента нет детей
		if (removable->right == nullptr && removable->left == nullptr)
		{
			if (get_father(removable) != nullptr)
			{
				if (removable == get_father(removable)->left) 
					get_father(removable)->left = nullptr;
				else if (get_father(removable)->right) 
					get_father(removable)->right = nullptr;
			}
			else
				root = nullptr;

			delete removable;
		}
		//У элемента один ребёнок
		else if (removable->left == nullptr || removable->right == nullptr)
		{
			Node* child = nullptr;

			if (removable->left == nullptr) 
				child = removable->right;
			else if (removable->right == nullptr) 
				child = removable->left;

			if (get_father(removable) != nullptr)
			{
				if (removable == get_father(removable)->left) 
				{
					get_father(removable)->left = child;
					child->parent = get_father(removable);
				}
				else if (removable == removable->parent->right) 
				{
					get_father(removable)->right = child;
					child->parent = get_father(removable);
				}

				if (removable->color == BLACK)
				{
					if (child->color == RED)
						child->color = BLACK;
					else
						remove_balance(child);
				}

				delete removable;
			}
			else
				root = child;

		}
		//У элемента максимум детей(2)
		else
		{
			Node* temp = removable;
			removable = removable->right;

			while (removable->left != nullptr)
				removable = removable->left;

			swap(removable->key, temp->key);
			swap(removable->value, temp->value);

			remove(removable);
		}
	}

	bool contains(Key& key, Node* node)
	{
		if (node != nullptr)
		{
			if (key < node->key)
				return contains(key, node->left);
			else if (key > node->key)
				return contains(key, node->right);
			else if (key == node->key)
				return true;
			else
				return false;
		}
		else
			return false;
	}

	Val find(Key& key, Node* node)
	{
		if (node != nullptr)
		{
			if (key < node->key)
				return find(key, node->left);
			else if (key > node->key)
				return find(key, node->right);
			else if (key == node->key)
				return node->value;
		    else
			     throw out_of_range("Элемента с таким ключём нет!");
	    }
		return 0;
	}

	//Возвращение указателя на элемент, который ищем по ключу
	Node* find_node(Key& key, Node* node)
	{
		if (node != nullptr)
		{
			if (key < node->key)
				return find_node(key, node->left);
			else if (key > node->key)
				return find_node(key, node->right);
			else if (key == node->key)
				return node;
			else
				throw out_of_range("Элемента с таким ключём нет!");

		}
	}

	//Симметричное пересечение дерева в зависимости от параметра
	void traverse_key(Node* node, List<Key>& returnable)
	{
		if (node != nullptr)
		{
			traverse_key(node->left, returnable);
			returnable.push_back(node->key);
			traverse_key(node->right, returnable);
		}
	}

	void traverse_value(Node* node, List<Val>& returnable)
	{
		if (node != nullptr)
		{
			traverse_value(node->left, returnable);
			returnable.push_back(node->value);
			traverse_value(node->right, returnable);
		}
	}

	void traverse_print(Node* node)
	{
		if (node != nullptr)
		{
			traverse_print(node->left);
			cout << "[" << node->key << ":" << node->value << "] ";
			traverse_print(node->right);
		}
	}

public:

	Map()
	{
		root = nullptr;
		size = 0;
	}

	Map(Key key, Val value)
	{
		size = 0;
		root = nullptr;
		insert(key, value);
	}

	~Map()
	{
		clear();
	}

	void insert(Key key, Val value)
	{
		if (root == nullptr)
		{
			root = new Node(key, value, BLACK, nullptr, nullptr, nullptr);
		}
		else
			insert(key, value, root, nullptr);

		size++;
	}

	void remove(Key key)
	{
		Node* removable = find_node(key, root);

		remove(removable);

		size--;
	}

	void clear()
	{
		clear(root);

		root = nullptr;
		size = 0;
	}

	void print()
	{
		traverse_print(root);
	}

	void correction(Key key, Val new_value)
	{
		Node* temp = find_node(key, root);
		temp->value = new_value;
	}

	//Возвращение всех ключей в виде списка
	List<Key> get_keys()
	{
		List<Key> list;
		if (root != nullptr)
			traverse_key(root, list);

		return list;
	}

	//Возвращение всех значений в виде списка
	List<Val> get_values()
	{
		List<Val> list;

		if (root != nullptr)
			traverse_value(root, list);

		return list;
	}

	Val find(Key key)
	{
		return find(key, root);
	}

	bool contains(Key key)
	{
		if (root != nullptr)
			return contains(key, root);
		else
			return false;
	}

	size_t getSize()
	{
		return size;
	}
};