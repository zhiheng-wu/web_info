#pragma once
#include <iostream>
void testCreateStaticSkipList();
void testIterateStaticSkipList();
void testSearchStaticSkipList();
#define DEBUGTYPE 0
#if DEBUGTYPE==1
template<typename ...T>
void sllog(T... args)
{
	bool arr[] = { ([](auto arg)->bool {
		std::cout << arg << " ";
		return true; })(std::forward<T>(args)) ... };
	std::cout << "\n";
}
#endif
#if DEBUGTYPE==0
#define sllog(...) ;
#endif
