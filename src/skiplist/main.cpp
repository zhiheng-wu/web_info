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
#include "InvertedIndex.h"
#include "TimeCounter.h"
#define GEN_PY 0
#if GEN_PY
#include <pybind11/pybind11.h>
namespace py = pybind11;

void* getDataSet(const char* str)
{
	return InvertedIndex::readFromFile(str);
};

int checkDataSet(void* p)
{
	return p != NULL;
};

void searchInDataSet(const char* input, const char* outputPath, void* dataset)
{
	std::ofstream f;
	f.open(outputPath, std::ios::out);
	f << search(input, (InvertedIndex*)dataset);
	f.close();
};

void releaseDataSet(void* dataset)
{
	auto d = (InvertedIndex*)dataset;
	delete d;
};

PYBIND11_MODULE(stringskiplist, m) {
	m.def("getDataSet", &getDataSet, R"pbdoc(
        Get dataset from file. Parameter(FilePath), Return(DataSet).
    )pbdoc");
	m.def("checkDataSet", &checkDataSet, R"pbdoc(
        Check if dataset is exsit. Parameter(DataSet). Output(Bool)
    )pbdoc");
	m.def("searchInDataSet", &searchInDataSet, R"pbdoc(
        Search input equation in dataset, put output to outputPath. Parameter(Input,OutputPath,DataSet).
    )pbdoc");
	m.def("releaseDataSet", &releaseDataSet, R"pbdoc(
        Release dataset. Parameter(DataSet).
    )pbdoc");

#ifdef VERSION_INFO
	m.attr("__version__") = VERSION_INFO;
#else
	m.attr("__version__") = "dev";
#endif
}

#else
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
};

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
};

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
};

#define FILE1 R"(book_processed_by_jieba)"
#define FILE2 R"(book_processed_by_pkuseg)"
#define FILE3 R"(movie_processed_by_jieba)"
#define FILE4 R"(movie_processed_by_pkuseg)"
#define FILEJ R"(all_processed_by_jieba)"
#define FILEP R"(all_processed_by_pkuseg)"
#define PATH_ROOT R"(..\..\data\)"
#define II_ROOT R"(invertedindex\)"
#define CSV_SUFFIX R"(.csv)"
#define SKL_SUFFIX R"(.skl)"
#define II_SUFFIX R"(.ii)"

void* getDataSet(const char* str)
{
	return InvertedIndex::readFromFile(str);
};

int checkDataSet(void* p)
{
	return p != NULL;
};

void searchInDataSet(const char* input, const char* outputPath, void* dataset)
{
	std::ofstream f;
	f.open(outputPath, std::ios::out);
	f << search(input, (InvertedIndex*)dataset);
	f.close();
};

void releaseDataSet(void* dataset)
{
	auto d = (InvertedIndex*)dataset;
	delete d;
};

void dataGenStringSkipList()
{
	auto dataset = buildStringSkipList(PATH_ROOT FILE1 CSV_SUFFIX);
	writeStringSkipListToFile(PATH_ROOT II_ROOT FILE1 SKL_SUFFIX, dataset);
	release(dataset);
	dataset = ReadStringSkipListFromFile(PATH_ROOT II_ROOT FILE1 SKL_SUFFIX);
	release(dataset);
	dataset = buildStringSkipList(PATH_ROOT FILE2 CSV_SUFFIX);
	writeStringSkipListToFile(PATH_ROOT II_ROOT FILE2 SKL_SUFFIX, dataset);
	release(dataset);
	dataset = ReadStringSkipListFromFile(PATH_ROOT II_ROOT FILE2 SKL_SUFFIX);
	release(dataset);
	dataset = buildStringSkipList(PATH_ROOT FILE3 CSV_SUFFIX);
	writeStringSkipListToFile(PATH_ROOT II_ROOT FILE3 SKL_SUFFIX, dataset);
	release(dataset);
	dataset = ReadStringSkipListFromFile(PATH_ROOT II_ROOT FILE3 SKL_SUFFIX);
	release(dataset);
	dataset = buildStringSkipList(PATH_ROOT FILE4 CSV_SUFFIX);
	writeStringSkipListToFile(PATH_ROOT II_ROOT FILE4 SKL_SUFFIX, dataset);
	release(dataset);
	dataset = ReadStringSkipListFromFile(PATH_ROOT II_ROOT FILE4 SKL_SUFFIX);
	release(dataset);
	dataset = buildStringSkipListForFiles({ PATH_ROOT FILE1 CSV_SUFFIX , PATH_ROOT FILE3 CSV_SUFFIX });
	writeStringSkipListToFile(PATH_ROOT II_ROOT FILEJ SKL_SUFFIX, dataset);
	release(dataset);
	dataset = ReadStringSkipListFromFile(PATH_ROOT II_ROOT FILEJ SKL_SUFFIX);
	release(dataset);
	dataset = buildStringSkipListForFiles({ PATH_ROOT FILE2 CSV_SUFFIX , PATH_ROOT FILE4 CSV_SUFFIX });
	writeStringSkipListToFile(PATH_ROOT II_ROOT FILEP SKL_SUFFIX, dataset);
	release(dataset);
	dataset = ReadStringSkipListFromFile(PATH_ROOT II_ROOT FILEP SKL_SUFFIX);
	release(dataset);
};

