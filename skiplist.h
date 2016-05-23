#pragma once
#include <ctime>

template<class E> class node
{
private:
	E* data;
	int height;
	node<E>** next;

	node(E*, int, int, node<E>**);
	~node();

public:
	template <class E> friend class skiplist;
	template <class E> friend class skipListIterator;

};

template<class E> class skipListIterator
{
private:
	node<E>* _node;

	skipListIterator();
	skipListIterator(node<E>*);

public:
	template<class E> friend class skiplist;

	bool operator== (const skipListIterator&) const;
	bool operator!= (const skipListIterator&) const;
	E& operator *() const;
	skipListIterator<E> operator++ (int);
	E* operator ->();
};

template<class E> class skiplist
{
private:

	node<E>* header;
	int currentHeight;
	int maxHeight;
	int _size;
	float incrChance = 0.5;

	bool throwCoin(float chance)
	{
		return !(rand() % (int)(1.0 / chance));
	}

public:
	typedef skipListIterator<E> iterator;

	template<class E> friend class skipListIterator;

	skiplist();
	~skiplist();

	bool insert(E);
	bool erase(E);
	void clear();
	int size() const;
	skipListIterator<E> begin() const;
	skipListIterator<E> end() const;
	skipListIterator<E> find(E&) const;

	skiplist<E>& operator= (const skiplist<E>&);
	bool operator== (const skiplist<E>&);
	bool operator!= (const skiplist<E>&);

	template<class E> friend std::ostream& operator<< (std::ostream&, const skiplist&);

};

template<class E> skiplist<E>::skiplist()
{
	header = new node<E>(nullptr, 0, 0, nullptr);
	currentHeight = 0;
	maxHeight = 15;
	_size = 0;

	srand(time(nullptr));
}

template<class E> skiplist<E>::~skiplist()
{
	clear();
	delete header;
}

template<class E> node<E>::node(E* data, int height, int nextCapacity, node<E>** next)
{
	this->data = data;
	this->height = height;
	this->next = new node<E>*[height + 1];

	for (int i = 0; i <= this->height; i++)
	{
		this->next[i] = nullptr;
	};

	if (next != nullptr)
	{
		for (int i = 0; i <= this->height, i <= nextCapacity; i++)
		{
			this->next[i] = next[i];
		};
	}
}

template<class E> node<E>::~node()
{
	delete data, next;
}

template<class E> bool skiplist<E>::insert(E e)
{
	int height = 0;
	while ((throwCoin(incrChance)) && (height < maxHeight))
	{
		height++;
	};

	node<E>* newHeader;
	if (height > currentHeight)
	{
		newHeader = new node<E>(nullptr, height, header->height, header->next);
	}
	else
	{
		newHeader = header;
	}

	node<E>** prevNodes = new node<E>*[height + 1];
	node<E>** nextNodes = new node<E>*[height + 1];

	for (int i = 0; i <= height; i++)
	{
		prevNodes[i] = newHeader;
		nextNodes[i] = nullptr;
	};

	node<E>* current = newHeader;
	node<E>* next = newHeader->next[currentHeight];

	for (int level = currentHeight; level >= 0; level--)
	{
		next = current->next[level];
		while (next != nullptr)
		{
			if (*(next->data) == e)
			{
				return false;
			}
			else if (*(next->data) > e)
			{
				break;
			}
			current = next;
			next = current->next[level];
		}
		if (level <= height)
		{
			prevNodes[level] = current;
			nextNodes[level] = current->next[level];
		}
	}

	if (height > currentHeight)
	{
		delete header;
		header = newHeader;
		currentHeight = height;
	}

	node<E>* newNode = new node<E>(new E(e), height, height, nextNodes);
	for (int i = 0; i <= height; i++)
	{
		prevNodes[i]->next[i] = newNode;
	}
	delete prevNodes, nextNodes;
	_size++;
	return true;
}

