#pragma once
#include "ResultLinkedList.h"
#include <set>
#include <fstream>
#include <vector>
#include <list>
class StringSkipList
{
	friend class SequenceQueue;
	friend class PriorityQueue;
	friend ResultLinkedList* andNotOperation(std::list<StringSkipList*>* sv, StringSkipList* notlist);
	friend ResultLinkedList* andNotOperation(std::list<ResultLinkedList*>* rv, StringSkipList* notlist);
	friend ResultLinkedList* andNotOperation(std::list<StringSkipList*>* sv, std::list<ResultLinkedList*>* rv, StringSkipList* notlist);
	friend ResultLinkedList* andNotOperation(std::list<StringSkipList*>* sv, ResultLinkedList* notlist);
	friend ResultLinkedList* andNotOperation(std::list<ResultLinkedList*>* rv, ResultLinkedList* notlist);
	friend ResultLinkedList* andNotOperation(std::list<StringSkipList*>* sv, std::list<ResultLinkedList*>* rv, ResultLinkedList* notlist);
	friend ResultLinkedList* notOperation(StringSkipList* s, StringSkipList* notlist);
	friend ResultLinkedList* notOperation(ResultLinkedList* r, StringSkipList* notlist);
	friend ResultLinkedList* notOperation(StringSkipList* s, ResultLinkedList* notlist);
	friend ResultLinkedList* notOperation(ResultLinkedList* r, ResultLinkedList* notlist);

private:
	int* _data = nullptr;
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
	int getNext(int baseIdx) const;
	int getFirst() const;
	int getSize()const;
	void writeToFile(std::ofstream& stream) const;
	static StringSkipList* readFromFile(std::ifstream& stream);
};

