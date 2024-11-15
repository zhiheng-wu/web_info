#pragma once
#include "ResultLinkedList.h"
#include <set>
#include <fstream>
#include <vector>
class StringSkipList
{
	friend class SequenceQueue;
	friend class PriorityQueue;
	friend ResultLinkedList* andNotOperation(std::vector<StringSkipList*>* sv, StringSkipList* notlist);
	friend ResultLinkedList* andNotOperation(std::vector<ResultLinkedList*>* rv, StringSkipList* notlist);
	friend ResultLinkedList* andNotOperation(std::vector<StringSkipList*>* sv, std::vector<ResultLinkedList*>* rv, StringSkipList* notlist);
	friend ResultLinkedList* andNotOperation(std::vector<StringSkipList*>* sv, ResultLinkedList* notlist);
	friend ResultLinkedList* andNotOperation(std::vector<ResultLinkedList*>* rv, ResultLinkedList* notlist);
	friend ResultLinkedList* andNotOperation(std::vector<StringSkipList*>* sv, std::vector<ResultLinkedList*>* rv, ResultLinkedList* notlist);
private:
	int* _data = nullptr;
	int _entryCount = 0;
	int _size = 0;
	StringSkipList();
public:
	//用户应当确保 baseNotcmpIdx != -1
	int FastSearchForMaxEntryNotSmallerThan(int value, int BaseIdx, bool cmpBase = true) const;
	~StringSkipList();
	StringSkipList(const std::set<int>& s);
	StringSkipList(ResultLinkedList& l);
	int getValue(int entryBeginIdx);
	int FastSearchForMaxEntryNotBiggerThan(int value)const;
	// baseNotcmpIdx == -1 时从头开始搜索
	int FastSearchForMaxEntryNotBiggerThan(int value, int baseNotcmpIdx)const;
	friend ResultLinkedList* andStringSkipList(StringSkipList* a, StringSkipList* b)
	{
		ResultLinkedList* result = new ResultLinkedList();
		StringSkipList& la = *a;
		StringSkipList& lb = *b;
		if (la._entryCount == 0 || lb._entryCount == 0)return result;
		int va = la._data[0];
		int vb = lb._data[0];
		int pa = 0;
		int pb = 0;
		while (1)
		{
			if (va > vb)
			{
				int currentEntryIndex = pb;
				int nextEntryIndex = lb._data[pb + 1];
				int rightEntryIndexPointer = currentEntryIndex + 2;
				while (nextEntryIndex != 0)
				{
					if (rightEntryIndexPointer == nextEntryIndex)
					{
						int nextEntryValue = lb._data[rightEntryIndexPointer];
						if (nextEntryValue < va) // to next
						{
							currentEntryIndex = rightEntryIndexPointer;
							nextEntryIndex = lb._data[rightEntryIndexPointer + 1];
							rightEntryIndexPointer = rightEntryIndexPointer + 2;
						}
						else if (nextEntryValue == va)
						{
							result->append(va);
							pa = la._data[pa + 1];
							if (pa == 0)return result;
							va = la._data[pa];
							pb = lb._data[rightEntryIndexPointer + 1];
							if (pb == 0)return result;
							vb = lb._data[pb];
						}
						else
						{
							pa = la._data[pa + 1];
							if (pa == 0)return result;
							va = la._data[pa];
							pb = rightEntryIndexPointer;
							vb = nextEntryValue;
						}
					}
					else
					{
						int rightEntryIndex = lb._data[rightEntryIndexPointer];
						int rightEntryValue = lb._data[rightEntryIndex];
						if (rightEntryValue < va) //to right
						{
							currentEntryIndex = rightEntryIndex;
							nextEntryIndex = lb._data[rightEntryIndex + 1];
							rightEntryIndexPointer = rightEntryIndex + 2;
						}
						else if (rightEntryValue == va)
						{
							result->append(va);
							pa = la._data[pa + 1];
							if (pa == 0)return result;
							va = la._data[pa];
							pb = lb._data[rightEntryIndex + 1];
							if (pb == 0)return result;
							vb = lb._data[pb];
						}
						else
							rightEntryIndexPointer++; // to down
					}
				}
				return result;
			}
			else if (va < vb)
			{
				int currentEntryIndex = pa;
				int nextEntryIndex = la._data[pa + 1];
				int rightEntryIndexPointer = currentEntryIndex + 2;
				while (nextEntryIndex != 0)
				{
					if (rightEntryIndexPointer == nextEntryIndex)
					{
						int nextEntryValue = la._data[rightEntryIndexPointer];
						if (nextEntryValue < vb) // to next
						{
							currentEntryIndex = rightEntryIndexPointer;
							nextEntryIndex = la._data[rightEntryIndexPointer + 1];
							rightEntryIndexPointer = rightEntryIndexPointer + 2;
						}
						else if (nextEntryValue == vb)
						{
							result->append(vb);
							pb = lb._data[pb + 1];
							if (pb == 0)return result;
							vb = lb._data[pb];
							pa = la._data[rightEntryIndexPointer + 1];
							if (pa == 0)return result;
							va = la._data[pa];
						}
						else
						{
							pb = lb._data[pb + 1];
							if (pb == 0)return result;
							vb = lb._data[pb];
							pa = rightEntryIndexPointer;
							va = nextEntryValue;
						}
					}
					else
					{
						int rightEntryIndex = la._data[rightEntryIndexPointer];
						int rightEntryValue = la._data[rightEntryIndex];
						if (rightEntryValue < vb) //to right
						{
							currentEntryIndex = rightEntryIndex;
							nextEntryIndex = la._data[rightEntryIndex + 1];
							rightEntryIndexPointer = rightEntryIndex + 2;
						}
						else if (rightEntryValue == vb)
						{
							result->append(vb);
							pb = lb._data[pb + 1];
							if (pb == 0)return result;
							vb = lb._data[pb];
							pa = la._data[rightEntryIndex + 1];
							if (pa == 0)return result;
							va = la._data[pa];
						}
						else
							rightEntryIndexPointer++; // to down
					}
				}
				return result;
			}
			else
			{
				result->append(va);
				pa = la._data[pa + 1];
				if (pa == 0)return result;
				va = la._data[pa];
				pb = lb._data[pb + 1];
				if (pb == 0)return result;
				vb = lb._data[pb];
			}
		}
	}
	int getNext(int baseIdx) const;
	int getFirst() const;
	int getEntryCount()const;
	int getSize()const;
	void writeToFile(std::ofstream& stream) const;
	static StringSkipList* readFromFile(std::ifstream& stream);
};

