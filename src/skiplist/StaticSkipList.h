#pragma once
#include <set>
class StaticSkipList {
	struct Node {
		int right;
		int down;
		bool downIsValue() const;
		bool downNotValue() const;
		int getValue()const;
		void setValue(const int& v);
	};
	Node* _list = nullptr;
	int _size = 0;
public:
	StaticSkipList(int density, const std::set<int>& s);
	~StaticSkipList();
};
