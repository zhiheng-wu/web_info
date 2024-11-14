#include <iostream>
#include <set>
#include <algorithm>
#include <iterator> // for std::inserter

int main3() {
    std::set<int> set1 = { 1, 2, 3, 4, 5 };
    std::set<int> set2 = { 3, 4, 5, 6, 7 };
    std::set<int> commonElements;
    std::set<int> unionElements;
    std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::inserter(commonElements, commonElements.begin()));
    std::set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), std::inserter(unionElements, unionElements.begin()));
    // Êä³ö½á¹û
    std::cout << "Common elements: ";
    for (const auto& elem : commonElements) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;


    std::cout << "Union elements: ";
    for (const auto& elem : unionElements) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
    return 0;
}