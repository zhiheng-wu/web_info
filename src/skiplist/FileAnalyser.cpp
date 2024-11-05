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
			else if (c == ',')
			{
				_state = 1;
				return new Node{ true,count,string() };
			}
			else
			{
				_isEnd = true;
				_stream.close();
				return nullptr;
			}
			break;
		case 1:
			if (c == '"')
				_state = 2;
			else
				exit(-1);
			break;
		case 2:
			if (c == '{' || c == '[')
				_state = 3;
			else
				exit(-1);
			break;
		case 3:
			if (c == '"')
				_state = 4;
			else if (c == '\'')
				_state = 8;
			else
				exit(-1);
			break;
		case 4:
			if (c == '"')
				_state = 5;
			else
				exit(-1);
			break;
		case 5:
			if (c == '"')
				_state = 6;
			else
				buf += c;
			break;
		case 6:
			if (c == '"')
			{
				_state = 7;
				return new Node({ false,0,buf });
			}
			else
			{
				_state = 5;
				buf += '"';
				buf += c;
			}
			break;
		case 7:
			if (c == '}')
				_state = 10;
			else if (c == ',')
				_state = 12;
			else
				exit(-1);
			break;
		case 8:
			if (c == '\'')
				_state = 9;
			else
				buf += c;
			break;
		case 9:
			if (c == '}' || c == ']')
			{
				_state = 10;
				return new Node({ false,0,buf });
			}
			else if (c == ',')
			{
				_state = 12;
				return new Node({ false,0,buf });
			}
			{
				_state = 8;
				buf += '\'';
				buf += c;
			}
			break;
		case 10:
			if (c == '"')
				_state = 11;
			else
				exit(-1);
			break;
		case 11:
			if (c == '\n' || c == '\r')
				_state = 0;
			else
				exit(-1);
			break;
		case 12:
			if (c == ' ')
				_state = 3;
			else
				exit(-1);
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
