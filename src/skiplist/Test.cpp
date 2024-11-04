#include "Test.h"
#include "StaticSkipList.h"
#include <functional>
#include <random>
#include <cassert>
#include <set>
using namespace std;
std::random_device RandomSeed;
std::mt19937 RandomEngine(RandomSeed());
template<typename Iter>
void RandomIntList(Iter begin, std::function<int& (decltype(*begin)&)> insertplace, const int& min, const int& max, int count, const bool& isrepeat) {
	assert(max >= min);
	assert(count > 0 && (isrepeat || (max - min + 1 >= count)));
	if (isrepeat)
	{
		std::uniform_int_distribution<int> intrd(min, max);
		for (int i = 0; i < count; i++)
		{
			insertplace(*begin) = intrd(RandomEngine);
			begin++;
		}
	}
	else
	{
		std::list<int> result;
		for (int num = 0; num < count; num++, begin++)
		{
			int number = std::uniform_int_distribution<int>(min, max - num)(RandomEngine);
			auto i = result.begin();
			for (; i != result.end(); i++)
			{
				if (number >= *i)
					number++;
				else
					break;
			}
			result.insert(i, number);
			insertplace(*begin) = number;
		}
	}
}
void testStaticSkipList() {
	set<int> s;
	for (int i = 0; i < 5; i++)
	{
		s.insert(i);
	}
	auto l = StaticSkipList(1, s);
}