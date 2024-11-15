#pragma once
#include "StringSkipList.h"
#include "ResultLinkedList.h"
#include <vector>
#include <cassert>
#include <algorithm>
#include <queue>
//用于索引表快速AND操作
class SequenceQueue
{
	struct Node {
		int value = 0;
		int SIndex = -1;
		union {
			StringSkipList* SParent;
			ResultLinkedList::Node* RParent;
		};
		Node* next = nullptr;
		friend bool operator<(const Node& a, const Node& b) {
			return a.value > b.value;
		};
	}*_root, * _begin, * _end;
	int _valueBegin = 0, _valueEnd = 0;
	int _size = 0;
public:
	// 用户应该确保 sv 和 rv 存在且其内的索引表中没有空表. SequenceQueue 不负责释放 sv 和 rv
	SequenceQueue(std::vector<StringSkipList*>* sv, std::vector<ResultLinkedList*>* rv);
	// 用户应该确保 sv 存在且其内的索引表中没有空表. SequenceQueue 不负责释放 sv
	SequenceQueue(std::vector<StringSkipList*>* sv);
	// 用户应该确保 rv 存在且其内的索引表中没有空表. SequenceQueue 不负责释放 rv
	SequenceQueue(std::vector<ResultLinkedList*>* rv);

	~SequenceQueue();
	bool meetSame();
	bool upgrade();
	bool upgradeAll();
	int getValue();
};
//用于索引表快速OR操作
class PriorityQueue {
	struct Node {
		int value = 0;
		int SIndex = -1;
		union {
			StringSkipList* SParent;
			ResultLinkedList::Node* RParent;
		};
		friend bool operator<(const Node& a, const Node& b) {
			return a.value > b.value;
		}
		Node(int v, int id, StringSkipList* s);
		Node(int v, ResultLinkedList::Node* r);
	};
	std::priority_queue<Node> _data;
public:
	// 用户应该确保 sv 和 rv 存在且其内的索引表中没有空表, size > 0. PriorityQueue 不负责释放 sv 和 rv
	PriorityQueue(std::vector<StringSkipList*>* sv, std::vector<ResultLinkedList*>* rv);
	// 用户应该确保 sv 存在且其内的索引表中没有空表, size > 0. PriorityQueue 不负责释放 sv
	PriorityQueue(std::vector<StringSkipList*>* sv);
	// 用户应该确保 rv 存在且其内的索引表中没有空表, size > 0. PriorityQueue 不负责释放 rv
	PriorityQueue(std::vector<ResultLinkedList*>* rv);
	bool hasNext();
	int popValue();
};
// 用户应该确保若 sv 存在则其内的索引表中没有空表, 且 sv size > 1. andOperation 负责释放 sv, 返回值可能为空
ResultLinkedList* andOperation(std::vector<StringSkipList*>* sv);
// 用户应该确保若 rv 存在则其内的索引表中没有空表, 且 rv size > 1. andOperation 负责释放 rv, 返回值可能为空
ResultLinkedList* andOperation(std::vector<ResultLinkedList*>* rv);
// 用户应该确保若 sv 或 rv 存在则其内的索引表中没有空表, 且无论是否存在, 它们合起来都 size > 1. andOperation 负责释放 sv, 返回值可能为空
ResultLinkedList* andOperation(std::vector<StringSkipList*>* sv, std::vector<ResultLinkedList*>* rv);
// 用户应该确保若 sv 存在则其内的索引表中没有空表, 且 sv size > 1. andOperation 负责释放 sv, 返回值可能为空
ResultLinkedList* andNotOperation(std::vector<StringSkipList*>* sv, StringSkipList* notlist);
// 用户应该确保若 rv 存在则其内的索引表中没有空表, 且 rv size > 1. andOperation 负责释放 rv, 返回值可能为空
ResultLinkedList* andNotOperation(std::vector<ResultLinkedList*>* rv, StringSkipList* notlist);
// 用户应该确保若 sv 或 rv 存在则其内的索引表中没有空表, 且无论是否存在, 它们合起来都 size > 1. andOperation 负责释放 sv, 返回值可能为空
ResultLinkedList* andNotOperation(std::vector<StringSkipList*>* sv, std::vector<ResultLinkedList*>* rv, StringSkipList* notlist);
// 用户应该确保若 sv 存在则其内的索引表中没有空表, 且 sv size > 1. andOperation 负责释放 sv, 返回值可能为空
ResultLinkedList* andNotOperation(std::vector<StringSkipList*>* sv, ResultLinkedList* notlist);
// 用户应该确保若 rv 存在则其内的索引表中没有空表, 且 rv size > 1. andOperation 负责释放 rv, 返回值可能为空
ResultLinkedList* andNotOperation(std::vector<ResultLinkedList*>* rv, ResultLinkedList* notlist);
// 用户应该确保若 sv 或 rv 存在则其内的索引表中没有空表, 且无论是否存在, 它们合起来都 size > 1. andOperation 负责释放 sv, 返回值可能为空
ResultLinkedList* andNotOperation(std::vector<StringSkipList*>* sv, std::vector<ResultLinkedList*>* rv, ResultLinkedList* notlist);
// 用户应该确保若 sv 存在则其内的索引表中没有空表. andOperation 负责释放 sv, 返回值可能为空
ResultLinkedList* orOperation(std::vector<StringSkipList*>* sv);
// 用户应该确保若 rv 存在则其内的索引表中没有空表. andOperation 负责释放 rv, 返回值可能为空
ResultLinkedList* orOperation(std::vector<ResultLinkedList*>* rv);
// 用户应该确保若 sv 或 rv 存在则其内的索引表中没有空表. andOperation 负责释放 sv, 返回值可能为空
ResultLinkedList* orOperation(std::vector<StringSkipList*>* sv, std::vector<ResultLinkedList*>* rv);

