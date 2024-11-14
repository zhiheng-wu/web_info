#pragma once
class SkipList {
	struct Node {
		int _level;
		int _value;
		Node** _pointers;
		Node(int level, int value);
		~Node();
	}_root;
	int _max_level;
	int _skiplist_p;
	int getRandomLevel() const;
	Node* searchMaxCoveredSmallerNode(int level, int value);
public:
	SkipList(int maxlevel, float p);
	//插入节点
	void insert(int id);
	~SkipList();
};


