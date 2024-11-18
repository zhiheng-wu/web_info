#include "BoolAnalyser.h"

BoolAnalyser::BoolAnalyser(std::string s)
{
	str = s;
	idx = 0;
	state = 0;
}

BoolAnalyser::Node BoolAnalyser::get()
{
	std::string buf;
	while (idx < str.size())
	{
		char c = str[idx];
		idx++;
		switch (state)
		{
		case 0:
			switch (c)
			{
			case 'A':
				state = 1;
				break;
			case 'N':
				state = 3;
				break;
			case 'O':
				state = 5;
				break;
			case '\'':
				state = 6;
				break;
			case '(':
				return Node(Node::LEFT);
			case ')':
				return Node(Node::RIGHT);
			case ' ':
				break;
			default:
				state = 7;
				break;
			}
			break;
		case 1:
			if (c == 'N')
				state = 2;
			else
				state = 7;
			break;
		case 2:
			if (c == 'D')
			{
				state = 0;
				return Node(Node::AND);
			}
			else
				state = 7;
			break;
		case 3:
			if (c == 'O')
				state = 4;
			else
				state = 7;
			break;
		case 4:
			if (c == 'T')
			{
				state = 0;
				return Node(Node::NOT);
			}
			else
				state = 7;
			break;
		case 5:
			if (c == 'R')
			{
				state = 0;
				return Node(Node::OR);
			}
			else
				state = 7;
			break;
		case 6:
			switch (c)
			{
			case '\'':
				if (buf.size())
				{
					state = 0;
					return Node(buf);
				}
				else
					state = 7;
				break;
			case ' ':
				state = 7;
				break;
			default:
				buf += c;
				break;
			}
			break;
		case 7:
			idx = str.size();
			return Node();
			break;
		default:
			exit(-1);
		}
	}
	return Node();
}
