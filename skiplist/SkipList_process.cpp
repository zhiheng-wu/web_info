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

void PrintStringSkipList(std::map< std::string, StringSkipList*> skipListMap)//���100��ʾ��
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

void PrintStringSkipList2(std::map< std::string, StringSkipList*> skipListMap)//���100��ʾ��
{

	int count = 0;
	for (const auto& pair : skipListMap) {
		if (count < 100) {
			const std::string& label = pair.first;
			auto skipList = pair.second;
			std::cout << "Label: " << "���Ӳ�ѯ" << std::endl;
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


std::string Utf8ToGb2312(const std::string& utf8Str)//utf-8�ַ���תgb2312�ַ����������ò����ˣ�
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

map<string, StringSkipList*>* SignleLabelSkipList(std::map<std::string, StringSkipList*>& skipListMap, const std::string& label)//���ص���label��Ӧ����������NOT��label
{
	//set<int> s;
	//s.insert(0);
	//auto null_skiplist = StringSkipList(s);//ֻ��һ��0���������û���ҵ��򷵻���
	auto it = skipListMap.find(label);
	
	if (it == skipListMap.end()) {
		std::cout << "Label " << label << " Not Found!" << std::endl;
		return nullptr;
	}

	const std::string& foundLabel = it->first;
	auto skipList = it->second;
	std::set<int> set;
	// ��������1����� set
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

// �����������û������������ǩ�����Ҷ�Ӧ�����������ͬԪ�أ�����������AND����������ǩ��
map<string, StringSkipList*>* PrintCommonSkipListElements(std::map<std::string, StringSkipList*>& skipListMap, const std::string& label1, const std::string& label2) {
	//set<int> s;
	//s.insert(0);
	//auto null_skiplist = StringSkipList(s);//ֻ��һ��0���������û���ҵ��򷵻���
	// ���� label1 ��Ӧ������
	auto it1 = skipListMap.find(label1);
	
	if (it1 == skipListMap.end()) {
		std::cout << "Label " << label1 << " not found!" << std::endl;
		return nullptr;
	}

	// ���� label2 ��Ӧ������
	auto it2 = skipListMap.find(label2);
	if (it2 == skipListMap.end()) {
		std::cout << "Label " << label2 << " not found!" << std::endl;
		return nullptr;
	}

	// ��ȡ����1������2
	auto skipList1 = it1->second;
	auto skipList2 = it2->second;

	// ʹ�ü��ϣ�set�����洢�����е�Ԫ��
	std::set<int> set1;
	std::set<int> set2;

	// ��������1����� set1
	auto index1 = skipList1->getNext();
	while (index1 != -1) {
		auto value = skipList1->getValue(index1);
		if (value != -1) {
			set1.insert(value);
		}
		index1 = skipList1->getNext(index1);
	}

	// ��������2����� set2
	auto index2 = skipList2->getNext();
	while (index2 != -1) {
		int value = skipList2->getValue(index2);
		if (value != -1) {
			set2.insert(value);
		}
		index2 = skipList2->getNext(index2);
	}

	// �ҳ�������set1 �� set2 �Ĺ�ͬԪ��
	std::set<int> commonElements;
	std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::inserter(commonElements, commonElements.begin()));

	// �����ͬԪ��
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
	//��������
	map<string, StringSkipList*>* ret = new map<string, StringSkipList*>();
	StringSkipList* l = new StringSkipList(commonElements);
	(*ret)[label1 + "_" + label2] = l;
	return ret;
}


// �����������û������������ǩ�����Ҷ�Ӧ�������������Ԫ�أ�����������OR����������ǩ��
map<string, StringSkipList*>* PrintUnionSkipListElements(std::map<std::string, StringSkipList*>& skipListMap, const std::string& label1, const std::string& label2) {
	// ���� label1 ��Ӧ������
	auto it1 = skipListMap.find(label1);
	//set<int> s;
	//s.insert(0);
	//auto null_skiplist = StringSkipList(s);//ֻ��һ��0���������û���ҵ��򷵻���
	if (it1 == skipListMap.end()) {
		std::cout << "Label " << label1 << " not found!" << std::endl;
		return nullptr;
	}

	// ���� label2 ��Ӧ������
	auto it2 = skipListMap.find(label2);
	if (it2 == skipListMap.end()) {
		std::cout << "Label " << label2 << " not found!" << std::endl;
		return nullptr;
	}

	// ��ȡ����1������2
	auto skipList1 = it1->second;
	auto skipList2 = it2->second;

	// ʹ�ü��ϣ�set�����洢�����е�Ԫ��
	std::set<int> set1;
	std::set<int> set2;

	// ��������1����� set1
	auto index1 = skipList1->getNext();
	while (index1 != -1) {
		auto value = skipList1->getValue(index1);
		set1.insert(value);
		index1 = skipList1->getNext(index1);
	}

	// ��������2����� set2
	auto index2 = skipList2->getNext();
	while (index2 != -1) {
		int value = skipList2->getValue(index2);
		set2.insert(value);
		index2 = skipList2->getNext(index2);
	}

	// �ҳ�������set1 �� set2 �Ĺ�ͬԪ��
	std::set<int> unionElements;
	std::set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), std::inserter(unionElements, unionElements.begin()));

	// �����ͬԪ��
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
	//��������
	//*auto unionSkipList = StringSkipList(unionElements);
	//return unionSkipList;
	map<string, StringSkipList*>* ret = new map<string, StringSkipList*>();
	StringSkipList* l = new StringSkipList(unionElements);
	(*ret)[label1 + "_" + label2] = l;
	return ret;
}

// ���Ҷ����ǩ��Ӧ�����������ͬԪ�أ��������ĺ���
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

	// ��������
	map<string, StringSkipList*>* ret = new map<string, StringSkipList*>();
	StringSkipList* l = new StringSkipList(commonElements);
	std::string combinedLabel;
	for (const auto& label : labels) {
		combinedLabel += label + "_";
	}
	combinedLabel.pop_back(); // �Ƴ����һ���»���
	(*ret)[combinedLabel] = l;
	return ret;
}

