#include "ResultLinkedList.h"

ResultLinkedList::ResultLinkedList()
{
	_root = new struct ResultLinkedList::Node();
	_end = _root;
}

ResultLinkedList::~ResultLinkedList()
{
	ResultLinkedList::Node* p = _root;
	ResultLinkedList::Node* pm;
	while (p)
	{
		pm = p;
		p = p->next;
		delete pm;
	}
}

void ResultLinkedList::append(int value)
{
	_end->next = new struct ResultLinkedList::Node( nullptr,value );
	_end = _end->next;
	_size++;
}

int ResultLinkedList::getSize() const
{
	return _size;
}

const ResultLinkedList::Node* ResultLinkedList::getFirst()
{
	return _root->next;
}

const ResultLinkedList::Node* ResultLinkedList::getNext(const ResultLinkedList::Node* first)
{
	if (first == nullptr)return nullptr;
	return first->next;
}

int ResultLinkedList::getValue(const ResultLinkedList::Node* n)
{
	if (n == nullptr)return -1;
	return n->value;
}

std::string ResultLinkedList::print()
{
	std::string ret;
	Node* p = _root->next;
	while (p != nullptr)
	{
		ret += std::to_string(p->value);
		if (p->next != nullptr)
		{
			ret += '\n';
		}
		p = p->next;
	}
	return ret;
}

ResultLinkedList::Node::Node()
{
}

ResultLinkedList::Node::Node(Node* p, int v)
{
	next = p;
	value = v;
}
