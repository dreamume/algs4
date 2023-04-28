/******************************************************************************
 *  Compilation: clang++ -g -DDebug heap_priority_queue.cc -W -Wall -Wfatal-errors -std=c++20 -o heap_priority_queue
 *  Execution:  heap_priority_queue input.txt
 *  Dependencies: 
 *  Data files:   https://algs4.cs.princeton.edu/24pq/tinyPQ.txt
 *
 *  Heap Priority Queue implementation.
 *
 ******************************************************************************/

#include "heap_priority_queue.h"

#include <cassert>

#ifdef Debug
#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <fstream>
#include <utility>

using std::fstream;
using std::pair;
using std::vector;

int main(int args, char *argv[]) {
  fstream in(argv[1]);
  if (!in.is_open()) {
    std::cout << "failed to open " << argv[1] << '\n';
    return 1;
  }

  HeapPriorityQueue<char> pq;
  std::string line;
  std::getline(in, line);
  for_each(line.begin(), line.end(), [&pq](char c) {
    if (c == '-') std::cout << pq.DelMin() << " ";
    else if (c != ' ') pq.Insert(c);
  });

  std::cout << "(" << pq.Size() << " left on pq)" << std::endl;

  auto cmp = [](const pair<int, int>& lhs, const pair<int, int>& rhs) {
    return lhs.first > rhs.first || (lhs.first == rhs.first && lhs.second > rhs.second);
  };
  HeapPriorityQueue<pair<int, int>, decltype(cmp)> pq2(1000, cmp);

  vector<pair<int, int>> data{{1,2}, {2,3}, {3, 4}};
  HeapPriorityQueue<pair<int, int>, decltype(cmp)> pq3(data, cmp);
  HeapPriorityQueue<pair<int, int>, decltype(cmp)> pq4 = std::move(pq3);

  while (!pq4.IsEmpty()) {
    auto item = pq4.DelMin();
    std::cout << "(" << item.first << ',' << item.second << "), ";
  }
  std::cout << std::endl;
}
#endif
