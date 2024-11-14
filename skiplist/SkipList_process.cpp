#include "FileAnalyser.h"
#include "SkipList.h"
#include "StaticSkipList.h"
#include "StringSkipList.h"
#include "Util.h"
#include <map>
#include <set>
#include <string>
#include<iostream>
#include <vector>
#include <fstream>
#include<Windows.h>
#include <locale>
#include <algorithm>
#include <codecvt>
#pragma execution_character_set("utf-8")
using namespace std;

#define FILE1 R"(book_processed_by_jieba.csv)"
#define FILE2 R"(book_processed_by_pkuseg.csv)"
#define FILE3 R"(movie_processed_by_jieba.csv)"
#define FILE4 R"(movie_processed_by_pkuseg.csv)"
#define FILE5 R"(selected_book_top_1200_data_tag.csv)"
#define FILE6 R"(selected_movie_top_1200_data_tag.csv)"
#define PATH_ROOT R"(D:\web_info\data\)"
#define SKIP_ROOT R"(skiplist\)"
#define FILE FILE6

void PrintStringSkipList(std::map< std::string, StringSkipList*> skipListMap)//输出100个示例
{

	int count = 0;
	for (const auto& pair : skipListMap) {
		if (count < 100) {
			const std::string& label = pair.first;
			auto skipList = pair.second;
			std::cout << "Label: " << label << std::endl;
			std::cout << "Skip List Contents:" << std::endl;

			int entryCount = skipList->getEntryCount();
			//auto l = skipList;
			auto p = skipList->getNext();

			while (p != -1)
			{
				std::cout << skipList->getValue(p) << std::endl;
				p = skipList->getNext(p);
			}
			std::cout << "---------------------" << std::endl;
			count++;
		}
		else
			break;
	}
}

void PrintStringSkipList2(std::map< std::string, StringSkipList*> skipListMap)//输出100个示例
{

	int count = 0;
	for (const auto& pair : skipListMap) {
		if (count < 100) {
			const std::string& label = pair.first;
			auto skipList = pair.second;
			std::cout << "Label: " << "复杂查询" << std::endl;
			std::cout << "Skip List Contents:" << std::endl;

			int entryCount = skipList->getEntryCount();
			//auto l = skipList;
			auto p = skipList->getNext();

			while (p != -1)
			{
				std::cout << skipList->getValue(p) << std::endl;
				p = skipList->getNext(p);
			}
			std::cout << "---------------------" << std::endl;
			count++;
		}
		else
			break;
	}
}


std::string Utf8ToGb2312(const std::string& utf8Str)//utf-8字符串转gb2312字符串（好像用不到了）
{
	int wLen = MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, nullptr, 0);
	if (wLen == 0) {
		return "";
	}

	std::wstring wideStr(wLen, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8Str.c_str(), -1, &wideStr[0], wLen);

	int gbLen = WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (gbLen == 0) {
		return "";
	}

	std::string gb2312Str(gbLen, 0);
	WideCharToMultiByte(CP_ACP, 0, wideStr.c_str(), -1, &gb2312Str[0], gbLen, nullptr, nullptr);

	return gb2312Str;
}

void PrintSkipListForLabel(const std::map<std::string, StringSkipList*>& skipListMap, const std::string& label) {
	
	//std::string gb2312Label = Utf8ToGb2312(label);
	//std::cout << gb2312Label << std::endl;
	auto it = skipListMap.find(label);
	if (it == skipListMap.end()){
		std::cout << "Label " << label <<" Not Found!" << std::endl;
		return;
	}

	const std::string& foundLabel = it->first;
	auto skipList = it->second;

	std::cout << "Label: " << foundLabel << std::endl;
	std::cout << "Skip List Contents:" << std::endl;

	int p = skipList->getNext();
	while (p != -1) {
		std::cout << skipList->getValue(p) << std::endl;
		p = skipList->getNext(p);
	}
	std::cout << "---------------------" << std::endl;
}

