#pragma once
#include "BoolAnalyser.h"
#include "ResultLinkedList.h"
#include "PriorityQueue.h"
#include "StringSkipList.h"
#include <list>
#include <map>
#include <stack>

// 1 EL2 -> EL2 OR E2
// 2 EL2 -> E2
// 3 E2 -> EL1
// 4 EL1 -> EL1 AND E1
// 5 EL1 -> E1
// 6 E1 -> E0
// 7 E0 -> NOT E0
// 8 E0 -> ( EL2 )
// 9 E0 -> VALUE

std::string search(std::string s, std::map<std::string, StringSkipList*>* dataset);