template<class E> skipListIterator<E> skiplist<E>::find(E& e) const
{
	node<E>* current = header;
	node<E>* next = header->next[currentHeight];
	for (int level = currentHeight; level >= 0; level--)
	{
		next = current->next[level];
		while (next != nullptr)
		{
			if (*(next->data) == e)
			{
				skipListIterator<E> it(next);
				return it;
			}
			else if (*(next->data) > e)
			{
				break;
			}
			current = next;
			next = next->next[level];
		}
	}
	return this->end();
}

template<class E> bool skiplist<E>::erase(E e)
{
	node<E>** prevNodes = new node<E>*[currentHeight + 1];
	node<E>* current = header;
	node<E>* next = header->next[currentHeight];
	node<E>* found = nullptr;
	for (int i = 0; i <= currentHeight; i++)
	{
		prevNodes[i] = header;
	};

	for (int level = currentHeight; level >= 0; level--)
	{
		next = current->next[level];
		while (next != nullptr)
		{
			if (*(next->data) == e)
			{
				if (found == nullptr)
				{
					found = next;
				}
				break;
			}
			else if (*(next->data) > e)
			{
				break;
			}
			current = next;
			next = next->next[level];
		}
		prevNodes[level] = current;
	}
	if (found == nullptr)
	{
		return false;
	}
	for (int i = 0; i <= found->height; i++)
	{
		prevNodes[i]->next[i] = found->next[i];
	}
	delete found, prevNodes;
	_size--;
	return true;
}

template<class E> void skiplist<E>::clear()
{
	node<E>* current = header->next[0];
	node<E>* next;
	while (current != nullptr)
	{
		next = current->next[0];
		delete current;
		current = next;
	}
	delete header;
	header = new node<E>(nullptr, 0, 0, nullptr);
	currentHeight = 0;
	_size = 0;
}

template<class E> int skiplist<E>::size() const
{
	return _size;
}

template<class E> std::ostream& operator<< (std::ostream& out, const skiplist<E>& sl)
{
	for (skipListIterator<E> it = sl.begin(); it != sl.end(); it++)
	{
		out << *it << ' ';
	}
	return out;
}

template<class E> skiplist<E>& skiplist<E>::operator= (const skiplist<E>& sl)
{
	clear();
	for (skipListIterator<E> it = sl.begin(); it != sl.end(); it++)
	{
		insert(*it);
	}
	return *this;
}

template<class E> bool skiplist<E>::operator== (const skiplist<E>& sl)
{
	if (_size != sl.size())
	{
		return false;
	}
	for (skipListIterator<E> it1 = sl.begin(), skipListIterator<E> it2 = this->begin(); it1 != sl.end; it1++, it2++)
	{
		if (*it1 != *it2)
		{
			return false;
		}
	}
	return true;
}

template<class E> bool skiplist<E>::operator!= (const skiplist<E>& sl)
{
	return !(*this == sl);
}

template<class E> skipListIterator<E>::skipListIterator()
{
	this->_node = nullptr;
}

template<class E> skipListIterator<E>::skipListIterator(node<E>* _node)
{
	this->_node = _node;
}

template<class E> skipListIterator<E> skiplist<E>::begin() const
{
	skipListIterator<E> it(header->next[0]);
	return it;
}

template<class E> skipListIterator<E> skiplist<E>::end() const
{
	skipListIterator<E> it(nullptr);
	return it;
}

template<class E> bool skipListIterator<E>::operator== (const skipListIterator& it) const
{
	return (this->_node == it._node);
}

template<class E> bool skipListIterator<E>::operator!= (const skipListIterator& it) const
{
	return !(*this == it);
}

template<class E> E& skipListIterator<E>::operator* () const
{
	return *(this->_node->data);
}

template<class E> skipListIterator<E> skipListIterator<E>::operator++ (int)
{
	skipListIterator<E> sl(*this);
	this->_node = this->_node->next[0];
	return sl;
}

template<class E> E* skipListIterator<E>::operator-> ()
{
	return this->_node->data;
}
