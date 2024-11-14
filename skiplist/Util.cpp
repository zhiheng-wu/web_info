#include "Util.h"
#include "FileAnalyser.h"
#include <iostream>
#include <set>
using namespace std;
void buildPreDataSet(const char* path, std::map<std::string, void*>& preDataset)
{
	FileAnalyser file(path);
	FileAnalyser::Node* node = nullptr;
	int focusID = 0;
	while ((node = file.get()) != nullptr)
	{
		if (node->isID)
			focusID = node->id;
		else if (preDataset.contains(node->tag))
		{
			((set<int>*)preDataset[node->tag])->insert(focusID);
		}
		else
		{
			auto s = new set<int>();
			s->insert(focusID);
			preDataset[node->tag] = s;
		}
		delete node;
	}
}
void writeStaticSkipListToFile(const char* path, std::map<std::string, StaticSkipList*>* s)
{
	int keySize = 0;
	int valueSize = 0;
	ofstream stream;
	stream.open(path, ios::out | ios::binary);
	for (auto& i : *s)
	{
		string str = i.first;
		int size = str.size();
		stream.write((const char*)&size, sizeof(size));
		stream.write(str.c_str(), sizeof(char) * size);
		keySize += sizeof(char) * size;
		i.second->writeToFile(stream);
		valueSize += i.second->getSize() * sizeof(int) * 2;
	}
	stream.close();
	cout << "Write " << keySize << " bytes of key " << valueSize << " bytes of value to file " << path << endl;
}
std::map<std::string, StaticSkipList*>* ReadStaticSkipListFromFile(const char* path)
{
	ifstream stream;
	stream.open(path, ios::in | ios::binary);
	int bufsize = 0;
	stream.read((char*)&bufsize, sizeof(bufsize));
	char* buffer = nullptr;
	int maxsize = 0;
	std::map<std::string, StaticSkipList*>* ret = new std::map<std::string, StaticSkipList*>();
	auto& r1 = *ret;
	while (!stream.eof())
	{
		if (maxsize < bufsize)
		{
			delete[] buffer;
			buffer = new char[bufsize + 1];
			maxsize = bufsize;
		}
		stream.read(buffer, bufsize);
		buffer[bufsize] = 0;
		string str = buffer;
		auto sl = StaticSkipList::readFromFile(stream);
		r1[str] = sl;
		stream.read((char*)&bufsize, sizeof(bufsize));
	}
	stream.close();
	return ret;
}

void writeStringSkipListToFile(const char* path, std::map<std::string, StringSkipList*>* s)
{
	int keySize = 0;
	int valueSize = 0;
	ofstream stream;
	stream.open(path, ios::out | ios::binary);
	for (auto& i : *s)
	{
		string str = i.first;
		int size = str.size();
		stream.write((const char*)&size, sizeof(size));
		stream.write(str.c_str(), sizeof(char) * size);
		keySize += sizeof(char) * size;
		i.second->writeToFile(stream);
		valueSize += i.second->getSize() * sizeof(int);
	}
	stream.close();
	cout << "Write " << keySize << " bytes of key " << valueSize << " bytes of value to file " << path << endl;
}
std::map<std::string, StringSkipList*>* ReadStringSkipListFromFile(const char* path)
{
	ifstream stream;
	stream.open(path, ios::in | ios::binary);
	int bufsize = 0;
	stream.read((char*)&bufsize, sizeof(bufsize));
	char* buffer = nullptr;
	int maxsize = 0;
	std::map<std::string, StringSkipList*>* ret = new std::map<std::string, StringSkipList*>();
	auto& r1 = *ret;
	while (!stream.eof())
	{
		if (maxsize < bufsize)
		{
			delete[] buffer;
			buffer = new char[bufsize + 1];
			maxsize = bufsize;
		}
		stream.read(buffer, bufsize);
		buffer[bufsize] = 0;
		string str = buffer;
		auto sl = StringSkipList::readFromFile(stream);
		r1[str] = sl;
		stream.read((char*)&bufsize, sizeof(bufsize));
	}
	stream.close();
	return ret;
}
