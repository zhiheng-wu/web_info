#pragma once
#include "StringSkipList.h"
#include <map>
#include <string>
#include <fstream>
class InvertedIndex
{
	//倒排表文件指针, 用于缓存未命中时查找
	std::ifstream* stream;
	// 文件流指针基准
	int streamBaseIdx = 0;
	// 字符串长度
	unsigned char strlen;
	// 索引总大小
	int indexlen;
	// 索引总数量
	int indexcount;
	// 索引, [关键词], 不需要序号, 因为它蕴含在位置内
	char* indexes;
	// 内存缓存的跳表
	StringSkipList** lists;
	// 每个跳表在文件中位置
	int* listPointerInFile;
	//用于 Read 构造
	InvertedIndex();
public:
	void writeIndexToFile(const char* path);
	// 从 mapData 构建 DataSet, 并写入 File.
	static void writeToFile(std::map<std::string, StringSkipList*>* mapData, const char* path);
	~InvertedIndex();
	//从文件加载(缓存加载)
	static InvertedIndex* readFromFile(const char* path);
	//contains
	int contains(const std::string& str);
	int contains(const char* str);
	//使用contains索引查找
	StringSkipList* operator[](const int& i);
	//使用str查找
	StringSkipList* operator[](const std::string& str);
	StringSkipList* operator[](const char* str);
};