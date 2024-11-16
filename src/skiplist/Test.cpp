#include "Test.h"
#include "StringSkipList.h"
#include <set>
using namespace std;
void testCreateStringSkipList()
{
	set<int> s;
	for (int i = 0; i < 10; i++)
	{
		s.insert(i);
	}
	auto l = StringSkipList(s);
}
void testIterateStringSkipList()
{
	set<int> s;
	for (int i = 0; i < 100; i++)
	{
		s.insert(i);
	}
	auto l = StringSkipList(s);
	auto p = l.getFirst();
	while (p != -1)
	{
		cout << l.getValue(p) << endl;
		p = l.getNext(p);
	}
}
void testSearchStringSkipList()
{
	set<int> s;
	for (int i = 0; i < 100; i++)
	{
		s.insert(i);
	}
	auto l = StringSkipList(s);
	int idx = 0;
	auto p = l.FastSearchForMaxEntryNotBiggerThan(idx);
	while (idx <= 100)
	{
		cout << idx << " " << l.getValue(p) << endl;
		p = l.FastSearchForMaxEntryNotBiggerThan(++idx);
	}
	p = l.FastSearchForMaxEntryNotBiggerThan(7);
	cout << l.getValue(p) << endl;
	p = l.FastSearchForMaxEntryNotBiggerThan(45);
	cout << l.getValue(p) << endl;
	p = l.FastSearchForMaxEntryNotBiggerThan(21);
	cout << l.getValue(p) << endl;
	p = l.FastSearchForMaxEntryNotBiggerThan(64);
	cout << l.getValue(p) << endl;
	p = l.FastSearchForMaxEntryNotBiggerThan(71, p);
	cout << l.getValue(p) << endl;
	p = l.FastSearchForMaxEntryNotBiggerThan(60, p);
	cout << l.getValue(p) << endl;
	p = l.FastSearchForMaxEntryNotBiggerThan(0);
	cout << l.getValue(p) << endl;
}
