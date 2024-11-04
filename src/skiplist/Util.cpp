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


