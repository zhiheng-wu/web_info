#include "CompressedInvertedIndex.h"
#include <cassert>

#define IndexCompress(a) (a)

#define IndexSpan(a,b) ((a) * 4 + (b))

using namespace std;

CompressedInvertedIndex::CompressedInvertedIndex() {
	stream = nullptr;
	indexcount = 0;
	strslen = 0;
	strcount = 0;
	strs = nullptr;
	indexcount = 0;
	indexes = nullptr;
	lists = nullptr;
	listPointerInFile = nullptr;
}

CompressedInvertedIndex::StrElement CompressedInvertedIndex::parseStr(int index)
{
	StrElement ret{ 0,0,0,0,0 };
	int ibase = indexes[index];
	int iend = indexes[index + 1];
	int bi = 0;
	for (int i = ibase; i < iend; i++)
	{
		char& c = strs[i];
		if (c == ',')  ret.strE[0] = i - ibase;
		else if (c == 0)  ret.strE[++bi] = i - ibase;
	}
	return ret;
}

void CompressedInvertedIndex::writeIndexToFile(const char* path) const
{
	ofstream ost;
	ost.open(path, ios::out | ios::binary);
	ost.write((const char*)&strslen, sizeof(int));
	ost.write((const char*)&strcount, sizeof(int));
	ost.write((const char*)strs, sizeof(char) * strslen);
	ost.write((const char*)indexes, sizeof(int) * indexcount);
	ost.write((const char*)listPointerInFile, sizeof(int) * strcount);
	ost.close();
}

void CompressedInvertedIndex::writeToFile(std::map<std::string, StringSkipList*>* mapData, const char* path)
{
	int strcount = mapData->size();
	int indexcount = (strcount + 3) / 4;
	int strmaxlen = 0;
	int lbegin = 0;
	int idx = 0;
	int* listPointerInFile = new int[strcount];
	StringSkipList** lists = new StringSkipList * [strcount];
	const string** rawstrs = new const string * [strcount];
	for (auto& i : *mapData)
	{
		rawstrs[idx] = &i.first;
		strmaxlen += i.first.length() + 1;
		lists[idx] = i.second;
		listPointerInFile[idx] = lbegin;
		lbegin += i.second->getSize() * sizeof(int);
		idx++;
	}
	int* indexes = new int[indexcount];
	char* strs = new char[strmaxlen];
	int baseIdxstrs = 0;
	int len = 0;
	for (int i = 0; i < indexcount; i++)
	{
		indexes[i] = baseIdxstrs;
		int stridx = IndexSpan(i, 0);
		int count = min(strcount - stridx, 4);
		if (count == 1)
		{
			int sl = rawstrs[stridx]->length();
			len += sl;
			memcpy(strs + baseIdxstrs, rawstrs[stridx]->c_str(), sizeof(char) * sl);
			baseIdxstrs += sl;
		}
		else
		{
			int commonL = 0;
			while (1)
			{
				bool issame = true;
				for (int i = 1; i < count; i++)
				{
					if (rawstrs[stridx]->c_str()[commonL] != rawstrs[stridx + i]->c_str()[commonL])
					{
						issame = false;
						break;
					}
				}
				if (issame)
				{
					commonL++;
				}
				else break;
			}
			if (commonL != 0)
			{
				memcpy(strs + baseIdxstrs, rawstrs[stridx]->c_str(), sizeof(char) * commonL);
				strs[baseIdxstrs + commonL] = ',';
				len += commonL + 1;
				baseIdxstrs += commonL + 1;
			}
			for (int i = 0; i < count; i++)
			{
				int l = rawstrs[stridx + i]->length() - commonL;
				memcpy(strs + baseIdxstrs,
					rawstrs[stridx + i]->c_str() + commonL,
					sizeof(char) * l);
				if (i != count - 1)
				{
					strs[baseIdxstrs + l] = 0;
					baseIdxstrs += 1;
					len += 1;
				}
				baseIdxstrs += l;
				len += l;
			}
		}
	}
	ofstream ost;
	ost.open(path, ios::out | ios::binary);
	ost.write((const char*)&len, sizeof(int));
	ost.write((const char*)&strcount, sizeof(int));
	ost.write((const char*)strs, sizeof(char) * len);
	ost.write((const char*)indexes, sizeof(int) * indexcount);
	ost.write((const char*)listPointerInFile, sizeof(int) * strcount);
	for (auto& i : *mapData)
	{
		i.second->writeToFile(ost);
	}
	ost.close();
	delete[] listPointerInFile;
	delete[] lists;
	delete[] rawstrs;
	delete[] indexes;
	delete[] strs;
}

