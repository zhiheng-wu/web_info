#include "FileAnalyser.h"
#include <string>
using namespace std;

FileAnalyser::FileAnalyser(const char* path)
{
	_state = 0;
	_isEnd = false;
	_stream.open(path, ios::in);
	if (_stream.get() == 'B')
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
	char c = _stream.get();
	while (_stream.eof() == false)
	{
		switch (_state)
		{
		case 0:
			if (c >= '0' && c <= '9')
			{
				count *= 10;
				count += c - '0';
			}
			else if (c == '\'')
			{
				_state = 1;
				return new Node{ true,count,string() };
			}
			else
			{
				_state = 2;
				return new Node{ true,count,string() };
			}
			break;
		case 1:
			if (c == '\'')
			{
				_state = 2;
				return new Node{ false,0,buf };
			}
			else
			{
				buf += c;
			}
			break;
		case 2:
			if (c == '\'')
			{
				_state = 1;
			}
			else if (c == '\n')
			{
				_state = 0;
			}
			break;
		}

		c = _stream.get();
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
