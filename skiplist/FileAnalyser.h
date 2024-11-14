#pragma once
#include <fstream>
class FileAnalyser {
	std::ifstream _stream;
	bool _isBook;
	bool _isEnd;
	short _state;
public:
	struct Node {
		bool isID;
		int id;
		std::string tag;
	};
	FileAnalyser(const char* path);
	// return new object, nullable
	Node* get();
	~FileAnalyser();
};


