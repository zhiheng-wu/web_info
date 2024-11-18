#pragma once
#include <string>
class ResultLinkedList
{
public: struct Node {
	Node* next = nullptr;
	int value = 0;
	Node();
	Node(struct Node* p, int v);
};
private:

	Node* _root, * _end;
	int _size = 0;
public:
	ResultLinkedList();
	~ResultLinkedList();
	void append(int value);
	int getSize()const;
	const Node* getFirst();
	const Node* getNext(const Node* first);
	int getValue(const ResultLinkedList::Node* n);
	std::string print();
};