// ���Ҷ����ǩ��Ӧ�����������ͬԪ�أ��������ĺ���
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

	// ��������
	map<string, StringSkipList*>* ret = new map<string, StringSkipList*>();
	StringSkipList* l = new StringSkipList(unionElements);
	std::string combinedLabel;
	for (const auto& label : labels) {
		combinedLabel += label + "|";
	}
	combinedLabel.pop_back(); // �Ƴ����һ��|
	(*ret)[combinedLabel] = l;
	return ret;
}

std::map<std::string, StringSkipList*>* RemoveCommonElementsBetweenSkipLists(std::map<std::string, StringSkipList*>& skipListMap1,std::map<std::string, StringSkipList*>& skipListMap2)//NOT����
{

	// ʹ�ü��ϣ�set�����洢 map2 �����������Ԫ��
	std::set<int> elementsInMap2;
	std::set<int> elementsInMap1;
	// ���� map2 �е�ÿ�������ռ�����Ԫ��
	for (auto& entry2 : skipListMap2) {
		StringSkipList* skipList2 = entry2.second;

		// ��������2����� elementsInMap2
		auto index2 = skipList2->getNext();
		while (index2 != -1) {
			int value = skipList2->getValue(index2);
			if (value != -1) {
				elementsInMap2.insert(value);
			}
			index2 = skipList2->getNext(index2);
		}
	}

	// ���� map1 �е�ÿ������
	for (auto& entry1 : skipListMap1) {
		StringSkipList* skipList1 = entry1.second;

		// ��������1���ҵ��� map2 ����������Ԫ��
		auto index1 = skipList1->getNext();
		while (index1 != -1) {
			int value = skipList1->getValue(index1);
			if (value != -1 ) {
				elementsInMap1.insert(value);
			}
			index1 = skipList1->getNext(index1);
		}
	}
	// �����洢���set1�д��ڵ�set2��û�е�Ԫ�أ�
	std::set<int> result;

	// ʹ��set_difference��ɾ��set1�д�����set2��Ԫ��
	std::set_difference(elementsInMap1.begin(), elementsInMap1.end(), elementsInMap2.begin(), elementsInMap2.end(),
		std::inserter(result, result.begin()));
	// ��������
	map<string, StringSkipList*>* ret = new map<string, StringSkipList*>();
	StringSkipList* l = new StringSkipList(result);
	std::string combinedLabel;
	combinedLabel = "NOT";
	(*ret)[combinedLabel] = l;
	return ret;
}

