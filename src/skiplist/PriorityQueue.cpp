#include "PriorityQueue.h"

SequenceQueue::SequenceQueue(std::vector<StringSkipList*>* sv, std::vector<ResultLinkedList*>* rv)
{
	_size = sv->size() + rv->size();
	assert(_size > 1);
	_root = new Node[_size];
	_end = _root;
	int idx = 0;
	for (auto& i : *sv)
	{
		_root[idx] = { i->_data[0],0,i,nullptr };
		_end->next = _root + idx;
		_end = _end->next;
		idx++;
	}
	for (auto& i : *rv)
	{
		ResultLinkedList::Node* n = const_cast<ResultLinkedList::Node*>(i->getFirst());
		_root[idx] = { n->value,-1,nullptr,nullptr };
		_root[idx].RParent = n;
		_end->next = _root + idx;
		_end = _root + idx;
		idx++;
	}
	std::sort(_root, _root + _size);
	for (int i = 1; i < _size; i++)
	{
		_root[i - 1].next = _root + i;
	}
	_root[_size - 1].next = nullptr;
	_begin = _root;
	_end = _root + _size - 1;
	_valueBegin = _begin->value;
	_valueEnd = _end->value;

}

SequenceQueue::SequenceQueue(std::vector<StringSkipList*>* sv)
{
	_size = sv->size();
	assert(_size > 1);
	_root = new Node[_size];
	_end = _root;
	int idx = 0;
	for (auto& i : *sv)
	{
		_root[idx] = { i->_data[0],0,i,nullptr };
		_end->next = _root + idx;
		_end = _end->next;
		idx++;
	}
	std::sort(_root, _root + _size);
	for (int i = 1; i < _size; i++)
	{
		_root[i - 1].next = _root + i;
	}
	_root[_size - 1].next = nullptr;
	_begin = _root;
	_end = _root + _size - 1;
	_valueBegin = _begin->value;
	_valueEnd = _end->value;
}

SequenceQueue::SequenceQueue(std::vector<ResultLinkedList*>* rv)
{
	_size = rv->size();
	assert(_size > 1);
	_root = new Node[_size];
	_end = _root;
	int idx = 0;
	for (auto& i : *rv)
	{
		ResultLinkedList::Node* n = const_cast<ResultLinkedList::Node*>(i->getFirst());
		_root[idx] = { n->value,-1,nullptr,nullptr };
		_root[idx].RParent = n;
		_end->next = _root + idx;
		_end = _root + idx;
		idx++;
	}
	std::sort(_root, _root + _size, [](const Node& a, const Node& b)->bool {
		return a.value <= b.value;
		});
	for (int i = 1; i < _size; i++)
	{
		_root[i - 1].next = _root + i;
	}
	_root[_size - 1].next = nullptr;
	_begin = _root;
	_end = _root + _size - 1;
	_valueBegin = _begin->value;
	_valueEnd = _end->value;
}

SequenceQueue::~SequenceQueue()
{
	delete[] _root;
}

bool SequenceQueue::meetSame()
{
	return _valueBegin == _valueEnd;
}

bool SequenceQueue::upgrade()
{
	if (_begin->SIndex == -1)
	{
		auto p = _begin->RParent->next;
		if (p == nullptr) return false;
		while (p->value < _valueEnd)
		{
			p = p->next;
			if (p == nullptr)return false;
		}
		_begin->RParent = p;
		_begin->value = p->value;
		Node* p0 = _begin->next;
		_end->next = _begin;
		_end = _end->next;
		_begin->next = nullptr;
		_begin = p0;
		_valueBegin = p0->value;
		_valueEnd = _end->value;
	}
	else
	{
		auto s = _begin->SParent;
		auto p = s->_data[_begin->SIndex + 1];
		auto& v = _end->value;
		if (p == 0) return false;
		if (s->_data[p] < _valueEnd)
			p = s->FastSearchForMaxEntryNotSmallerThan(v, p, false);
		if (p == -1)return false;
		_begin->SIndex = p;
		_begin->value = s->_data[p];
		Node* p0 = _begin->next;
		_end->next = _begin;
		_end = _end->next;
		_begin->next = nullptr;
		_begin = p0;
		_valueBegin = p0->value;
		_valueEnd = _end->value;
	}
	return true;
}

bool SequenceQueue::upgradeAll()
{
	for (int i = 0; i < _size; i++)
	{
		if (upgrade() == false)return false;
	}
	return true;
}

int SequenceQueue::getValue()
{
	return _valueBegin;
}

