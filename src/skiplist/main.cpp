#include "FileAnalyser.h"
#include "SkipList.h"
#include "StaticSkipList.h"
#include "TimeCounter.h"
#include "Util.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <string>
using yang::time::TimeCounter;
using yang::time::Time;
using namespace std;

map<string, set<int>*>* buildStack(const char* path)
{
	map<string, set<int>*>* pdataset = new map<string, set<int>*>();
	auto& dataset = *pdataset;
	FileAnalyser file(path);
	FileAnalyser::Node* node = nullptr;
	int focusID = 0;
	while ((node = file.get()) != nullptr)
	{
		if (node->isID)
			focusID = node->id;
		else if (dataset.contains(node->tag))
		{
			dataset[node->tag]->insert(focusID);
		}
		else
		{
			set<int>* n = new set<int>();
			n->insert(focusID);
			dataset[node->tag] = n;
		}
		delete node;
	}
	return pdataset;
}

map<string, SkipList*>* buildSkipList(const char* path)
{
	map<string, SkipList*>* pdataset = new map<string, SkipList*>();
	auto& dataset = *pdataset;
	vector<SkipList*> record;
	FileAnalyser file(path);
	FileAnalyser::Node* node = nullptr;
	int focusID = 0;
	while ((node = file.get()) != nullptr)
	{
		if (node->isID)
			focusID = node->id;
		else if (dataset.contains(node->tag))
		{
			dataset[node->tag]->insert(focusID);
		}
		else
		{
			SkipList* n = new SkipList(10, 0.25);
			n->insert(focusID);
			dataset[node->tag] = n;
		}
		delete node;
	}
	return pdataset;
}

map<string, StaticSkipList*>* buildStaticSkipList(const char* path, int density = 1)
{
	map<string, void*>* dataset = new map<string, void*>();
	map<string, StaticSkipList*>* ret = new map<string, StaticSkipList*>();
	auto& sr = *ret;
	buildPreDataSet(path, *dataset);
	for (auto& i : *dataset)
	{
		set<int>* s = (set<int>*)(i.second);
		StaticSkipList* l = new StaticSkipList(density, *s);
		delete s;
		sr[i.first] = l;
	}
	return ret;
}

int main()
{
	const char* path = R"(D:\Desktop\web_info\data\book_processed_by_pkuseg.csv)";
	auto dataset = buildStaticSkipList(path);
	while (1);
	for (auto& i : *dataset)
	{
		delete i.second;
	}
	delete dataset;
}