CompressedInvertedIndex::~CompressedInvertedIndex()
{
	if (stream)
	{
		stream->close();
		delete stream;
	}
	delete[] listPointerInFile;
	if (lists)
	{
		for (int i = 0; i < strcount; i++)
		{
			delete lists[i];
		}
		delete[] lists;
	}
	delete[] indexes;
	delete[] strs;
}

CompressedInvertedIndex* CompressedInvertedIndex::readFromFile(const char* path)
{
	CompressedInvertedIndex* d = new CompressedInvertedIndex();
	CompressedInvertedIndex& dataset = *d;
	dataset.stream = new ifstream();
	ifstream& ist = *dataset.stream;
	ist.open(path, ios::in | ios::binary);
	ist.read((char*)&dataset.strslen, sizeof(int));
	ist.read((char*)&dataset.strcount, sizeof(int));
	dataset.indexcount = (dataset.strcount + 3) / 4;
	dataset.strs = new char[dataset.strslen];
	ist.read((char*)dataset.strs, sizeof(char) * dataset.strslen);
	dataset.indexes = new int[dataset.indexcount + 1];
	ist.read((char*)dataset.indexes, sizeof(int) * dataset.indexcount);
	dataset.indexes[dataset.indexcount] = dataset.strslen;
	dataset.listPointerInFile = new int[dataset.strcount];
	ist.read((char*)dataset.listPointerInFile, sizeof(int) * dataset.strcount);
	dataset.lists = new StringSkipList * [dataset.strcount];
	int e = dataset.strcount;
	for (int i = 0; i < e; i++)
	{
		dataset.lists[i] = nullptr;
	}
	dataset.streamBaseIdx = 2 * sizeof(int) + sizeof(char) * dataset.strslen + sizeof(int) * (dataset.strcount + dataset.indexcount);
	return d;
}

static inline bool greaterThan(const char& a, const char& b)
{
	return (unsigned char)a > (unsigned char)b;
}
static inline bool smallerThan(const char& a, const char& b)
{
	return (unsigned char)a < (unsigned char)b;
}
static inline bool equalTo(const char& a, const char& b)
{
	return (unsigned char)a == (unsigned char)b;
}


static short limitedstrcpy(const int& limit, const char* limitstr, const char* str)
{
	int i;
	for (i = 0; i < limit; i++)
	{
		unsigned char cl = limitstr[i];
		unsigned char cs = str[i];
		if (cl != cs)return (short)cl - cs;
		else if (cl == 0)return 0;
	}
	if (str[i] == 0)
		return 0;
	return -(short)(unsigned char)str[i];
}

static short strictlimitedstrcpy(const int& limit, const char* limitstr, const char* limitstr2)
{
	int i;
	for (i = 0; i < limit; i++)
	{
		unsigned char cl = limitstr[i];
		unsigned char cs = limitstr2[i];
		if (cl != cs)return (short)cl - cs;
		else if (cl == 0)return 0;
	}
	return 0;
}

