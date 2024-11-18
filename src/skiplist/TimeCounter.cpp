#include <cassert>
#include <iostream>
#include <stack>
#include"TimeCounter.h"
namespace yang
{
	namespace time {
		TimeCounter::TimeCounter()
		{
			UseTime = std::chrono::nanoseconds::zero();
		}
		void TimeCounter::Start()
		{
			assert(!IsStart);
			IsStart = true;
			StartTime = std::chrono::steady_clock::now();
		}
		void TimeCounter::End()
		{
			assert(IsStart);
			EndTime = std::chrono::steady_clock::now();
			IsStart = false;
			UseTime += std::chrono::duration_cast<std::chrono::nanoseconds>(EndTime - StartTime);
		}
		void TimeCounter::Reset()
		{
			UseTime = static_cast<Time>(static_cast<std::chrono::nanoseconds>(std::chrono::nanoseconds::zero()));
		}
		Time TimeCounter::GetTime() {
			return static_cast<Time>(UseTime);
		}
	}
}

std::string yang::time::Time::toStdString(int precesion)
{
	assert(precesion >= 0);
	const char* tag[4] = {"ns","us","ms","s"};
	precesion += 1;
	std::stack<std::pair<long long,int>> stack;
	long long s = count();
	int ns = s % 1000;
	if (ns != 0)
		stack.push({ ns,0 });
	s /= 1000;
	int us = s % 1000;
	if (us != 0)
		stack.push({ us,1 });
	s /= 1000;
	int ms = s % 1000;
	if (ms != 0)
		stack.push({ ms,2 });
	s /= 1000;
	if (s != 0)
		stack.push({ s,3 });
	std::string ret;
	for (int i = 0; i < precesion && (stack.empty() == false); i++)
	{
		auto& [m, n] = stack.top();
		ret += std::to_string(m) + tag[n];
		stack.pop();
	}
	return ret;
}
