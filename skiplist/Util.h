#pragma once
#include <string>
#include <map>
#include "StringSkipList.h"
#include "StaticSkipList.h"
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