std::map<std::string, StringSkipList*>* MergeCommonElementsBetweenSkipLists(std::map<std::string, StringSkipList*>& skipListMap1, std::map<std::string, StringSkipList*>& skipListMap2)//NOT����
{

	// ʹ�ü��ϣ�set�����洢 map2 �����������Ԫ��
	std::set<int> elementsInMap2;
	std::set<int> elementsInMap1;
	// ���� map2 �е�ÿ�������ռ�����Ԫ��
	for (auto& entry2 : skipListMap2) {
		StringSkipList* skipList2 = entry2.second;

		// ��������2����� elementsInMap2
		auto index2 = skipList2->getNext();
		while (index2 != -1) {
			int value = skipList2->getValue(index2);
			if (value != -1) {
				elementsInMap2.insert(value);
			}
			index2 = skipList2->getNext(index2);
		}
	}

	// ���� map1 �е�ÿ������
	for (auto& entry1 : skipListMap1) {
		StringSkipList* skipList1 = entry1.second;

		// ��������1���ҵ��� map2 ����������Ԫ��
		auto index1 = skipList1->getNext();
		while (index1 != -1) {
			int value = skipList1->getValue(index1);
			if (value != -1 ) {
				elementsInMap1.insert(value);
			}
			index1 = skipList1->getNext(index1);
		}
	}
	// �����洢����Ԫ�ص�set
	std::set<int> commonElements;

	// ʹ��set_intersection�����ҽ���
	std::set_intersection(elementsInMap1.begin(), elementsInMap1.end(), elementsInMap2.begin(), elementsInMap2.end(),
		std::inserter(commonElements, commonElements.begin()));
	// ��������
	map<string, StringSkipList*>* ret = new map<string, StringSkipList*>();
	StringSkipList* l = new StringSkipList(commonElements);
	std::string combinedLabel;
	combinedLabel = "AND";
	(*ret)[combinedLabel] = l;
	return ret;
}


// �ϲ����� map �ĺ���
std::map<std::string, StringSkipList*>* MergeSkipListMaps(const std::map<std::string, StringSkipList*>& map1, const std::map<std::string, StringSkipList*>& map2) {
	auto* mergedMap = new std::map<std::string, StringSkipList*>(map1);

	for (const auto& pair : map2) {
		mergedMap->insert(pair);
	}

	return mergedMap;
}


// ��ȡ��ǩ�б�ĺ�����ֱ��������б�ʾ����
void getLabels(const std::string& prompt, std::vector<std::string>& labels) {
	std::string label;
	std::cout << prompt << "�������ǩ�������λس��������룩��" << std::endl;
	while (true) {
		std::getline(std::cin, label);  // ��ȡһ������
		if (label.empty()) {
			break;  // ���б�ʾ��������
		}
		labels.push_back(label);  // �ǿ�����ӵ���ǩ�б�
	}
}

std::map<std::string, StringSkipList*>* OR_process(std::map<std::string, StringSkipList*>& dataset ,const std::vector<std::string>& orlabel, const std::vector<int>& ornum) {
	// ����һ��������������������
	std::vector<size_t> indices(ornum.size());
	for (size_t i = 0; i < indices.size(); ++i) {
		indices[i] = i;
	}

	// ����ornum��ֵ��������������
	std::sort(indices.begin(), indices.end(), [&](size_t a, size_t b) {
		return ornum[a] < ornum[b];
		});


	map<string, StringSkipList*>* dataset2= new map<string, StringSkipList*>();
	map<string, StringSkipList*>* dataset3 = new map<string, StringSkipList*>();
	map<string, StringSkipList*>* dataset4 = new map<string, StringSkipList*>();
	// ������������������orlabel�е�Ԫ��
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
			// ���ֻ��һ��Ԫ��ʣ�࣬��������
			dataset2 = SignleLabelSkipList(dataset, orlabel[indices[i]]);
				dataset4 = MergeSkipListMaps(*dataset2, *dataset4);
		}
	}
	delete dataset2;
	delete dataset3;
	return dataset4;
}


