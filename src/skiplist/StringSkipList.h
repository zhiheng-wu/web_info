#pragma once
#include <set>
#include <fstream>
class StringSkipList
{
private:
	int* _data = nullptr;
	int _entryCount = 0;
	int _size = 0;
	StringSkipList();
public:
	~StringSkipList();
	StringSkipList(const std::set<int>& s);
	int getValue(int entryBeginIdx);
	int FastSearchForMaxEntryNotBiggerThan(int value)const;
	int FastSearchForMaxEntryNotBiggerThan(int value, int baseNotcmpIdx)const;
	int getNext(int baseIdx) const;
	int getNext() const;
	int getEntryCount()const;
	int getSize()const;
	void writeToFile(std::ofstream& stream) const;
	static StringSkipList* readFromFile(std::ifstream& stream);
};

