#include "Util.h"
#include "FileAnalyser.h"
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
			((set<int>*)preDataset[node->tag])->insert(node->id);
		}
		else
		{
			auto s = new set<int>();
			s->insert(node->id);
			preDataset[node->tag] = s;
		}
		delete node;
	}
}
void writeStaticSkipListToFile(const char* path, std::map<std::string, StaticSkipList*>* s)
{
	ofstream stream;
	stream.open(path, ios::out | ios::binary);
	for (auto& i : *s)
	{
		string str = i.first;
		int size = str.size();
		stream.write((const char*)&size, sizeof(size));
		stream.write(str.c_str(), sizeof(char) * size);
		i.second->writeToFile(stream);
	}
	stream.close();
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

