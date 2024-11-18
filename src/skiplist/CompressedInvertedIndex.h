#pragma once

#include "StringSkipList.h"
#include <map>
#include <string>
#include <fstream>
class CompressedInvertedIndex
{
	//倒排表文件指针, 用于缓存未命中时查找
	std::ifstream* stream;
	// 文件流指针基准
	int streamBaseIdx = 0;
	// 词项总大小
	int strslen;
	// 词项总数量
	int strcount;
	// 词项表
	char* strs;
	//索引总数量
	int indexcount;
	// 按块存储索引表, 4 个词项一个索引, [公共前缀,s0s0s0s], 最后一项指到 str 表尾
	int* indexes;
	// 内存缓存的跳表
	StringSkipList** lists;
	// 每个跳表在文件中位置
	int* listPointerInFile;
	//用于 Read 构造
	CompressedInvertedIndex();
	//用于内部比较
	struct StrElement {
		short strE[5];
	};
	StrElement parseStr(int index);
public:
	void writeIndexToFile(const char* path) const;
	// 从 mapData 构建 DataSet, 并写入 File.
	static void writeToFile(std::map<std::string, StringSkipList*>* mapData, const char* path);
	~CompressedInvertedIndex();
	//从文件加载(缓存加载)
	static CompressedInvertedIndex* readFromFile(const char* path);
	//contains
	int contains(const std::string& str);
	int contains(const char* str);
	//使用contains索引查找
	StringSkipList* operator[](const int& i);
	//使用str查找
	StringSkipList* operator[](const std::string& str);
	StringSkipList* operator[](const char* str);
};