int GetNumOfValueForLabel(const std::map<std::string, StringSkipList*>& skipListMap, const std::string& label){
	auto it = skipListMap.find(label);
	if (it == skipListMap.end()) {
		std::cout << "Label " << label << " Not Found!" << std::endl;
		return 0;
	}
	int count = 0;
	const std::string& foundLabel = it->first;
	auto skipList = it->second;

	//std::cout << "Label: " << foundLabel << std::endl;
	//std::cout << "Skip List Contents:" << std::endl;

	int p = skipList->getNext();
	while (p != -1) {
		count++;
		//std::cout << skipList->getValue(p) << std::endl;
		p = skipList->getNext(p);
	}
	//std::cout << "---------------------" << std::endl;
	return count;
}

map<string, StringSkipList*>* SignleLabelSkipList(std::map<std::string, StringSkipList*>& skipListMap, const std::string& label)//返回单个label对应的跳表，用于NOT＋label
{
	//set<int> s;
	//s.insert(0);
	//auto null_skiplist = StringSkipList(s);//只有一个0的跳表，如果没查找到则返回其
	auto it = skipListMap.find(label);
	
	if (it == skipListMap.end()) {
		std::cout << "Label " << label << " Not Found!" << std::endl;
		return nullptr;
	}

	const std::string& foundLabel = it->first;
	auto skipList = it->second;
	std::set<int> set;
	// 遍历跳表1，填充 set
	auto index = skipList->getNext();
	while (index != -1) {
		auto value = skipList->getValue(index);
		set.insert(value);
		index = skipList->getNext(index);
	}
	/*auto SingleSkipList = StringSkipList(set);
	return SingleSkipList;*/
	map<string, StringSkipList*>* ret = new map<string, StringSkipList*>();
	StringSkipList* l = new StringSkipList(set);
	(*ret)[label] = l;
	return ret;
}

// 函数：根据用户输入的两个标签，查找对应的跳表并输出共同元素（交集）（即AND连接两个标签）
map<string, StringSkipList*>* PrintCommonSkipListElements(std::map<std::string, StringSkipList*>& skipListMap, const std::string& label1, const std::string& label2) {
	//set<int> s;
	//s.insert(0);
	//auto null_skiplist = StringSkipList(s);//只有一个0的跳表，如果没查找到则返回其
	// 查找 label1 对应的跳表
	auto it1 = skipListMap.find(label1);
	
	if (it1 == skipListMap.end()) {
		std::cout << "Label " << label1 << " not found!" << std::endl;
		return nullptr;
	}

	// 查找 label2 对应的跳表
	auto it2 = skipListMap.find(label2);
	if (it2 == skipListMap.end()) {
		std::cout << "Label " << label2 << " not found!" << std::endl;
		return nullptr;
	}

	// 获取跳表1和跳表2
	auto skipList1 = it1->second;
	auto skipList2 = it2->second;

	// 使用集合（set）来存储跳表中的元素
	std::set<int> set1;
	std::set<int> set2;

	// 遍历跳表1，填充 set1
	auto index1 = skipList1->getNext();
	while (index1 != -1) {
		auto value = skipList1->getValue(index1);
		if (value != -1) {
			set1.insert(value);
		}
		index1 = skipList1->getNext(index1);
	}

	// 遍历跳表2，填充 set2
	auto index2 = skipList2->getNext();
	while (index2 != -1) {
		int value = skipList2->getValue(index2);
		if (value != -1) {
			set2.insert(value);
		}
		index2 = skipList2->getNext(index2);
	}

	// 找出交集：set1 和 set2 的共同元素
	std::set<int> commonElements;
	std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::inserter(commonElements, commonElements.begin()));

	// 输出共同元素
	if (commonElements.empty()) {
		std::cout << "No common elements between " << label1 << " and " << label2 << std::endl;
		return nullptr;
	}
	else {
		std::cout << "Common elements between " << label1 << " and " << label2 << ":" << std::endl;
		for (int element : commonElements) {
			std::cout << element << std::endl;
		}
	}
	//返回跳表
	map<string, StringSkipList*>* ret = new map<string, StringSkipList*>();
	StringSkipList* l = new StringSkipList(commonElements);
	(*ret)[label1 + "_" + label2] = l;
	return ret;
}


