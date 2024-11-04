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
	int len = counts[0];
	sllog("StaticSkipList", "data", 0, "len", counts[0]);
	if (1 != size)
	{
		_list[0].right = counts[1] - 1;
		sllog("StaticSkipList", "list", 0, "right", counts[1] - 1);
	}
	else
	{
		_list[0].right = 0;
		sllog("StaticSkipList", "list", 0, "right", 0);
	}
	_list[0].setValue(*s.begin());
	sllog("StaticSkipList", "list", 0, "setDown", *s.begin());
	int to = 2;
	for (int i = 1; i < len; i++, to <<= density)
	{
		_list[i].right = counts[to] - 1;
		_list[i].down = 0;
		sllog("StaticSkipList", "list", i, "right", counts[to] - 1);
		sllog("StaticSkipList", "list", i, "down", 0);
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
		int len = listNextIdxBegin - listIdxBegin;
		sllog("StaticSkipList", "data", idx, "len", len);
		int to = 2;
		if (idx + 1 != size)
		{
			sllog("StaticSkipList", "list", listIdxBegin, "right", counts[idx + 1] - 1);
			_list[listIdxBegin].right = counts[idx + 1] - 1;
		}
		else
		{
			sllog("StaticSkipList", "list", listIdxBegin, "right", 0);
			_list[listIdxBegin].right = 0;
		}
		_list[listIdxBegin].setValue(*it);
		sllog("StaticSkipList", "list", listIdxBegin, "setDown", *it);
		for (int i = 1; i < len; i++, to <<= density)
		{
			_list[listIdxBegin + i].right = counts[idx + to] - 1;
			_list[listIdxBegin + i].down = listIdxBegin;
			sllog("StaticSkipList", "list", listIdxBegin + i, "right", counts[idx + to] - 1);
			sllog("StaticSkipList", "list", listIdxBegin + i, "down", listIdxBegin);

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
