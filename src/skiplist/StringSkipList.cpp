﻿#include "StringSkipList.h"
#include "Test.h"
#include <fstream>
using namespace std;
int getNodeCount(int indexBeginfrom1, int size)
{
	int indexLimit = (indexBeginfrom1 - 1) << 1;
	int ret = 1;
	int n = size - indexBeginfrom1;
	while (n && !(indexLimit & 1))
	{
		ret++;
		n >>= 1;
		indexLimit >>= 1;
	}
	if (ret == 1)ret = 2;
	return ret;
}


StringSkipList::~StringSkipList()
{
	delete[] _data;
}

StringSkipList::StringSkipList(const std::set<int>& s)
{
	int _entryCount = s.size();
	int* entryEndsize = new int[_entryCount];
	entryEndsize[0] = getNodeCount(1, _entryCount);
	for (int i = 1; i < _entryCount; i++)
		entryEndsize[i] = entryEndsize[i - 1] + getNodeCount(i + 1, _entryCount);
	_size = entryEndsize[_entryCount - 1];
	_data = new int[_size];
	std::set<int>::const_iterator it = s.begin();
	_data[0] = *it;
	int len = entryEndsize[0];
#ifdef DEBUG
	if (_entryCount != 1)
	{
		sllog("stringskiplist", "size", _size, "entrycount", _entryCount);
	}
#endif // DEBUG
	if (1 != _entryCount)
		_data[1] = len;
	else
		_data[1] = 0;
	int rightEntryIdx = 2;
	for (int i = len - 1; i > 1; i--, rightEntryIdx <<= 1)
	{
		_data[i] = entryEndsize[rightEntryIdx - 1];
	}
	it++;
	auto ed = s.end();
	int entryIdx = 1;
	while (it != ed)
	{
		int entryBeginIdx = entryEndsize[entryIdx - 1];
		int entryNextBeginIdx = entryEndsize[entryIdx];
		int entryEndIdx = entryNextBeginIdx - 1;
		int len = entryNextBeginIdx - entryBeginIdx;
		int rightEntryIdxMultiplier = 2;
		_data[entryBeginIdx] = *it;
		if (entryIdx + 1 != _entryCount)
		{
			_data[entryBeginIdx + 1] = entryNextBeginIdx;
		}
		else
		{
			_data[entryBeginIdx + 1] = 0;
		}
		for (int i = len - 1; i > 1; i--, rightEntryIdxMultiplier <<= 1)
		{
			_data[entryBeginIdx + i] = entryEndsize[entryIdx + rightEntryIdxMultiplier - 1];
		}
		it++;
		entryIdx++;
	}
}

StringSkipList::StringSkipList(ResultLinkedList& l)
{
	int _entryCount = l.getSize();
	int* entryEndsize = new int[_entryCount];
	entryEndsize[0] = getNodeCount(1, _entryCount);
	for (int i = 1; i < _entryCount; i++)
		entryEndsize[i] = entryEndsize[i - 1] + getNodeCount(i + 1, _entryCount);
	_size = entryEndsize[_entryCount - 1];
	_data = new int[_size];
	auto it = l.getFirst();
	_data[0] = it->value;
	int len = entryEndsize[0];
#ifdef DEBUG
	if (_entryCount != 1)
	{
		sllog("stringskiplist", "size", _size, "entrycount", _entryCount);
	}
#endif // DEBUG
	if (1 != _entryCount)
		_data[1] = len;
	else
		_data[1] = 0;
	int rightEntryIdx = 2;
	for (int i = len - 1; i > 1; i--, rightEntryIdx <<= 1)
	{
		_data[i] = entryEndsize[rightEntryIdx - 1];
	}
	it++;
	int entryIdx = 1;
	while (it != nullptr)
	{
		int entryBeginIdx = entryEndsize[entryIdx - 1];
		int entryNextBeginIdx = entryEndsize[entryIdx];
		int entryEndIdx = entryNextBeginIdx - 1;
		int len = entryNextBeginIdx - entryBeginIdx;
		int rightEntryIdxMultiplier = 2;
		_data[entryBeginIdx] = it->value;
		if (entryIdx + 1 != _entryCount)
		{
			_data[entryBeginIdx + 1] = entryNextBeginIdx;
		}
		else
		{
			_data[entryBeginIdx + 1] = 0;
		}
		for (int i = len - 1; i > 1; i--, rightEntryIdxMultiplier <<= 1)
		{
			_data[entryBeginIdx + i] = entryEndsize[entryIdx + rightEntryIdxMultiplier - 1];
		}
		it = it->next;
		entryIdx++;
	}
}

int StringSkipList::getValue(int entryBeginIdx)
{
	if (entryBeginIdx == -1)return -1;
	return _data[entryBeginIdx];
}

