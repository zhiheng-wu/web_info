#pragma once
#include <string>
#include <map>
#include "StaticSkipList.h"
// return new object
void buildPreDataSet(const char* path, std::map<std::string, void*>& preDataset);
void writeStaticSkipListToFile(const char* path, std::map<std::string, StaticSkipList*>* s);
std::map<std::string, StaticSkipList*>* ReadStaticSkipListFromFile(const char* path);
