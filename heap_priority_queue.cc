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

using std::fstream;

#endif

using std::vector;
using std::swap;

template<typename Key, class Cmp>
Key HeapPriorityQueue<Key, Cmp>::min() const {
    if (isEmpty()) throw std::out_of_range("heap priority queue is empty!");
    return pq_[1];
}

template<typename Key, class Cmp>
void HeapPriorityQueue<Key, Cmp>::resize(int capacity) {
    assert(capacity > n_);
    pq_.resize(capacity + 1);
}

template<typename Key, class Cmp>
void HeapPriorityQueue<Key, Cmp>::insert(Key x) {
    if (n_ == static_cast<int>(pq_.size()) - 1) resize(pq_.size() * 2);
    pq_[++n_] = x;
    swim(n_);
}

template<typename Key, class Cmp>
Key HeapPriorityQueue<Key, Cmp>::delMin() {
    if (isEmpty()) throw std::out_of_range("heap priority queue is empty!");
    int res{pq_[1]};
    swap(pq_[1], pq_[n_--]);
    sink(1);
    if (n_ > 0 && n_ == (pq_.size() - 1) / 4) resize(pq_.size() / 2);

    return res;
}

template<typename Key, class Cmp>
void HeapPriorityQueue<Key, Cmp>::swim(Key k) {
    while (k > 1 && cmp_(pq_[k], pq_[k / 2])) {
        swap(pq_[k/2], pq_[k]);
        k /= 2;
    }
}

template<typename Key, class Cmp>
void HeapPriorityQueue<Key, Cmp>::sink(Key k) {
    while (2 * k <= n_) {
        int i{2 * k};
        if (i < n_ && cmp_(pq_[i + 1], pq_[i])) ++i;
        if (!(cmp_(pq_[i], pq_[k]))) break;
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
        if (c == '-') std::cout << pq.delMin() << " ";
        else if (c != ' ') pq.insert(c);
    });

    std::cout << "(" << pq.size() << " left on pq)" << std::endl;
}
#endif