int CompressedInvertedIndex::contains(const std::string& str)
{
	int left = 0;
	int right = indexcount - 2;
	int mid = right / 2;
	while (1)
	{
		int idxbase = indexes[mid];
		StrElement ele = parseStr(mid);
		if (ele.strE[0] != 0)
		{
			short o = strictlimitedstrcpy(ele.strE[0], strs + idxbase, str.c_str());
			if (o > 0) goto GP;
			else if (o < 0) goto SP;
			else
			{
				const char* strbeg = str.c_str() + ele.strE[0];
				auto beginIdx = strs + idxbase;
				for (int i = 1; i < 5; i++)
				{
					if (ele.strE[i] != 0)
					{
						beginIdx += ele.strE[i - 1] + 1;
						short o = limitedstrcpy(ele.strE[i] - ele.strE[i - 1] - 1, beginIdx, strbeg);
						if (o > 0)goto GP;
						else if (o == 0) return IndexSpan(mid, i - 1);
					}
					else goto SP;
				}
			}
		}
		else
		{
			const char* strbeg = str.c_str();
			auto beginIdx = strs + idxbase;
			for (int i = 1; i < 5; i++)
			{
				if (ele.strE[i] != 0)
				{
					short o = limitedstrcpy((i == 1) ? ele.strE[1] : (ele.strE[i] - ele.strE[i - 1] - 1), beginIdx, strbeg);
					if (o > 0)goto GP;
					else if (o == 0) return IndexSpan(mid, i - 1);
					beginIdx += ele.strE[i] + 1;
				}
				else goto SP;
			}
		}
		exit(-1);
	SP:
		if (left == right || left > mid - 1)return -1;
		left = mid + 1;
		mid = (left + right) / 2;
		continue;
	GP:
		if (left == right || left > mid - 1)return -1;
		right = mid - 1;
		mid = (left + right) / 2;
	}
}

int CompressedInvertedIndex::contains(const char* str)
{
	int left = 0;
	int right = indexcount - 2;
	int mid = right / 2;
	while (1)
	{
		int idxbase = indexes[mid];
		StrElement ele = parseStr(mid);
		if (ele.strE[0] != 0)
		{
			short o = strictlimitedstrcpy(ele.strE[0], strs + idxbase, str);
			if (o > 0) goto GP;
			else if (o < 0) goto SP;
			else
			{
				const char* strbeg = str + ele.strE[0];
				auto beginIdx = strs + idxbase;
				for (int i = 1; i < 5; i++)
				{
					if (ele.strE[i] != 0)
					{
						beginIdx += ele.strE[i - 1] + 1;
						short o = limitedstrcpy(ele.strE[i] - ele.strE[i - 1] - 1, beginIdx, strbeg);
						if (o > 0)goto GP;
						else if (o == 0) return IndexSpan(mid, i - 1);
					}
					else goto SP;
				}
			}
		}
		else
		{
			const char* strbeg = str;
			auto beginIdx = strs + idxbase;
			for (int i = 1; i < 5; i++)
			{
				if (ele.strE[i] != 0)
				{
					short o = limitedstrcpy((i == 1) ? ele.strE[1] : (ele.strE[i] - ele.strE[i - 1] - 1), beginIdx, strbeg);
					if (o > 0)goto GP;
					else if (o == 0) return IndexSpan(mid, i - 1);
					beginIdx += ele.strE[i] + 1;
				}
				else goto SP;
			}
		}
		exit(-1);
	SP:
		if (left == right || left > mid - 1)return -1;
		left = mid + 1;
		mid = (left + right) / 2;
		continue;
	GP:
		if (left == right || left > mid - 1)return -1;
		right = mid - 1;
		mid = (left + right) / 2;
	}
}

StringSkipList* CompressedInvertedIndex::operator[](const int& i)
{
	if (i == -1)return nullptr;
	if (lists[i] != nullptr)return lists[i];
	int off = streamBaseIdx + listPointerInFile[i];
	stream->seekg(off, ios::beg);
	StringSkipList* l = StringSkipList::readFromFile(*stream);
	lists[i] = l;
	return l;
}

StringSkipList* CompressedInvertedIndex::operator[](const std::string& str)
{
	return operator[](contains(str));
}

StringSkipList* CompressedInvertedIndex::operator[](const char* str)
{
	return operator[](contains(str));
}
