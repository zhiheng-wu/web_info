#include "InvertedIndex.h"
using namespace std;

// 每个索引大小
//unsigned char entrylen;
// 字符串长度
//unsigned char strlen;
// 索引总大小
//int indexlen;
// 索引, [关键词，跳表序号]
//unsigned char* indexes;
// 内存缓存的跳表
//StringSkipList** lists;
// 每个跳表在文件中位置
//int* listPointerInFile;

static short limitedstrcpy(const int& limit, const char* limitstr, const char* str)
{
	int i;
	for (i = 0; i < limit; i++)
	{
		unsigned char cl = limitstr[i];
		unsigned char cs = str[i];
		if (cl != cs)return (short)cl - cs;
		else if (cl == 0)return 0;
	}
	if (str[i] == 0)
		return 0;
	return -(short)(unsigned char)str[i];
}

void InvertedIndex::writeToFile(std::map<std::string, StringSkipList*>* mapData, const char* path)
{
	int indexcount = mapData->size();
	unsigned char strlen = 0;
	int lbegin = 0;
	int idx = 0;
	int* listPointerInFile = new int[indexcount];
	StringSkipList** lists = new StringSkipList * [indexcount];
	for (auto& i : *mapData)
	{
		if (i.first.size() > strlen)
			strlen = i.first.size();
		lists[idx] = i.second;
		listPointerInFile[idx] = lbegin;
		lbegin += i.second->getSize() * sizeof(int);
		idx++;
	}
	int indexlen = strlen * indexcount;
	char* indexes = new char[indexlen] {};
	idx = 0;
	for (auto& i : *mapData)
	{
		int  s = i.first.size();
		memcpy(&indexes[idx * strlen], i.first.c_str(), s);
		if (s < strlen)
			indexes[idx * strlen + s] = 0;
		idx++;
	}
	ofstream ost;
	ost.open(path, ios::out | ios::binary);
	ost.write((const char*)&strlen, sizeof(strlen));
	ost.write((const char*)&indexlen, sizeof(indexlen));
	ost.write((const char*)indexes, sizeof(char) * indexlen);
	ost.write((const char*)listPointerInFile, sizeof(int) * indexcount);
	for (int i = 0; i < indexcount; i++)
	{
		lists[i]->writeToFile(ost);
	}
	ost.close();
	delete[] indexes;
	delete[] listPointerInFile;
	delete[] lists;
}

InvertedIndex::~InvertedIndex()
{
	if (stream)
	{
		stream->close();
		delete stream;
	}
	delete[] indexes;
	if (lists)
		for (int i = 0; i < indexcount; i++)
		{
			delete lists[i];
		}
	delete[] lists;
	delete[] listPointerInFile;
}

InvertedIndex::InvertedIndex()
{
	stream = nullptr;
	strlen = 0;
	indexlen = 0;
	indexcount = 0;
	indexes = 0;
	lists = 0;
	listPointerInFile = 0;
}

void InvertedIndex::writeIndexToFile(const char* path)
{
	ofstream ost;
	ost.open(path, ios::out | ios::binary);
	ost.write((const char*)&strlen, sizeof(strlen));
	ost.write((const char*)&indexlen, sizeof(indexlen));
	ost.write((const char*)indexes, sizeof(char) * indexlen);
	ost.write((const char*)listPointerInFile, sizeof(int) * indexcount);
	ost.close();
}

InvertedIndex* InvertedIndex::readFromFile(const char* path)
{
	InvertedIndex* d = new InvertedIndex();
	InvertedIndex& dataset = *d;
	dataset.stream = new ifstream();
	ifstream& ist = *dataset.stream;
	ist.open(path, ios::in | ios::binary);
	ist.read((char*)&dataset.strlen, sizeof(unsigned char));
	ist.read((char*)&dataset.indexlen, sizeof(int));
	dataset.indexcount = dataset.indexlen / dataset.strlen;
	dataset.indexes = new char[dataset.indexlen];
	ist.read((char*)dataset.indexes, sizeof(char) * dataset.indexlen);
	dataset.listPointerInFile = new int[dataset.indexcount];
	ist.read((char*)dataset.listPointerInFile, sizeof(int) * dataset.indexcount);
	dataset.lists = new StringSkipList * [dataset.indexcount];
	int e = dataset.indexcount;
	for (int i = 0; i < e; i++)
	{
		dataset.lists[i] = nullptr;
	}
	dataset.streamBaseIdx = sizeof(unsigned char) + sizeof(int) + sizeof(char) * dataset.indexlen + sizeof(int) * dataset.indexcount;
	return d;
}

int InvertedIndex::contains(const std::string& str)
{
	const char* cstr = str.c_str();
	int left = 0;
	int right = indexcount - 1;
	int mid = right / 2;
	while (1)
	{
		short o = limitedstrcpy(strlen, indexes + mid * strlen, cstr);
		if (o > 0)
		{
			if (left == right || left > mid - 1)return -1;
			right = mid - 1;
			mid = (left + right) / 2;
		}
		else if (o < 0)
		{
			if (left == right || left > mid - 1)return -1;
			left = mid + 1;
			mid = (left + right) / 2;
		}
		else return mid;
	}
}

int InvertedIndex::contains(const char* str)
{
	int left = 0;
	int right = indexcount - 1;
	int mid = right / 2;
	while (1)
	{
		short o = limitedstrcpy(strlen, indexes + mid * strlen, str);
		if (o > 0)
		{
			if (left == right || left > mid - 1)return -1;
			right = mid - 1;
			mid = (left + right) / 2;
		}
		else if (o < 0)
		{
			if (left == right || left > mid - 1)return -1;
			left = mid + 1;
			mid = (left + right) / 2;
		}
		else return mid;
	}
}

StringSkipList* InvertedIndex::operator[](const int& i)
{
	if (i == -1)return nullptr;
	if (lists[i] != nullptr)return lists[i];
	int off = streamBaseIdx + listPointerInFile[i];
	stream->seekg(off, ios::beg);
	StringSkipList* l = StringSkipList::readFromFile(*stream);
	lists[i] = l;
	return l;
}

StringSkipList* InvertedIndex::operator[](const std::string& str)
{
	return operator[](contains(str));
}

StringSkipList* InvertedIndex::operator[](const char* str)
{
	return operator[](contains(str));
}