// 函数：根据用户输入的两个标签，查找对应的跳表并输出所有元素（并集）（即OR连接两个标签）
map<string, StringSkipList*>* PrintUnionSkipListElements(std::map<std::string, StringSkipList*>& skipListMap, const std::string& label1, const std::string& label2) {
	// 查找 label1 对应的跳表
	auto it1 = skipListMap.find(label1);
	//set<int> s;
	//s.insert(0);
	//auto null_skiplist = StringSkipList(s);//只有一个0的跳表，如果没查找到则返回其
	if (it1 == skipListMap.end()) {
		std::cout << "Label " << label1 << " not found!" << std::endl;
		return nullptr;
	}

	// 查找 label2 对应的跳表
	auto it2 = skipListMap.find(label2);
	if (it2 == skipListMap.end()) {
		std::cout << "Label " << label2 << " not found!" << std::endl;
		return nullptr;
	}

	// 获取跳表1和跳表2
	auto skipList1 = it1->second;
	auto skipList2 = it2->second;

	// 使用集合（set）来存储跳表中的元素
	std::set<int> set1;
	std::set<int> set2;

	// 遍历跳表1，填充 set1
	auto index1 = skipList1->getNext();
	while (index1 != -1) {
		auto value = skipList1->getValue(index1);
		set1.insert(value);
		index1 = skipList1->getNext(index1);
	}

	// 遍历跳表2，填充 set2
	auto index2 = skipList2->getNext();
	while (index2 != -1) {
		int value = skipList2->getValue(index2);
		set2.insert(value);
		index2 = skipList2->getNext(index2);
	}

	// 找出交集：set1 和 set2 的共同元素
	std::set<int> unionElements;
	std::set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), std::inserter(unionElements, unionElements.begin()));

	// 输出共同元素
	if (unionElements.empty()) {
		std::cout << "No union elements between " << label1 << " and " << label2 << std::endl;
		return nullptr;
	}
	else {
		std::cout << "Union elements between " << label1 << " and " << label2 << ":" << std::endl;
		for (int element : unionElements) {
			std::cout << element << std::endl;
		}
	}
	//返回跳表
	//*auto unionSkipList = StringSkipList(unionElements);
	//return unionSkipList;
	map<string, StringSkipList*>* ret = new map<string, StringSkipList*>();
	StringSkipList* l = new StringSkipList(unionElements);
	(*ret)[label1 + "_" + label2] = l;
	return ret;
}

// 查找多个标签对应的跳表并输出共同元素（交集）的函数
std::map<std::string, StringSkipList*>* PrintCommonSkipListElements(std::map<std::string, StringSkipList*>& skipListMap, const std::vector<std::string>& labels) {
	if (labels.empty()) {
		std::cout << "No labels provided!" << std::endl;
		return nullptr;
	}

	std::vector<StringSkipList*> skipLists;
	for (const auto& label : labels) {
		auto it = skipListMap.find(label);
		if (it == skipListMap.end()) {
			std::cout << "Label " << label << " not found!" << std::endl;
			return nullptr;
		}
		skipLists.push_back(it->second);
	}

	std::set<int> commonElements;
	bool firstIteration = true;

	for (auto skipList : skipLists) {
		std::set<int> currentSet;
		auto index = skipList->getNext();
		while (index != -1) {
			auto value = skipList->getValue(index);
			if (value != -1) {
				currentSet.insert(value);
			}
			index = skipList->getNext(index);
		}

		if (firstIteration) {
			commonElements = currentSet;
			firstIteration = false;
		}
		else {
			std::set<int> temp;
			std::set_intersection(commonElements.begin(), commonElements.end(), currentSet.begin(), currentSet.end(), std::inserter(temp, temp.begin()));
			commonElements = temp;
		}
	}

	//if (commonElements.empty()) {
	//	std::cout << "No common elements among the provided labels." << std::endl;
	//	return nullptr;
	//}
	//else {
	//	//std::cout << "Common elements among the provided labels:" << std::endl;
	//	//for (int element : commonElements) {
	//		//std::cout << element << std::endl;
	//	}
	//}

	// 返回跳表
	map<string, StringSkipList*>* ret = new map<string, StringSkipList*>();
	StringSkipList* l = new StringSkipList(commonElements);
	std::string combinedLabel;
	for (const auto& label : labels) {
		combinedLabel += label + "_";
	}
	combinedLabel.pop_back(); // 移除最后一个下划线
	(*ret)[combinedLabel] = l;
	return ret;
}

