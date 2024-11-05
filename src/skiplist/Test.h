#pragma once
#include <iostream>
void testCreateStaticSkipList();
void testIterateStaticSkipList();
void testSearchStaticSkipList();
void testCreateStringSkipList();
void testIterateStringSkipList();
void testSearchStringSkipList();
#define DEBUG 0
#if DEBUG==1
template<typename ...T>
void sllog(T... args)
{
	bool arr[] = { ([](auto arg)->bool {
		std::cout << arg << " ";
		return true; })(std::forward<T>(args)) ... };
	std::cout << "\n";
}
#endif
#if DEBUG==0
#define sllog(...) ;
#endif