PriorityQueue::PriorityQueue(std::vector<StringSkipList*>* sv, std::vector<ResultLinkedList*>* rv)
{
	for (auto& i : *sv)
		_data.push(Node(i->_data[0], 0, i));
	for (auto& i : *rv)
	{
		ResultLinkedList::Node* n = const_cast<ResultLinkedList::Node*>(i->getFirst());
		_data.push(Node(n->value, n));
	}
}

PriorityQueue::PriorityQueue(std::vector<StringSkipList*>* sv)
{
	for (auto& i : *sv)
		_data.push(Node(i->_data[0], 0, i));
}

PriorityQueue::PriorityQueue(std::vector<ResultLinkedList*>* rv)
{
	for (auto& i : *rv)
	{
		ResultLinkedList::Node* n = const_cast<ResultLinkedList::Node*>(i->getFirst());
		_data.push(Node(n->value, n));
	}
}

bool PriorityQueue::hasNext()
{
	return !_data.empty();
}

int PriorityQueue::popValue()
{
	Node n = _data.top();
	_data.pop();
	int v = n.value;
	if (n.SIndex == -1)
	{
		auto p = n.RParent->next;
		if (p != nullptr) {
			n.value = p->value;
			n.RParent = p;
			_data.push(n);
		}
	}
	else
	{
		auto& s = *n.SParent;
		auto p = s._data[n.SIndex + 1];
		if (p != 0)
		{
			n.value = s._data[p];
			n.SIndex = p;
			_data.push(n);
		}
	}
	return v;
}

PriorityQueue::Node::Node(int v, int id, StringSkipList* s)
{
	value = v;
	SIndex = id;
	SParent = s;
}

PriorityQueue::Node::Node(int v, ResultLinkedList::Node* r)
{
	value = v;
	SIndex = -1;
	RParent = r;
}

ResultLinkedList* andOperation(std::vector<StringSkipList*>* sv)
{
	if (sv == nullptr)return nullptr;
	SequenceQueue qu = SequenceQueue(sv);
	ResultLinkedList* ret = new ResultLinkedList();
	if (qu.meetSame())
	{
		ret->append(qu.getValue());
		if (qu.upgradeAll() == false) {
			delete sv;
			return ret;
		}
	}
	while (qu.upgrade())
	{
		if (qu.meetSame())
		{
			ret->append(qu.getValue());
			if (qu.upgradeAll() == false) break;
		}
	}
	delete sv;
	if (ret->getSize() == 0) {
		delete ret;
		return nullptr;
	}
	return ret;
}

ResultLinkedList* andOperation(std::vector<ResultLinkedList*>* rv)
{
	if (rv == nullptr)return nullptr;
	SequenceQueue qu = SequenceQueue(rv);
	ResultLinkedList* ret = new ResultLinkedList();
	if (qu.meetSame())
	{
		ret->append(qu.getValue());
		if (qu.upgradeAll() == false) {
			delete rv;
			return ret;
		}
	}
	while (qu.upgrade())
	{
		if (qu.meetSame())
		{
			ret->append(qu.getValue());
			if (qu.upgradeAll() == false) break;
		}
	}
	delete rv;
	if (ret->getSize() == 0) {
		delete ret;
		return nullptr;
	}
	return ret;
}

ResultLinkedList* andOperation(std::vector<StringSkipList*>* sv, std::vector<ResultLinkedList*>* rv)
{
	if (sv == nullptr)return andOperation(rv);
	if (rv == nullptr)return andOperation(sv);
	SequenceQueue qu = SequenceQueue(sv, rv);
	ResultLinkedList* ret = new ResultLinkedList();
	if (qu.meetSame())
	{
		ret->append(qu.getValue());
		if (qu.upgradeAll() == false) {
			delete sv;
			delete rv;
			return ret;
		}
	}
	while (qu.upgrade())
	{
		if (qu.meetSame())
		{
			ret->append(qu.getValue());
			if (qu.upgradeAll() == false) break;
		}
	}
	delete sv;
	delete rv;
	if (ret->getSize() == 0) {
		delete ret;
		return nullptr;
	}
	return ret;
}

