#include "StaticSkipList.h"
#include <cassert>

using namespace std;
int getNodeCount(int density, int densityMask, int indexBeginfrom1, int size)
{
	int indexLimit = indexBeginfrom1 - 1;
	int ret = 1;
	int n = size - indexBeginfrom1;
	while (n && !(indexLimit & densityMask))
	{
		ret++;
		n >>= density;
		indexLimit >>= density;
	}
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
	if (1 != size)
		_list[0].right = counts[1] - 1;
	else
		_list[0].right = 0;
	_list[0].setValue(*s.begin());
	int to = 2;
	for (int i = 1; i < len; i++, to <<= density)
	{
		_list[i].right = counts[to] - 1;
		_list[i].down = 0;
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
		int to = 2;
		if (idx + 1 != size)
			_list[listIdxBegin].right = counts[idx + 1] - 1;
		_list[listIdxBegin].setValue(*it);
		for (int i = 1; i < len; i++, to <<= density)
		{
			_list[listIdxBegin + i].right = counts[idx + to] - 1;
			_list[listIdxBegin + i].down = 0;
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