// 查找多个标签对应的跳表并输出共同元素（并集）的函数
std::map<std::string, StringSkipList*>* PrintniUnionSkipListElements(std::map<std::string, StringSkipList*>& skipListMap, const std::vector<std::string>& labels) {
	if (labels.empty()) {
		std::cout << "No labels provided!" << std::endl;
		return nullptr;
	}

	std::vector<StringSkipList*> skipLists;
	for (const auto& label : labels) {
		auto it = skipListMap.find(label);
		if (it == skipListMap.end()) {
			std::cout << "Label " << label << " not found!" << std::endl;
			return nullptr;
		}
		skipLists.push_back(it->second);
	}

	std::set<int> unionElements;
	bool firstIteration = true;

	for (auto skipList : skipLists) {
		std::set<int> currentSet;
		auto index = skipList->getNext();
		while (index != -1) {
			auto value = skipList->getValue(index);
			if (value != -1) {
				currentSet.insert(value);
			}
			index = skipList->getNext(index);
		}

		if (firstIteration) {
			unionElements = currentSet;
			firstIteration = false;
		}
		else {
			std::set<int> temp;
			std::set_union(unionElements.begin(), unionElements.end(), currentSet.begin(), currentSet.end(), std::inserter(temp, temp.begin()));
			unionElements = temp;
		}
	}

	//if (commonElements.empty()) {
	//	std::cout << "No common elements among the provided labels." << std::endl;
	//	return nullptr;
	//}
	//else {
	//	//std::cout << "Common elements among the provided labels:" << std::endl;
	//	//for (int element : commonElements) {
	//		//std::cout << element << std::endl;
	//	}
	//}

	// 返回跳表
	map<string, StringSkipList*>* ret = new map<string, StringSkipList*>();
	StringSkipList* l = new StringSkipList(unionElements);
	std::string combinedLabel;
	for (const auto& label : labels) {
		combinedLabel += label + "|";
	}
	combinedLabel.pop_back(); // 移除最后一个|
	(*ret)[combinedLabel] = l;
	return ret;
}

