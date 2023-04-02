/******************************************************************************
 *  Compilation: clang++ -g -DDebug index_min_priority_queue.cc -W -Wall -Wfatal-errors -std=c++20 -o index_min_priority_queue
 *  Execution:  index_min_priority_queue
 *  Dependencies: 
 *
 *  Index Min Priority Queue implementation.
 *
 ******************************************************************************/

#include "index_min_priority_queue.h"

#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <fstream>
#include <utility>

using std::fstream;
using std::string;
using std::vector;
using std::swap;
using std::to_string;

#ifdef Debug
int main(int args, char *argv[]) {
    vector<string> strings{ "it", "was", "the", "best", "of", "times", "it", "was", "the", "worst" };
    auto cmp = [](const string& lhs, const string& rhs) { return lhs > rhs; };
    IndexMinPriorityQueue<string, decltype(cmp)> pq(static_cast<int>(strings.size()), cmp);
    for (size_t i = 0; i < strings.size(); ++i)
        pq.Insert(static_cast<int>(i), strings[i]);

    while (!pq.isEmpty()) {
        auto item = pq.DelMin();
        std::cout << item << ' ' << strings[item] << ' ';
    }
    std::cout << std::endl;
}
#endif
