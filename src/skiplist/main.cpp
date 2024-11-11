#include "FileAnalyser.h"
#include "SkipList.h"
#include "StaticSkipList.h"
#include "StringSkipList.h"
#include "Util.h"
#include <map>
#include <set>
#include <string>
#include <vector>
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

map<string, StaticSkipList*>* buildStaticSkipListForFiles(initializer_list<const char*> paths, int density = 1)
{
	map<string, void*>* dataset = new map<string, void*>();
	map<string, StaticSkipList*>* ret = new map<string, StaticSkipList*>();
	auto& sr = *ret;
	for (auto& i : paths)
		buildPreDataSet(i, *dataset);
	for (auto& i : *dataset)
	{
		set<int>* s = (set<int>*)(i.second);
		StaticSkipList* l = new StaticSkipList(density, *s);
		delete s;
		sr[i.first] = l;
	}
	return ret;
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
}

void dataGenStaticSkipList()
{
	auto dataset = buildStaticSkipList(PATH_ROOT FILE1);
	writeStaticSkipListToFile(PATH_ROOT SKIP_ROOT FILE1 ".staticskiplist", dataset);
	release(dataset);
	dataset = ReadStaticSkipListFromFile(PATH_ROOT SKIP_ROOT FILE1 ".staticskiplist");
	release(dataset);
	dataset = buildStaticSkipList(PATH_ROOT FILE2);
	writeStaticSkipListToFile(PATH_ROOT SKIP_ROOT FILE2 ".staticskiplist", dataset);
	release(dataset);
	dataset = ReadStaticSkipListFromFile(PATH_ROOT SKIP_ROOT FILE2 ".staticskiplist");
	release(dataset);
	dataset = buildStaticSkipList(PATH_ROOT FILE3);
	writeStaticSkipListToFile(PATH_ROOT SKIP_ROOT FILE3 ".staticskiplist", dataset);
	release(dataset);
	dataset = ReadStaticSkipListFromFile(PATH_ROOT SKIP_ROOT FILE3 ".staticskiplist");
	release(dataset);
	dataset = buildStaticSkipList(PATH_ROOT FILE4);
	writeStaticSkipListToFile(PATH_ROOT SKIP_ROOT FILE4 ".staticskiplist", dataset);
	release(dataset);
	dataset = ReadStaticSkipListFromFile(PATH_ROOT SKIP_ROOT FILE4 ".staticskiplist");
	release(dataset);
	dataset = buildStaticSkipList(PATH_ROOT FILE5);
	writeStaticSkipListToFile(PATH_ROOT SKIP_ROOT FILE5 ".staticskiplist", dataset);
	release(dataset);
	dataset = ReadStaticSkipListFromFile(PATH_ROOT SKIP_ROOT FILE5 ".staticskiplist");
	release(dataset);
	dataset = buildStaticSkipList(PATH_ROOT FILE6);
	writeStaticSkipListToFile(PATH_ROOT SKIP_ROOT FILE6 ".staticskiplist", dataset);
	release(dataset);
	dataset = ReadStaticSkipListFromFile(PATH_ROOT SKIP_ROOT FILE6 ".staticskiplist");
	release(dataset);
	dataset = buildStaticSkipListForFiles({ PATH_ROOT FILE1 , PATH_ROOT FILE3 });
	writeStaticSkipListToFile(PATH_ROOT SKIP_ROOT "all_processed_by_jieba.csv" ".staticskiplist", dataset);
	release(dataset);
	dataset = ReadStaticSkipListFromFile(PATH_ROOT SKIP_ROOT "all_processed_by_jieba.csv" ".staticskiplist");
	release(dataset);
	dataset = buildStaticSkipListForFiles({ PATH_ROOT FILE2 , PATH_ROOT FILE4 });
	writeStaticSkipListToFile(PATH_ROOT SKIP_ROOT "all_processed_by_pkuseg.csv" ".staticskiplist", dataset);
	release(dataset);
	dataset = ReadStaticSkipListFromFile(PATH_ROOT SKIP_ROOT "all_processed_by_pkuseg.csv" ".staticskiplist");
	release(dataset);
	dataset = buildStaticSkipListForFiles({ PATH_ROOT FILE5 , PATH_ROOT FILE6 });
	writeStaticSkipListToFile(PATH_ROOT SKIP_ROOT "all_given.csv" ".staticskiplist", dataset);
	release(dataset);
	dataset = ReadStaticSkipListFromFile(PATH_ROOT SKIP_ROOT "all_given.csv" ".staticskiplist");
	release(dataset);
}


int main()
{
	dataGenStringSkipList();
	//auto dataset = buildStringSkipList(PATH_ROOT FILE);
	//writeStringSkipListToFile(PATH_ROOT SKIP_ROOT FILE SKIP_SUFFIX, dataset);

}