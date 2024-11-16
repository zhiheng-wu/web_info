#include "FileAnalyser.h"
#include "StringSkipList.h"
#include "PriorityQueue.h"
#include "Util.h"
#include "BoolParser.h"
#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
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

map<string, StringSkipList*>* buildStringSkipList(const char* path)
{
	map<string, void*>* dataset = new map<string, void*>();
	map<string, StringSkipList*>* ret = new map<string, StringSkipList*>();
	auto& sr = *ret;
	buildPreDataSet(path, *dataset);
	for (auto& i : *dataset)
	{
		set<int>* s = (set<int>*)(i.second);
		StringSkipList* l = new StringSkipList(*s);
		delete s;
		sr[i.first] = l;
	}
	return ret;
}

map<string, StringSkipList*>* buildStringSkipListForFiles(initializer_list<const char*> paths)
{
	map<string, void*>* dataset = new map<string, void*>();
	map<string, StringSkipList*>* ret = new map<string, StringSkipList*>();
	auto& sr = *ret;
	for (auto& i : paths)
		buildPreDataSet(i, *dataset);
	for (auto& i : *dataset)
	{
		set<int>* s = (set<int>*)(i.second);
		StringSkipList* l = new StringSkipList(*s);
		delete s;
		sr[i.first] = l;
	}
	return ret;
}

#define FILE1 R"(book_processed_by_jieba.csv)"
#define FILE2 R"(book_processed_by_pkuseg.csv)"
#define FILE3 R"(movie_processed_by_jieba.csv)"
#define FILE4 R"(movie_processed_by_pkuseg.csv)"
#define FILE5 R"(selected_book_top_1200_data_tag.csv)"
#define FILE6 R"(selected_movie_top_1200_data_tag.csv)"
#define PATH_ROOT R"(..\..\data\)"
#define SKIP_ROOT R"(skiplist\)"
#define STRING_SUFFIX R"(.stringskiplist)"
#define FILE FILE6

void dataGenStringSkipList()
{
	auto dataset = buildStringSkipList(PATH_ROOT FILE1);
	writeStringSkipListToFile(PATH_ROOT SKIP_ROOT FILE1 ".stringskiplist", dataset);
	release(dataset);
	dataset = ReadStringSkipListFromFile(PATH_ROOT SKIP_ROOT FILE1 ".stringskiplist");
	release(dataset);
	dataset = buildStringSkipList(PATH_ROOT FILE2);
	writeStringSkipListToFile(PATH_ROOT SKIP_ROOT FILE2 ".stringskiplist", dataset);
	release(dataset);
	dataset = ReadStringSkipListFromFile(PATH_ROOT SKIP_ROOT FILE2 ".stringskiplist");
	release(dataset);
	dataset = buildStringSkipList(PATH_ROOT FILE3);
	writeStringSkipListToFile(PATH_ROOT SKIP_ROOT FILE3 ".stringskiplist", dataset);
	release(dataset);
	dataset = ReadStringSkipListFromFile(PATH_ROOT SKIP_ROOT FILE3 ".stringskiplist");
	release(dataset);
	dataset = buildStringSkipList(PATH_ROOT FILE4);
	writeStringSkipListToFile(PATH_ROOT SKIP_ROOT FILE4 ".stringskiplist", dataset);
	release(dataset);
	dataset = ReadStringSkipListFromFile(PATH_ROOT SKIP_ROOT FILE4 ".stringskiplist");
	release(dataset);
	dataset = buildStringSkipList(PATH_ROOT FILE5);
	writeStringSkipListToFile(PATH_ROOT SKIP_ROOT FILE5 ".stringskiplist", dataset);
	release(dataset);
	dataset = ReadStringSkipListFromFile(PATH_ROOT SKIP_ROOT FILE5 ".stringskiplist");
	release(dataset);
	dataset = buildStringSkipList(PATH_ROOT FILE6);
	writeStringSkipListToFile(PATH_ROOT SKIP_ROOT FILE6 ".stringskiplist", dataset);
	release(dataset);
	dataset = ReadStringSkipListFromFile(PATH_ROOT SKIP_ROOT FILE6 ".stringskiplist");
	release(dataset);
	dataset = buildStringSkipListForFiles({ PATH_ROOT FILE1 , PATH_ROOT FILE3 });
	writeStringSkipListToFile(PATH_ROOT SKIP_ROOT "all_processed_by_jieba.csv" ".stringskiplist", dataset);
	release(dataset);
	dataset = ReadStringSkipListFromFile(PATH_ROOT SKIP_ROOT "all_processed_by_jieba.csv" ".stringskiplist");
	release(dataset);
	dataset = buildStringSkipListForFiles({ PATH_ROOT FILE2 , PATH_ROOT FILE4 });
	writeStringSkipListToFile(PATH_ROOT SKIP_ROOT "all_processed_by_pkuseg.csv" ".stringskiplist", dataset);
	release(dataset);
	dataset = ReadStringSkipListFromFile(PATH_ROOT SKIP_ROOT "all_processed_by_pkuseg.csv" ".stringskiplist");
	release(dataset);
	dataset = buildStringSkipListForFiles({ PATH_ROOT FILE5 , PATH_ROOT FILE6 });
	writeStringSkipListToFile(PATH_ROOT SKIP_ROOT "all_given.csv" ".stringskiplist", dataset);
	release(dataset);
	dataset = ReadStringSkipListFromFile(PATH_ROOT SKIP_ROOT "all_given.csv" ".stringskiplist");
	release(dataset);
};

void* getDataSet(const char* str)
{
	return ReadStringSkipListFromFile(str);
};

int checkDataSet(void* p)
{
	return p != NULL;
};

void searchInDataSet(const char* input, const char* outputPath, void* dataset)
{
	ofstream f;
	f.open(outputPath, ios::out);
	f << search(input, (std::map<std::string, StringSkipList*>*)dataset);
	f.close();
};

void releaseDataSet(void* dataset)
{
	auto d = (std::map<std::string, StringSkipList*>*)dataset;
	release(d);
};

int main()
{
	/*
	auto dataset = ReadStringSkipListFromFile(PATH_ROOT SKIP_ROOT FILE4 STRING_SUFFIX);
	// (A OR B) AND C AND NOT D
	string a, b;
	cin >> a;
	cin >> b;
	auto key1 = getListFromMap(dataset, a);
	auto key2 = getListFromMap(dataset, b);
	auto v = andOperation(makeList({ key1, key2 }));
	cout << v->print();
	delete v;
	release(dataset);*/
}