std::map<std::string, StringSkipList*>* RemoveCommonElementsBetweenSkipLists(std::map<std::string, StringSkipList*>& skipListMap1,std::map<std::string, StringSkipList*>& skipListMap2)//NOT操作
{

	// 使用集合（set）来存储 map2 中所有跳表的元素
	std::set<int> elementsInMap2;
	std::set<int> elementsInMap1;
	// 遍历 map2 中的每个跳表，收集所有元素
	for (auto& entry2 : skipListMap2) {
		StringSkipList* skipList2 = entry2.second;

		// 遍历跳表2，填充 elementsInMap2
		auto index2 = skipList2->getNext();
		while (index2 != -1) {
			int value = skipList2->getValue(index2);
			if (value != -1) {
				elementsInMap2.insert(value);
			}
			index2 = skipList2->getNext(index2);
		}
	}

	// 遍历 map1 中的每个跳表
	for (auto& entry1 : skipListMap1) {
		StringSkipList* skipList1 = entry1.second;

		// 遍历跳表1，找到与 map2 中跳表交集的元素
		auto index1 = skipList1->getNext();
		while (index1 != -1) {
			int value = skipList1->getValue(index1);
			if (value != -1 ) {
				elementsInMap1.insert(value);
			}
			index1 = skipList1->getNext(index1);
		}
	}
	// 用来存储差集（set1中存在但set2中没有的元素）
	std::set<int> result;

	// 使用set_difference来删除set1中存在于set2的元素
	std::set_difference(elementsInMap1.begin(), elementsInMap1.end(), elementsInMap2.begin(), elementsInMap2.end(),
		std::inserter(result, result.begin()));
	// 返回跳表
	map<string, StringSkipList*>* ret = new map<string, StringSkipList*>();
	StringSkipList* l = new StringSkipList(result);
	std::string combinedLabel;
	combinedLabel = "NOT";
	(*ret)[combinedLabel] = l;
	return ret;
}

std::map<std::string, StringSkipList*>* MergeCommonElementsBetweenSkipLists(std::map<std::string, StringSkipList*>& skipListMap1, std::map<std::string, StringSkipList*>& skipListMap2)//NOT操作
{

	// 使用集合（set）来存储 map2 中所有跳表的元素
	std::set<int> elementsInMap2;
	std::set<int> elementsInMap1;
	// 遍历 map2 中的每个跳表，收集所有元素
	for (auto& entry2 : skipListMap2) {
		StringSkipList* skipList2 = entry2.second;

		// 遍历跳表2，填充 elementsInMap2
		auto index2 = skipList2->getNext();
		while (index2 != -1) {
			int value = skipList2->getValue(index2);
			if (value != -1) {
				elementsInMap2.insert(value);
			}
			index2 = skipList2->getNext(index2);
		}
	}

	// 遍历 map1 中的每个跳表
	for (auto& entry1 : skipListMap1) {
		StringSkipList* skipList1 = entry1.second;

		// 遍历跳表1，找到与 map2 中跳表交集的元素
		auto index1 = skipList1->getNext();
		while (index1 != -1) {
			int value = skipList1->getValue(index1);
			if (value != -1 ) {
				elementsInMap1.insert(value);
			}
			index1 = skipList1->getNext(index1);
		}
	}
	// 用来存储交集元素的set
	std::set<int> commonElements;

	// 使用set_intersection来查找交集
	std::set_intersection(elementsInMap1.begin(), elementsInMap1.end(), elementsInMap2.begin(), elementsInMap2.end(),
		std::inserter(commonElements, commonElements.begin()));
	// 返回跳表
	map<string, StringSkipList*>* ret = new map<string, StringSkipList*>();
	StringSkipList* l = new StringSkipList(commonElements);
	std::string combinedLabel;
	combinedLabel = "AND";
	(*ret)[combinedLabel] = l;
	return ret;
}


// 合并两个 map 的函数
std::map<std::string, StringSkipList*>* MergeSkipListMaps(const std::map<std::string, StringSkipList*>& map1, const std::map<std::string, StringSkipList*>& map2) {
	auto* mergedMap = new std::map<std::string, StringSkipList*>(map1);

	for (const auto& pair : map2) {
		mergedMap->insert(pair);
	}

	return mergedMap;
}


// 获取标签列表的函数，直到输入空行表示结束
void getLabels(const std::string& prompt, std::vector<std::string>& labels) {
	std::string label;
	std::cout << prompt << "请输入标签（按两次回车结束输入）：" << std::endl;
	while (true) {
		std::getline(std::cin, label);  // 获取一行输入
		if (label.empty()) {
			break;  // 空行表示结束输入
		}
		labels.push_back(label);  // 非空行添加到标签列表
	}
}

