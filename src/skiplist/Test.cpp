#include "Test.h"
#include "StaticSkipList.h"
#include <functional>
#include <cassert>
#include <set>
using namespace std;
void testCreateStaticSkipList() {
	set<int> s;
	for (int i = 0; i < 100; i++)
	{
		s.insert(i);
	}
	auto l = StaticSkipList(1, s);
}
void testIterateStaticSkipList()
{
	set<int> s;
	for (int i = 0; i < 100; i++)
	{
		s.insert(i);
	}
	auto l = StaticSkipList(1, s);
	auto p = l.getNext();
	while (p != nullptr)
	{
		cout << l.getValue(p) << endl;
		p = l.getNext(p);
	}
}
void testSearchStaticSkipList()
{
	set<int> s;
	for (int i = 0; i < 100; i++)
	{
		s.insert(i);
	}
	auto l = StaticSkipList(1, s);
	int idx = 0;
	auto p = l.searchForMaxTopNodeNotBiggerThan(idx);
	while (idx <= 100)
	{
		cout << idx << " " << l.getValue(p) << endl;
		p = l.searchForMaxTopNodeNotBiggerThan(++idx);
	}
	p = l.searchForMaxTopNodeNotBiggerThan(7);
	cout << l.getValue(p) << endl;
	p = l.searchForMaxTopNodeNotBiggerThan(45);
	cout << l.getValue(p) << endl;
	p = l.searchForMaxTopNodeNotBiggerThan(21);
	cout << l.getValue(p) << endl;
	p = l.searchForMaxTopNodeNotBiggerThan(64);
	cout << l.getValue(p) << endl;
	p = l.searchForMaxTopNodeNotBiggerThan(71, p);
	cout << l.getValue(p) << endl;
	p = l.searchForMaxTopNodeNotBiggerThan(60, p);
	cout << l.getValue(p) << endl;
	p = l.searchForMaxTopNodeNotBiggerThan(0);
	cout << l.getValue(p) << endl;
}