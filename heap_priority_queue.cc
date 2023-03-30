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

#include <exception>
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

#endif

using std::vector;
using std::swap;

template<class Key, class Cmp>
HeapPriorityQueue<Key, Cmp>::HeapPriorityQueue(const vector<Key>& keys, const Cmp& cmp) noexcept : 
    pq_(keys.size() + 1), n_(keys.size()), cmp_(cmp) {
  for (size_t i = 0; i < n_; ++i) pq_[i + 1] = keys[i];
  for (size_t i = n_ / 2; i >= 1; --i) Sink(i);
  assert(IsMinHeap());
}

template<class Key, class Cmp>
Key HeapPriorityQueue<Key, Cmp>::Min() const {
  if (IsEmpty()) throw std::out_of_range("heap priority queue is empty!");
  return pq_[1];
}

template<class Key, class Cmp>
void HeapPriorityQueue<Key, Cmp>::Resize(int capacity) {
  assert(capacity > static_cast<int>(n_));
  pq_.resize(capacity + 1);
}

template<class Key, class Cmp>
void HeapPriorityQueue<Key, Cmp>::Insert(Key x) {
  if (n_ == pq_.size() - 1) Resize(pq_.size() * 2);
  pq_[++n_] = x;
  Swim(n_);
  assert(IsMinHeap());
}

template<class Key, class Cmp>
Key HeapPriorityQueue<Key, Cmp>::DelMin() {
  if (IsEmpty()) throw std::out_of_range("heap priority queue is empty!");
  Key res{pq_[1]};
  swap(pq_[1], pq_[n_--]);
  Sink(1);
  if (n_ > 0 && n_ == (pq_.size() - 1) / 4) Resize(pq_.size() / 2);
  assert(IsMinHeap());

  return res;
}

template<class Key, class Cmp>
void HeapPriorityQueue<Key, Cmp>::Swim(int k) {
  while (k > 1 && cmp_(pq_[k/2], pq_[k])) {
    swap(pq_[k/2], pq_[k]);
    k /= 2;
  }
}

template<class Key, class Cmp>
bool HeapPriorityQueue<Key, Cmp>::IsMinHeap() const {
  return IsMinHeapOrdered(1);
}

template<class Key, class Cmp>
bool HeapPriorityQueue<Key, Cmp>::IsMinHeapOrdered(int k) const {
  if (k > static_cast<int>(n_)) return true;
  int left = 2 * k;
  int right = left + 1;
  if (left <= static_cast<int>(n_) && cmp_(pq_[k], pq_[left])) return false;
  if (right <= static_cast<int>(n_) && cmp_(pq_[k], pq_[right])) return false;
  return IsMinHeapOrdered(left) && IsMinHeapOrdered(right);
}

template<class Key, class Cmp>
void HeapPriorityQueue<Key, Cmp>::Sink(int k) {
  while (2 * k <= static_cast<int>(n_)) {
    size_t i = 2 * k;
    if (i < n_ && cmp_(pq_[i], pq_[i+1])) ++i;
    if (!(cmp_(pq_[k], pq_[i]))) break;
    swap(pq_[k], pq_[i]);
    k = i;
  }
}

#ifdef Debug
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