void dataGenInvertedIndex()
{
	auto skl = ReadStringSkipListFromFile(PATH_ROOT II_ROOT FILE1 SKL_SUFFIX);
	InvertedIndex::writeToFile(skl, PATH_ROOT II_ROOT FILE1 II_SUFFIX);
	release(skl);
	auto dataset = InvertedIndex::readFromFile(PATH_ROOT II_ROOT FILE1 II_SUFFIX);
	delete dataset;
	skl = ReadStringSkipListFromFile(PATH_ROOT II_ROOT FILE2 SKL_SUFFIX);
	InvertedIndex::writeToFile(skl, PATH_ROOT II_ROOT FILE2 II_SUFFIX);
	release(skl);
	dataset = InvertedIndex::readFromFile(PATH_ROOT II_ROOT FILE2 II_SUFFIX);
	delete dataset;
	skl = ReadStringSkipListFromFile(PATH_ROOT II_ROOT FILE3 SKL_SUFFIX);
	InvertedIndex::writeToFile(skl, PATH_ROOT II_ROOT FILE3 II_SUFFIX);
	release(skl);
	dataset = InvertedIndex::readFromFile(PATH_ROOT II_ROOT FILE3 II_SUFFIX);
	delete dataset;
	skl = ReadStringSkipListFromFile(PATH_ROOT II_ROOT FILE4 SKL_SUFFIX);
	InvertedIndex::writeToFile(skl, PATH_ROOT II_ROOT FILE4 II_SUFFIX);
	release(skl);
	dataset = InvertedIndex::readFromFile(PATH_ROOT II_ROOT FILE4 II_SUFFIX);
	delete dataset;
	skl = ReadStringSkipListFromFile(PATH_ROOT II_ROOT FILEJ SKL_SUFFIX);
	InvertedIndex::writeToFile(skl, PATH_ROOT II_ROOT FILEJ II_SUFFIX);
	release(skl);
	dataset = InvertedIndex::readFromFile(PATH_ROOT II_ROOT FILEJ II_SUFFIX);
	delete dataset;
	skl = ReadStringSkipListFromFile(PATH_ROOT II_ROOT FILEP SKL_SUFFIX);
	InvertedIndex::writeToFile(skl, PATH_ROOT II_ROOT FILEP II_SUFFIX);
	release(skl);
	dataset = InvertedIndex::readFromFile(PATH_ROOT II_ROOT FILEP II_SUFFIX);
	delete dataset;
};

#define SEARCH_TARGET R"(('A' OR 'B'))"

int main()
{
	//dataGenInvertedIndex();
	{
		yang::time::TimeCounter t;
		t.Start();
		auto dataset = ReadStringSkipListFromFile(PATH_ROOT II_ROOT FILE1 SKL_SUFFIX);
		string s = search(SEARCH_TARGET, dataset);
		t.End();
		release(dataset);
		cout << t.GetTime().toStdString(1) << endl << s << endl;
	}
	{
		yang::time::TimeCounter t;
		t.Start();
		auto dataset = InvertedIndex::readFromFile(PATH_ROOT II_ROOT FILE1 II_SUFFIX);
		string s = search(SEARCH_TARGET, dataset);
		t.End();
		delete dataset;
		cout << t.GetTime().toStdString(1) << endl << s << endl;
	}
}
#endif