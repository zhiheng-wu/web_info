#include "SkipList.h"
#include <cassert>
#include <random>
SkipList::Node::Node(int level, int value)
{
	assert(level > 0);
	_level = level;
	_pointers = new Node * [level];
	_value = value;
}

SkipList::Node::~Node()
{
	delete[] _pointers;
}

int SkipList::getRandomLevel() const
{
	int level = 1;
	while ((rand() & 0xffff) <= _skiplist_p && level < _max_level)
		level += 1;
	return level;
}

SkipList::Node* SkipList::searchMaxCoveredSmallerNode(int level, int value)
{
	int idx = _max_level - 1;
	int gate = level - 1;
	Node* p = &_root;
	while (idx >= gate)
	{
		Node* tmp = p->_pointers[idx];
		while (tmp->_value < value)
		{
			p = tmp;
			tmp = p->_pointers[idx];
		}
		idx--;
	}
	return p;
}

SkipList::SkipList(int maxlevel, float p) : _root(maxlevel, INT_MAX)
{
	assert(maxlevel > 0);
	_max_level = maxlevel;
	_skiplist_p = (int)(0xffff * p);
	for (int i = 0; i < maxlevel; i++)
	{
		_root._pointers[i] = &_root;
	}
}

void SkipList::insert(int id)
{
	int level = getRandomLevel();
	Node*** _nodePPCache = new Node * *[level];
	Node* p = searchMaxCoveredSmallerNode(level, id);
	int idx = level - 1;
	if (p->_pointers[idx]->_value == id)
	{
		delete[] _nodePPCache;
		return;
	}
	else
		_nodePPCache[idx] = &p->_pointers[idx];
	for (idx--; idx >= 0; idx--)
	{
		Node* tmp = p->_pointers[idx];
		while (tmp->_value < id)
		{
			p = tmp;
			tmp = p->_pointers[idx];
		}
		if (tmp->_value == id)
		{
			delete[] _nodePPCache;
			return;
		}
		else
			_nodePPCache[idx] = &p->_pointers[idx];
	}
	Node* inserted = new Node(level, id);
	for (idx = 0; idx < level; idx++)
	{
		inserted->_pointers[idx] = *_nodePPCache[idx];
		*_nodePPCache[idx] = inserted;
	}
	delete[] _nodePPCache;
}

SkipList::~SkipList()
{
	Node* p = _root._pointers[0];
	Node* pre = nullptr;
	while (p != &_root)
	{
		pre = p;
		p = p->_pointers[0];
		delete pre;
	}
}
