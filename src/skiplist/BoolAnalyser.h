#pragma once
#include <string>
class BoolAnalyser
{
	std::string str;
	short state;
	int idx;
public:
	BoolAnalyser(std::string s);
	struct Node {
		enum TERMINATOR {
			OR, AND, NOT, LEFT, RIGHT, VALUE, END
		}terminator = OR;
		std::string value;
		Node(TERMINATOR t)
		{
			terminator = t;
		}
		Node()
		{
			terminator = END;
		}
		Node(std::string v)
		{
			terminator = VALUE;
			value = v;
		}
	};
	Node get();
};

