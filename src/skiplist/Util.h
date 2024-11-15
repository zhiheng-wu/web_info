#pragma once
#include <string>
#include <map>
#include <list>
#include <initializer_list>
#include "StringSkipList.h"
#include "StaticSkipList.h"
#include "ResultLinkedList.h"
// return new object
void buildPreDataSet(const char* path, std::map<std::string, void*>& preDataset);
void writeStaticSkipListToFile(const char* path, std::map<std::string, StaticSkipList*>* s);
std::map<std::string, StaticSkipList*>* ReadStaticSkipListFromFile(const char* path);
void writeStringSkipListToFile(const char* path, std::map<std::string, StringSkipList*>* s);
std::map<std::string, StringSkipList*>* ReadStringSkipListFromFile(const char* path);
template<typename T>
void release(std::map<std::string, T*>*& dataset);

template<typename T>
inline void release(std::map<std::string, T*>*& dataset)
{
	for (auto& i : *dataset)
	{
		delete i.second;
	}
	delete dataset;
	dataset = nullptr;
}

//会自动剔除输入空值, 输出可能为空
std::vector<ResultLinkedList*>* makeList(std::initializer_list<ResultLinkedList*> in);

//会自动剔除输入空值, 输出可能为空
std::vector<StringSkipList*>* makeList(std::initializer_list<StringSkipList*> in);

//用户需要确保 m 存在. 返回值可能为空
StringSkipList* getListFromMap(std::map<std::string, StringSkipList*>* m, std::string s);