ResultLinkedList* andNotOperation(std::vector<StringSkipList*>* sv, StringSkipList* notlist)
{
	if (notlist->getEntryCount() == 0)return andOperation(sv);
	if (sv == nullptr)return nullptr;
	SequenceQueue qu = SequenceQueue(sv);
	ResultLinkedList* ret = new ResultLinkedList();
	auto p = 0;
	if (qu.meetSame())
	{
		int v = qu.getValue();
		p = notlist->FastSearchForMaxEntryNotSmallerThan(v, p);
		if (p == -1 || v != notlist->_data[p])
		{
			ret->append(v);
			if (qu.upgradeAll() == false)
			{
				delete sv;
				delete ret;
				return nullptr;
			}
		}
		else if (qu.upgradeAll() == false) {
			delete sv;
			return ret;
		}
	}
	while (qu.upgrade())
	{
		if (qu.meetSame())
		{
			int v = qu.getValue();
			if (p != -1)
				p = notlist->FastSearchForMaxEntryNotSmallerThan(v, p);
			if (p == -1 || v != notlist->_data[p])
				ret->append(v);
			if (qu.upgradeAll() == false) break;
		}
	}
	delete sv;
	if (ret->getSize() == 0) {
		delete ret;
		return nullptr;
	}
	return ret;
}

ResultLinkedList* andNotOperation(std::vector<ResultLinkedList*>* rv, StringSkipList* notlist)
{
	if (notlist->getEntryCount() == 0)return andOperation(rv);
	if (rv == nullptr)return nullptr;
	SequenceQueue qu = SequenceQueue(rv);
	ResultLinkedList* ret = new ResultLinkedList();
	auto p = 0;
	if (qu.meetSame())
	{
		int v = qu.getValue();
		p = notlist->FastSearchForMaxEntryNotSmallerThan(v, p);
		if (p == -1 || v != notlist->_data[p])
		{
			ret->append(v);
			if (qu.upgradeAll() == false)
			{
				delete rv;
				delete ret;
				return nullptr;
			}
		}
		else if (qu.upgradeAll() == false) {
			delete rv;
			return ret;
		}
	}
	while (qu.upgrade())
	{
		if (qu.meetSame())
		{
			int v = qu.getValue();
			if (p != -1)
				p = notlist->FastSearchForMaxEntryNotSmallerThan(v, p);
			if (p == -1 || v != notlist->_data[p])
				ret->append(v);
			if (qu.upgradeAll() == false) break;
		}
	}
	delete rv;
	if (ret->getSize() == 0) {
		delete ret;
		return nullptr;
	}
	return ret;
}

ResultLinkedList* andNotOperation(std::vector<StringSkipList*>* sv, std::vector<ResultLinkedList*>* rv, StringSkipList* notlist)
{
	if (notlist->getEntryCount() == 0)return andOperation(sv, rv);
	if (sv == nullptr)return andNotOperation(rv, notlist);
	if (rv == nullptr)return andNotOperation(sv, notlist);
	SequenceQueue qu = SequenceQueue(sv, rv);
	ResultLinkedList* ret = new ResultLinkedList();
	auto p = 0;
	if (qu.meetSame())
	{
		int v = qu.getValue();
		p = notlist->FastSearchForMaxEntryNotSmallerThan(v, p);
		if (p == -1 || v != notlist->_data[p])
		{
			ret->append(v);
			if (qu.upgradeAll() == false)
			{
				delete rv;
				delete sv;
				delete ret;
				return nullptr;
			}
		}
		else if (qu.upgradeAll() == false) {
			delete rv;
			delete sv;
			return ret;
		}
	}
	while (qu.upgrade())
	{
		if (qu.meetSame())
		{
			int v = qu.getValue();
			if (p != -1)
				p = notlist->FastSearchForMaxEntryNotSmallerThan(v, p);
			if (p == -1 || v != notlist->_data[p])
				ret->append(v);
			if (qu.upgradeAll() == false) break;
		}
	}
	delete sv;
	delete rv;
	if (ret->getSize() == 0) {
		delete ret;
		return nullptr;
	}
	return ret;
}

ResultLinkedList* andNotOperation(std::vector<StringSkipList*>* sv, ResultLinkedList* notlist)
{
	if (notlist->getSize() == 0)return andOperation(sv);
	if (sv == nullptr)return nullptr;
	SequenceQueue qu = SequenceQueue(sv);
	ResultLinkedList* ret = new ResultLinkedList();
	auto p = notlist->getFirst();
	if (qu.meetSame())
	{
		int v = qu.getValue();
		while (p != nullptr && p->value < v)
		{
			p = p->next;
		}
		if (p == nullptr || v != p->value)
		{
			ret->append(v);
			if (qu.upgradeAll() == false)
			{
				delete sv;
				delete ret;
				return nullptr;
			}
		}
		else if (qu.upgradeAll() == false) {
			delete sv;
			return ret;
		}
	}
	while (qu.upgrade())
	{
		if (qu.meetSame())
		{
			int v = qu.getValue();
			while (p != nullptr && p->value < v)
			{
				p = p->next;
			}
			if (p == nullptr || v != p->value)
				ret->append(v);
			if (qu.upgradeAll() == false) break;
		}
	}
	delete sv;
	if (ret->getSize() == 0) {
		delete ret;
		return nullptr;
	}
	return ret;
}