int StringSkipList::FastSearchForMaxEntryNotBiggerThan(int value, int baseNotcmpIdx) const
{
	if (baseNotcmpIdx == -1)return FastSearchForMaxEntryNotBiggerThan(value);
	if (_data[baseNotcmpIdx] > value)return -1;
	int currentEntryIndex = baseNotcmpIdx;
	int nextEntryIndex = _data[currentEntryIndex + 1];
	int rightEntryIndexPointer = currentEntryIndex + 2;
	while (nextEntryIndex != 0)
	{
		if (rightEntryIndexPointer == nextEntryIndex)
		{
			int nextEntryValue = _data[rightEntryIndexPointer];
			if (nextEntryValue < value) // to next
			{
				currentEntryIndex = rightEntryIndexPointer;
				nextEntryIndex = _data[rightEntryIndexPointer + 1];
				rightEntryIndexPointer = rightEntryIndexPointer + 2;
			}
			else if (nextEntryValue == value)
				return rightEntryIndexPointer;
			else
				return currentEntryIndex;
		}
		else
		{
			int rightEntryIndex = _data[rightEntryIndexPointer];
			int rightEntryValue = _data[rightEntryIndex];
			if (rightEntryValue < value) //to right
			{
				currentEntryIndex = rightEntryIndex;
				nextEntryIndex = _data[rightEntryIndex + 1];
				rightEntryIndexPointer = rightEntryIndex + 2;
			}
			else if (rightEntryValue == value)
				return rightEntryIndex;
			else
				rightEntryIndexPointer++; // to down
		}
	}
	return currentEntryIndex;
}

int StringSkipList::getNext(int baseIdx) const
{
	int p = _data[baseIdx + 1];
	if (p == 0)return -1;
	return p;
}

int StringSkipList::getFirst() const
{
	return 0;
}

int StringSkipList::getSize() const
{
	return _size;
}

void StringSkipList::writeToFile(std::ofstream& stream) const
{
	stream.write((const char*)&_size, sizeof(_size));
	stream.write((const char*)_data, sizeof(int) * (_size - 1));
}

StringSkipList* StringSkipList::readFromFile(std::ifstream& stream)
{
	StringSkipList* s = new StringSkipList();
	stream.read((char*)&s->_size, sizeof(_size));
	s->_data = new int[s->_size];
	stream.read((char*)s->_data, sizeof(int) * (s->_size - 1));
	s->_data[s->_size - 1] = 0;
	return s;
}

int StringSkipList::FastSearchForMaxEntryNotBiggerThan(int value) const
{
	int currentEntryIndex = 0;
	int tmp = _data[0];
	if (tmp > value)
		return -1;
	else if (tmp == value)
		return 0;
	int nextEntryIndex = _data[currentEntryIndex + 1];
	int rightEntryIndexPointer = currentEntryIndex + 2;
	while (nextEntryIndex != 0)
	{
		if (rightEntryIndexPointer >= nextEntryIndex)
		{
			int nextEntryValue = _data[rightEntryIndexPointer];
			if (nextEntryValue < value) // to next
			{
				currentEntryIndex = rightEntryIndexPointer;
				nextEntryIndex = _data[rightEntryIndexPointer + 1];
				rightEntryIndexPointer = rightEntryIndexPointer + 2;
			}
			else if (nextEntryValue == value)
				return rightEntryIndexPointer;
			else
				return currentEntryIndex;
		}
		else
		{
			int rightEntryIndex = _data[rightEntryIndexPointer];
			int rightEntryValue = _data[rightEntryIndex];
			if (rightEntryValue < value) //to right
			{
				currentEntryIndex = rightEntryIndex;
				nextEntryIndex = _data[rightEntryIndex + 1];
				rightEntryIndexPointer = (nextEntryIndex > rightEntryIndex + 3) ? (rightEntryIndex + 3) : nextEntryIndex; //fast search
			}
			else if (rightEntryValue == value)
				return rightEntryIndex;
			else
				rightEntryIndexPointer++; // to down
		}
	}
	return currentEntryIndex;
}

StringSkipList::StringSkipList()
{

}

int StringSkipList::FastSearchForMaxEntryNotSmallerThan(int value, int BaseIdx, bool cmpBase) const
{
	if (cmpBase && _data[BaseIdx] >= value)
		return BaseIdx;
	int currentEntryIndex = BaseIdx;
	int nextEntryIndex = _data[currentEntryIndex + 1];
	int rightEntryIndexPointer = currentEntryIndex + 2;
	while (nextEntryIndex != 0)
	{
		if (rightEntryIndexPointer == nextEntryIndex)
		{
			int nextEntryValue = _data[rightEntryIndexPointer];
			if (nextEntryValue < value) // to next
			{
				currentEntryIndex = rightEntryIndexPointer;
				nextEntryIndex = _data[rightEntryIndexPointer + 1];
				rightEntryIndexPointer = rightEntryIndexPointer + 2;
			}
			else
				return rightEntryIndexPointer;
		}
		else
		{
			int rightEntryIndex = _data[rightEntryIndexPointer];
			int rightEntryValue = _data[rightEntryIndex];
			if (rightEntryValue < value) //to right
			{
				currentEntryIndex = rightEntryIndex;
				nextEntryIndex = _data[rightEntryIndex + 1];
				rightEntryIndexPointer = rightEntryIndex + 2;
			}
			else if (rightEntryValue == value)
				return rightEntryIndex;
			else
				rightEntryIndexPointer++; // to down
		}
	}
	int ret = _data[currentEntryIndex + 1];
	if (ret == 0)return -1;
	return ret;
}



