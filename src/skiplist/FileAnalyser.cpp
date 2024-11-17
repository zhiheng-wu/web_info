#include "FileAnalyser.h"
#include <string>
#include <fstream>
using namespace std;

FileAnalyser::FileAnalyser(const char* path)
{
	_state = 0;
	_isEnd = false;
	_stream.open(path, ios::in);
	if (_stream.get() == L'B')
		_isBook = true;
	else
		_isBook = false;
	string line;
	getline(_stream, line);
}
FileAnalyser::Node* FileAnalyser::get()
{
	if (_isEnd)return nullptr;
	int count = 0;
	string buf;
	char c;
	while (_stream.eof() == false)
	{
		c = _stream.get();
		switch (_state)
		{
		case 0:
			if (c >= '0' && c <= '9')
			{
				count += c - '0';
				_state = 1;
			}
			else if (c == '\'')
				_state = 2;
			break;
		case 1:
			if (c >= '0' && c <= '9')
			{
				count *= 10;
				count += c - '0';
			}
			else if (c == '\'')
			{
				_state = 2;
				return new Node{ true,count,string() };
			}
			else
			{
				_state = 0;
				return new Node{ true,count,string() };
			}
			break;
		case 2:
			if (c == '\'')
			{
				_state = 0;
				if (buf.size())
					return new Node({ false,0,buf });
				else
					_state = 3;
			}
			else if (c == ' ')
			{
				_state = 3;
			}
			else
			{
				buf += c;
			}
			break;
		case 3:
			_isEnd = true;
			return nullptr;
		default:
			exit(-1);
		}
	}
	_isEnd = true;
	_stream.close();
	return nullptr;
}

FileAnalyser::~FileAnalyser()
{
	if (_isEnd == false)
		_stream.close();
}
