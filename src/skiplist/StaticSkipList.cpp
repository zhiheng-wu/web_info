#include "StaticSkipList.h"
#include "Test.h"
#include <cassert>
#include <iostream>
#include <string>


using namespace std;
int getNodeCount(int density, int densityMask, int indexBeginfrom1, int size)
{
	int indexLimit = (indexBeginfrom1 - 1) << 1;
	int ret = 0;
	int n = size - indexBeginfrom1;
	while (n && !(indexLimit & densityMask))
	{
		ret++;
		n >>= density;
		indexLimit >>= density;
	}
	if (ret == 0)ret = 1;
	sllog("getNodeCount", "indexBeginfrom1", indexBeginfrom1, "size", size, "ret", ret);
	return ret;
}

StaticSkipList::StaticSkipList() {}

StaticSkipList::StaticSkipList(int density, const set<int>& s)
{
	int size = s.size();
	int* counts = new int[size];
	int densityMask = 1;
	int d = density;
	while ((--d) > 0)
	{
		densityMask |= densityMask << 1;
	}
	counts[0] = getNodeCount(density, densityMask, 1, size);
	for (int i = 1; i < size; i++)
		counts[i] = counts[i - 1] + getNodeCount(density, densityMask, i + 1, size);
	_list = new Node[counts[size - 1]];
	_size = counts[size - 1];
	_entrycount = size;
	int len = counts[0];
	int endIdx = len - 1;
	sllog("StaticSkipList", "data", 0, "len", len);
	if (1 != size)
	{
		_list[endIdx].right = len;
		sllog("StaticSkipList", "list", endIdx, "right", len);
	}
	else
	{
		_list[endIdx].right = 0;
		sllog("StaticSkipList", "list", endIdx, "right", 0);
	}
	_list[endIdx].setValue(*s.begin());
	sllog("StaticSkipList", "list", endIdx, "setDown", *s.begin());
	int to = 2;
	for (int i = len - 2; i >= 0; i--, to <<= density)
	{
		_list[i].right = counts[to] - 1;
		_list[i].down = endIdx;
		sllog("StaticSkipList", "list", i, "right", counts[to] - 1);
		sllog("StaticSkipList", "list", i, "down", endIdx);
	}
	std::set<int>::const_iterator it = s.begin();
	it++;
	auto ed = s.end();
	int idx = 1;
	while (it != ed)
	{
		assert(idx < size);
		int listIdxBegin = counts[idx - 1];
		int listNextIdxBegin = counts[idx];
		int listIdxEnd = listNextIdxBegin - 1;
		int len = listNextIdxBegin - listIdxBegin;
		sllog("StaticSkipList", "data", idx, "len", len);
		int to = 2;
		if (idx + 1 != size)
		{
			sllog("StaticSkipList", "list", listIdxEnd, "right", listNextIdxBegin);
			_list[listIdxEnd].right = listNextIdxBegin;
		}
		else
		{
			sllog("StaticSkipList", "list", listIdxEnd, "right", 0);
			_list[listIdxEnd].right = 0;
		}
		_list[listIdxEnd].setValue(*it);
		sllog("StaticSkipList", "list", listIdxEnd, "setDown", *it);
		for (int i = len - 2; i >= 0; i--, to <<= density)
		{
			_list[listIdxBegin + i].right = counts[idx + to] - 1;
			_list[listIdxBegin + i].down = listIdxEnd;
			sllog("StaticSkipList", "list", listIdxBegin + i, "right", counts[idx + to] - 1);
			sllog("StaticSkipList", "list", listIdxBegin + i, "down", listIdxEnd);

		}
		idx++;
		it++;
	}
	delete[] counts;
}

StaticSkipList::~StaticSkipList()
{
	delete[] _list;
}

const StaticSkipList::Node* StaticSkipList::searchForMaxTopNodeNotBiggerThan(int value)const
{
	Node* p = &_list[0];
	int buf = getValue(p);
	if (buf > value)
		return nullptr;
	else if (buf == value)
		return p;
	else
	{
		Node* tmp = nullptr;
		Node* record = p;
		while (1)
		{
			while (1)
			{
				if (p->right == 0)return record;
				tmp = &_list[p->right];
				buf = getValue(tmp);
				if (buf < value) {
					p = tmp;
					record = tmp;
				}
				else if (buf == value)return tmp;
				else break;
			}
			if (p->downIsValue())return record;
			p = &_list[p->down];
		}
	}
}

const StaticSkipList::Node* StaticSkipList::searchForMaxTopNodeNotBiggerThan(int value, const Node* baseNotcmp) const
{
	if (baseNotcmp == nullptr)return nullptr;
	int buf;
	Node* p = const_cast<Node*>(baseNotcmp);
	Node* tmp = nullptr;
	Node* record = nullptr;
	while (1)
	{
		while (1)
		{
			if (p->right == 0)return record;
			tmp = &_list[p->right];
			buf = getValue(tmp);
			if (buf < value) {
				p = tmp;
				record = tmp;
			}
			else if (buf == value)return tmp;
			else break;
		}
		if (p->downIsValue())return record;
		p = &_list[p->down];
	}
}

const StaticSkipList::Node* StaticSkipList::getNext(const Node* base) const
{
	if (base == nullptr)return nullptr;
	if (base->right == 0)
		return nullptr;
	if (base->downIsValue())
		return &_list[base->right];
	return &_list[_list[base->down].right];
}

const StaticSkipList::Node* StaticSkipList::getNext() const
{
	return &_list[0];
}

void StaticSkipList::writeToFile(ofstream& stream) const
{
	stream.write((const char*)&_entrycount, sizeof(_entrycount));
	stream.write((const char*)&_size, sizeof(_size));
	stream.write((const char*)_list, sizeof(Node) * _size);
}

bool StaticSkipList::Node::downIsValue() const
{
	return (int)down & 0x80000000;
}

bool StaticSkipList::Node::downNotValue() const
{
	return ((int)down & 0x80000000) == 0;
}

int StaticSkipList::Node::getValue() const
{
	return (int)down & 0x7FFFFFFF;
}

void StaticSkipList::Node::setValue(const int& v)
{
	down = v | 0x80000000;
}
