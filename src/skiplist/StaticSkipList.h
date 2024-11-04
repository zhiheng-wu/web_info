#pragma once
#include <set>
#include <fstream>
class StaticSkipList {
public:
	struct Node {
		int right;
		int down;
		bool downIsValue() const;
		bool downNotValue() const;
		int getValue()const;
		void setValue(const int& v);
	};
private:
	Node* _list = nullptr;
	int _entrycount = 0;
	int _size = 0;
	StaticSkipList();
public:
	StaticSkipList(int density, const std::set<int>& s);
	~StaticSkipList();
	const Node* searchForMaxTopNodeNotBiggerThan(int value)const;
	const Node* searchForMaxTopNodeNotBiggerThan(int value, const Node* baseNotcmp)const;
	const Node* getNext(const Node* base) const;
	const Node* getNext() const;
	void writeToFile(std::ofstream& stream) const;
	static StaticSkipList* readFromFile(std::ifstream& stream)
	{
		StaticSkipList* s = new StaticSkipList();
		stream.read((char*)&s->_entrycount, sizeof(_entrycount));
		stream.read((char*)&s->_size, sizeof(_size));
		s->_list = new Node[s->_size];
		stream.read((char*)s->_list, sizeof(Node) * s->_size);
		return s;
	}
	int getValue(const Node* node)const
	{
		if (node == nullptr)
			return -1;
		if (node->downIsValue())
			return node->getValue();
		else return _list[node->down].getValue();
	}
};
