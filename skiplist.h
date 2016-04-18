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
	skipListIterator<E> _begin;
	skipListIterator<E> _end;

	bool throwCoin(float chance)
	{
		return !(rand() % (int)(1.0 / chance));
	}

public:
	typedef skipListIterator<E> iterator;

	template<class E> friend class skipListIterator;

	skiplist();

	bool add(E);
	bool contains(E) const;
	bool remove(E);
	void clear();
	int size() const;
	skipListIterator<E> begin() const;
	skipListIterator<E> end() const;

	template<class E> friend std::ostream& operator<< (std::ostream&, const skiplist&);

};

template<class E> skiplist<E>::skiplist()
{
	header = new node<E>(NULL, 0, 0, NULL);
	currentHeight = 0;
	maxHeight = 15;
	_size = 0;
	_begin._node = header;
	_end._node = NULL;

	srand(time(NULL));
}

template<class E> node<E>::node(E* data, int height, int nextCapacity, node<E>** next)
{
	this->data = data;
	this->height = height;
	this->next = new node<E>*[height + 1];

	for (int i = 0; i <= this->height; i++)
	{
		this->next[i] = NULL;
	};

	if (next != NULL)
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

template<class E> bool skiplist<E>::add(E e)
{
	int height = 0;
	while ((throwCoin(incrChance)) && (height < maxHeight))
	{
		height++;
	};

	node<E>* newHeader;
	if (height > currentHeight)
	{
		newHeader = new node<E>(NULL, height, header->height, header->next);
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
		nextNodes[i] = NULL;
	};

	node<E>* current = newHeader;
	node<E>* next = newHeader->next[currentHeight];

	for (int level = currentHeight; level >= 0; level--)
	{
		next = current->next[level];
		while (next != NULL)
		{
			if (*(next->data) == e)
			{
				return false;
			}
			else if (*(next->data) > e)
			{
				next = current;
				break;
			}
			current = next;
			next = next->next[level];
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
		_begin._node = newHeader;
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

template<class E> bool skiplist<E>::contains(E e) const
{
	node<E>* current = header;
	node<E>* next = header->next[currentHeight];
	for (int level = currentHeight; level >= 0; level--)
	{
		next = current->next[level];
		while (next != NULL)
		{
			if (*(next->data) == e)
			{
				return true;
			}
			else if (*(next->data) > e)
			{
				next = current;
				break;
			}
			current = next;
			next = next->next[level];
		}
	}
	return false;
}

template<class E> bool skiplist<E>::remove(E e)
{
	node<E>** prevNodes = new node<E>*[currentHeight + 1];
	node<E>* current = header;
	node<E>* next = header->next[currentHeight];
	node<E>* found = NULL;
	for (int i = 0; i <= currentHeight; i++)
	{
		prevNodes[i] = header;
	};

	for (int level = currentHeight; level >= 0; level--)
	{
		next = current->next[level];
		while (next != NULL)
		{
			if (*(next->data) == e)
			{
				if (found == NULL)
				{
					found = next;
				}
				break;
			}
			else if (*(next->data) > e)
			{
				next = current;
				break;
			}
			current = next;
			next = next->next[level];
		}
		prevNodes[level] = current;
	}
	if (found == NULL)
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
	while (current != NULL)
	{
		next = current->next[0];
		delete current;
		current = next;
	}
	delete header;
	header = new node<E>(NULL, 0, 0, NULL);
	currentHeight = 0;
	_size = 0;
	_begin._node = header;
}

template<class E> int skiplist<E>::size() const
{
	return _size;
}

template<class E> std::ostream& operator<< (std::ostream& out, const skiplist<E>& sl)
{
	if (sl.size() != 0)
	for (skipListIterator<E> it = sl.begin(); it != sl.end(); it++)
	{
		out << *it << ' ';
	}
	return out;
}

template<class E> skipListIterator<E>::skipListIterator()
{
	this->_node = NULL;
}

template<class E> skipListIterator<E>::skipListIterator(node<E>* _node)
{
	this->_node = _node;
}

template<class E> skipListIterator<E> skiplist<E>::begin() const
{
	return _begin;
}

template<class E> skipListIterator<E> skiplist<E>::end() const
{
	return _end;
}

template<class E> bool skipListIterator<E>::operator== (const skipListIterator& it) const
{
	return (this->_node == it._node);
}

template<class E> bool skipListIterator<E>::operator!= (const skipListIterator& it) const
{
	return (this->_node != it._node);
}

template<class E> E& skipListIterator<E>::operator* () const
{
	if (this->_node == NULL)
	{
		std::cout << " error: iterator not dereferencable ";
		exit(-1);
	}
	if (this->_node->data == NULL)
	{
		if (this->_node->next[0] != NULL)
		{
			return *(this->_node->next[0]->data);
		}
		else
		{
			std::cout << " error: iterator not dereferencable ";
			exit(-1);
		}
	}
	else
	{
		return *(this->_node->data);
	}
}

template<class E> skipListIterator<E> skipListIterator<E>::operator++ (int)
{
	if (this->_node == NULL)
	{
		std::cout << " error: iterator not incrementable ";
		exit(-1);
	}
	if (this->_node->data == NULL)
	{
		if (this->_node->next[0] != NULL)
		{
			skipListIterator<E> sl(*this);
			this->_node = this->_node->next[0]->next[0];
			return sl;
		}
		else
		{
			std::cout << " error: iterator not incrementable ";
			exit(-1);
		}
	}
	else
	{
		skipListIterator<E> sl(*this);
		this->_node = this->_node->next[0];
		return sl;
	}
}

template<class E> E* skipListIterator<E>::operator-> ()
{
	if (this->_node == NULL)
	{
		std::cout << " error: iterator not dereferencable ";
		exit(-1);
	}
	if (this->_node->data == NULL)
	{
		if (this->_node->next[0] != NULL)
		{
			return this->_node->next[0]->data;
		}
		else
		{
			std::cout << " error: iterator not dereferencable ";
			exit(-1);
		}
	}
	else
	{
		return this->_node->data;
	}
}