ResultLinkedList* andNotOperation(std::vector<ResultLinkedList*>* rv, ResultLinkedList* notlist)
{
	if (notlist->getSize() == 0)return andOperation(rv);
	if (rv == nullptr)return nullptr;
	SequenceQueue qu = SequenceQueue(rv);
	ResultLinkedList* ret = new ResultLinkedList();
	auto p = notlist->getFirst();
	if (qu.meetSame())
	{
		int v = qu.getValue();
		while (p != nullptr && p->value < v)
		{
			p = p->next;
		}
		if (p == nullptr || v != p->value)
		{
			ret->append(v);
			if (qu.upgradeAll() == false)
			{
				delete rv;
				delete ret;
				return nullptr;
			}
		}
		else if (qu.upgradeAll() == false) {
			delete rv;
			return ret;
		}
	}
	while (qu.upgrade())
	{
		if (qu.meetSame())
		{
			int v = qu.getValue();
			while (p != nullptr && p->value < v)
			{
				p = p->next;
			}
			if (p == nullptr || v != p->value)
				ret->append(v);
			if (qu.upgradeAll() == false) break;
		}
	}
	delete rv;
	if (ret->getSize() == 0) {
		delete ret;
		return nullptr;
	}
	return ret;
}

ResultLinkedList* andNotOperation(std::vector<StringSkipList*>* sv, std::vector<ResultLinkedList*>* rv, ResultLinkedList* notlist)
{
	if (notlist->getSize() == 0)return andOperation(sv, rv);
	if (sv == nullptr)return andNotOperation(rv, notlist);
	if (rv == nullptr)return andNotOperation(sv, notlist);
	SequenceQueue qu = SequenceQueue(sv, rv);
	ResultLinkedList* ret = new ResultLinkedList();
	auto p = notlist->getFirst();
	if (qu.meetSame())
	{
		int v = qu.getValue();
		while (p != nullptr && p->value < v)
		{
			p = p->next;
		}
		if (p == nullptr || v != p->value)
		{
			ret->append(v);
			if (qu.upgradeAll() == false)
			{
				delete rv;
				delete sv;
				delete ret;
				return nullptr;
			}
		}
		else if (qu.upgradeAll() == false) {
			delete rv;
			delete sv;
			return ret;
		}
	}
	while (qu.upgrade())
	{
		if (qu.meetSame())
		{
			int v = qu.getValue();
			while (p != nullptr && p->value < v)
			{
				p = p->next;
			}
			if (p == nullptr || v != p->value)
				ret->append(v);
			if (qu.upgradeAll() == false) break;
		}
	}
	delete sv;
	delete rv;
	if (ret->getSize() == 0) {
		delete ret;
		return nullptr;
	}
	return ret;
}

ResultLinkedList* orOperation(std::vector<StringSkipList*>* sv)
{
	if (sv == nullptr)return nullptr;
	PriorityQueue qu = PriorityQueue(sv);
	ResultLinkedList* ret = new ResultLinkedList();
	while (qu.hasNext())
	{
		ret->append(qu.popValue());
	}
	delete sv;
	if (ret->getSize() == 0) {
		delete ret;
		return nullptr;
	}
	return ret;
}

ResultLinkedList* orOperation(std::vector<ResultLinkedList*>* rv)
{
	if (rv == nullptr)return nullptr;
	PriorityQueue qu = PriorityQueue(rv);
	ResultLinkedList* ret = new ResultLinkedList();
	while (qu.hasNext())
	{
		ret->append(qu.popValue());
	}
	delete rv;
	if (ret->getSize() == 0) {
		delete ret;
		return nullptr;
	}
	return ret;
}

ResultLinkedList* orOperation(std::vector<StringSkipList*>* sv, std::vector<ResultLinkedList*>* rv)
{
	if (sv == nullptr)return orOperation(rv);
	if (rv == nullptr)return orOperation(sv);
	PriorityQueue qu = PriorityQueue(sv, rv);
	ResultLinkedList* ret = new ResultLinkedList();
	while (qu.hasNext())
	{
		ret->append(qu.popValue());
	}
	delete rv;
	if (ret->getSize() == 0) {
		delete ret;
		return nullptr;
	}
	return ret;
}