std::map<std::string, StringSkipList*>* OR_process(std::map<std::string, StringSkipList*>& dataset ,const std::vector<std::string>& orlabel, const std::vector<int>& ornum) {
	// 创建一个索引向量，用于排序
	std::vector<size_t> indices(ornum.size());
	for (size_t i = 0; i < indices.size(); ++i) {
		indices[i] = i;
	}

	// 根据ornum的值对索引进行排序
	std::sort(indices.begin(), indices.end(), [&](size_t a, size_t b) {
		return ornum[a] < ornum[b];
		});


	map<string, StringSkipList*>* dataset2= new map<string, StringSkipList*>();
	map<string, StringSkipList*>* dataset3 = new map<string, StringSkipList*>();
	map<string, StringSkipList*>* dataset4 = new map<string, StringSkipList*>();
	// 按照排序后的索引处理orlabel中的元素
	int count = 0;
	for (size_t i = 0; i < indices.size(); i += 2) {
		if (i + 1 < indices.size()) {
			
			if (count % 2 == 0){
				dataset2 = PrintUnionSkipListElements(dataset, orlabel[indices[i]], orlabel[indices[i + 1]]);
				count++; }
			if (count % 2 == 1) {
				dataset3 = PrintUnionSkipListElements(dataset, orlabel[indices[i]], orlabel[indices[i + 1]]);
				count++;
			}
			if (count>1) {
				dataset4=MergeSkipListMaps(*dataset2, *dataset3);
			}
		}
		else {
			// 如果只有一个元素剩余，单独处理
			dataset2 = SignleLabelSkipList(dataset, orlabel[indices[i]]);
				dataset4 = MergeSkipListMaps(*dataset2, *dataset4);
		}
	}
	delete dataset2;
	delete dataset3;
	return dataset4;
}