// ȡ���� map �Ľ����ĺ���
std::map<std::string, StringSkipList*>* IntersectSkipListMaps(const std::map<std::string, StringSkipList*>& map1, const std::map<std::string, StringSkipList*>& map2) {
	auto* intersectedMap = new std::map<std::string, StringSkipList*>();

	for (const auto& pair1 : map1) {
		auto it2 = map2.find(pair1.first);
		if (it2 != map2.end()) {
			// �ҵ���ͬ�ı�ǩ������Ӧ��������뵽�µ� map ��
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
	std::cout << "���ã�" << std::endl;
	std::cout << "��ѡ�����²�����" << std::endl;
	while (true) {
		// �˵�չʾ
		std::cout << "\n��ѡ��һ������:\n";
		std::cout << "1. ���100��<��ǩ, ���>��ʾ��\n";
		std::cout << "2. ��ѯ������ǩ��Ӧ���鼮���Ӱ\n";
		std::cout << "3. ��ѯ����������ǩ���鼮���Ӱ (A AND B)\n";
		std::cout << "4. ��ѯ����������ǩ���鼮���Ӱ (A OR B)\n";
		std::cout << "5. ���Ӳ�ѯ\n";
		std::cout << "0. �˳�\n";
		std::cout << "������ѡ��: ";
		std::cin >> choice;
		std::string userLabel;
		std::string labe2;
		std::string label;
		std::vector<std::string> andLabels, orLabels, notLabels;
		std::vector<int> andnum, ornum, notnum;
		int andcount = 0;
			int orcount = 0;
			int notcount = 0;
		std::cin.ignore();  // ������뻺�����еĻ��з�

		switch (choice) {
		case 1:
			std::cout << "���100�� <��ǩ,���> ʾ�����£�" << std::endl;//���100�� <��ǩ,���> ʾ�����£�
			PrintStringSkipList(*dataset);
			break;
		
		case 2: {
			std::cout << "����������Ҫ��ѯ�ı�ǩ��" << std::endl;
			std::cin >> userLabel;
			PrintSkipListForLabel(*dataset, userLabel);
			
			break;
		}
		case 3: {

			std::cout << "�������ǩ1��" << std::endl;
			std::cin >> label;

			std::cout << "�������ǩ2��" << std::endl;
			std::cin >> labe2;
			// ��ѯ���������
			auto l=PrintCommonSkipListElements(*dataset, label, labe2);
			delete l;
			std::cout << "�����ϣ�" << std::endl;
			
			break;
		}
		case 4: {
			std::cout << "�������ǩ1��" << std::endl;
			std::cin >> label;

			std::cout << "�������ǩ2��" << std::endl;
			std::cin >> labe2;

			// ��ѯ���������
			auto l=PrintUnionSkipListElements(*dataset, label, labe2);
			std::cout << "�����ϣ�" << std::endl;
			delete l;
			break;
		}
		case 5: {
			// ��ȡAND��ǩ
			std::cout << "�õģ�����֧�ָ��Ӳ�ѯ�����ṩAND��OR�Լ�NOT����"<<endl<<"���磬�����ѯ�й������ഺ��У԰���鼮�������ܳ���Ѫ��Ԫ�أ��������AND������ �й� ��������OR������ У԰ �ഺ����NOT������ Ѫ��" << std::endl;
			//��Ϊ"(�й�&����)&(�ഺ or У԰) but NOTѪ��"
			getLabels("AND:", andLabels);
			/*for (size_t i = 0; i < andLabels.size(); ++i) {
				andnum[i] = GetNumOfValueForLabel(*dataset, andLabels[i]);
				std::cout << "andLabels[" << i << "]: " << andLabels[i] <<"��Ӧ���鼮(���Ӱ)��"<<andnum[i]<< std::endl;
				andcount += andnum[i];
			}*/
			// ��ȡOR��ǩ
			getLabels("OR:", orLabels);
			//for (size_t i = 0; i < orLabels.size(); ++i) {
				/*ornum[i] = GetNumOfValueForLabel(*dataset, orLabels[i]);
				std::cout << "orLabels[" << i << "]: " << orLabels[i] << "��Ӧ���鼮(���Ӱ)��" << ornum[i]<< std::endl;
				orcount += ornum[i];*/
			//}
			// ��ȡNOT��ǩ
			getLabels("BUT NOT:", notLabels);
			//for (size_t i = 0; i < notLabels.size(); ++i) {
				/*notnum[i] = GetNumOfValueForLabel(*dataset, notLabels[i]);
				std::cout << "notLabels[" << i << "]: " << notLabels[i] << "��Ӧ���鼮(���Ӱ)��" <<notnum[i] << std::endl;
				notcount += notnum[i];*/
			//}
			map<string, StringSkipList*>* dataset2 = new map<string, StringSkipList*>();
			map<string, StringSkipList*>* dataset3 = new map<string, StringSkipList*>();
			map<string, StringSkipList*>* dataset4 = new map<string, StringSkipList*>();
			map<string, StringSkipList*>* dataset5 = new map<string, StringSkipList*>();
			//dataset2 = OR_process(*dataset, orLabels, ornum);
			dataset2 = PrintniUnionSkipListElements(*dataset, orLabels);
			std::cout << "���1" << std::endl;
			dataset3 = PrintCommonSkipListElements(*dataset, andLabels);
			std::cout << "���2" << std::endl;
			dataset4 = MergeCommonElementsBetweenSkipLists(*dataset2, *dataset3);
			std::cout << "���3" << std::endl;
			for (size_t i = 0; i < notLabels.size(); ++i) {
				dataset5 = SignleLabelSkipList(*dataset, notLabels[i]);
				std::cout << "���4" << std::endl;
				dataset4 = RemoveCommonElementsBetweenSkipLists(*dataset4, *dataset5);
				std::cout << "���5" << std::endl;
			}
			PrintStringSkipList2(*dataset4);
			delete dataset2;
			delete dataset3;
			delete dataset4;
			delete dataset5;
			break;
		}
		case 0:
			exit(0);  // �˳���������
		default:
			cout << "��������ǣ�" << choice << endl;
			break;
		}
	}
	

	// �����ڴ�
	for (auto& pair : *dataset) {
		delete pair.second;
	}
	system("pause");
	return 0;
}