// 取两个 map 的交集的函数
std::map<std::string, StringSkipList*>* IntersectSkipListMaps(const std::map<std::string, StringSkipList*>& map1, const std::map<std::string, StringSkipList*>& map2) {
	auto* intersectedMap = new std::map<std::string, StringSkipList*>();

	for (const auto& pair1 : map1) {
		auto it2 = map2.find(pair1.first);
		if (it2 != map2.end()) {
			// 找到共同的标签，将对应的跳表插入到新的 map 中
			(*intersectedMap)[pair1.first] = pair1.second;
		}
	}

	return intersectedMap;
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	int choice;
	auto dataset = ReadStringSkipListFromFile(PATH_ROOT SKIP_ROOT FILE1 ".stringskiplist");
	std::cout << "您好！" << std::endl;
	std::cout << "请选择以下操作：" << std::endl;
	while (true) {
		// 菜单展示
		std::cout << "\n请选择一个功能:\n";
		std::cout << "1. 输出100个<标签, 编号>的示例\n";
		std::cout << "2. 查询单个标签对应的书籍或电影\n";
		std::cout << "3. 查询符合两个标签的书籍或电影 (A AND B)\n";
		std::cout << "4. 查询符合两个标签的书籍或电影 (A OR B)\n";
		std::cout << "5. 复杂查询\n";
		std::cout << "0. 退出\n";
		std::cout << "请输入选项: ";
		std::cin >> choice;
		std::string userLabel;
		std::string labe2;
		std::string label;
		std::vector<std::string> andLabels, orLabels, notLabels;
		std::vector<int> andnum, ornum, notnum;
		int andcount = 0;
			int orcount = 0;
			int notcount = 0;
		std::cin.ignore();  // 清除输入缓冲区中的换行符

		switch (choice) {
		case 1:
			std::cout << "输出100个 <标签,编号> 示例如下：" << std::endl;//输出100个 <标签,编号> 示例如下：
			PrintStringSkipList(*dataset);
			break;
		
		case 2: {
			std::cout << "请输入您想要查询的标签：" << std::endl;
			std::cin >> userLabel;
			PrintSkipListForLabel(*dataset, userLabel);
			
			break;
		}
		case 3: {

			std::cout << "请输入标签1：" << std::endl;
			std::cin >> label;

			std::cout << "请输入标签2：" << std::endl;
			std::cin >> labe2;
			// 查询并输出交集
			auto l=PrintCommonSkipListElements(*dataset, label, labe2);
			delete l;
			std::cout << "输出完毕！" << std::endl;
			
			break;
		}
		case 4: {
			std::cout << "请输入标签1：" << std::endl;
			std::cin >> label;

			std::cout << "请输入标签2：" << std::endl;
			std::cin >> labe2;

			// 查询并输出并集
			auto l=PrintUnionSkipListElements(*dataset, label, labe2);
			std::cout << "输出完毕！" << std::endl;
			delete l;
			break;
		}
		case 5: {
			// 获取AND标签
			std::cout << "好的，此项支持复杂查询！将提供AND、OR以及NOT操作"<<endl<<"例如，您想查询中国当代青春或校园的书籍，但不能出现血腥元素；则可以在AND处输入 中国 当代，在OR处输入 校园 青春，在NOT处输入 血腥" << std::endl;
			//则为"(中国&当代)&(青春 or 校园) but NOT血腥"
			getLabels("AND:", andLabels);
			/*for (size_t i = 0; i < andLabels.size(); ++i) {
				andnum[i] = GetNumOfValueForLabel(*dataset, andLabels[i]);
				std::cout << "andLabels[" << i << "]: " << andLabels[i] <<"对应的书籍(或电影)有"<<andnum[i]<< std::endl;
				andcount += andnum[i];
			}*/
			// 获取OR标签
			getLabels("OR:", orLabels);
			//for (size_t i = 0; i < orLabels.size(); ++i) {
				/*ornum[i] = GetNumOfValueForLabel(*dataset, orLabels[i]);
				std::cout << "orLabels[" << i << "]: " << orLabels[i] << "对应的书籍(或电影)有" << ornum[i]<< std::endl;
				orcount += ornum[i];*/
			//}
			// 获取NOT标签
			getLabels("BUT NOT:", notLabels);
			//for (size_t i = 0; i < notLabels.size(); ++i) {
				/*notnum[i] = GetNumOfValueForLabel(*dataset, notLabels[i]);
				std::cout << "notLabels[" << i << "]: " << notLabels[i] << "对应的书籍(或电影)有" <<notnum[i] << std::endl;
				notcount += notnum[i];*/
			//}
			map<string, StringSkipList*>* dataset2 = new map<string, StringSkipList*>();
			map<string, StringSkipList*>* dataset3 = new map<string, StringSkipList*>();
			map<string, StringSkipList*>* dataset4 = new map<string, StringSkipList*>();
			map<string, StringSkipList*>* dataset5 = new map<string, StringSkipList*>();
			//dataset2 = OR_process(*dataset, orLabels, ornum);
			dataset2 = PrintniUnionSkipListElements(*dataset, orLabels);
			std::cout << "你好1" << std::endl;
			dataset3 = PrintCommonSkipListElements(*dataset, andLabels);
			std::cout << "你好2" << std::endl;
			dataset4 = MergeCommonElementsBetweenSkipLists(*dataset2, *dataset3);
			std::cout << "你好3" << std::endl;
			for (size_t i = 0; i < notLabels.size(); ++i) {
				dataset5 = SignleLabelSkipList(*dataset, notLabels[i]);
				std::cout << "你好4" << std::endl;
				dataset4 = RemoveCommonElementsBetweenSkipLists(*dataset4, *dataset5);
				std::cout << "你好5" << std::endl;
			}
			PrintStringSkipList2(*dataset4);
			delete dataset2;
			delete dataset3;
			delete dataset4;
			delete dataset5;
			break;
		}
		case 0:
			exit(0);  // 退出整个程序
		default:
			cout << "你输入的是：" << choice << endl;
			break;
		}
	}
	

	// 清理内存
	for (auto& pair : *dataset) {
		delete pair.second;
	}
	system("pause");
	return 0;
}