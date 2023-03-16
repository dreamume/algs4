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

template<typename Key>
Key HeapPriorityQueue<Key>::min() const {
    if (isEmpty()) throw std::out_of_range("heap priority queue is empty!");
    return pq_[1];
}

template<typename Key>
void HeapPriorityQueue<Key>::resize(int capacity) {
    assert(capacity > n_);
    pq_.resize(capacity + 1);
}

template<typename Key>
void HeapPriorityQueue<Key>::insert(Key x) {
    if (n_ == static_cast<int>(pq_.size()) - 1) resize(pq_.size() * 2);
    pq_[++n_] = x;
    swim(n_);
}

template<typename Key>
Key HeapPriorityQueue<Key>::delMin() {
    if (isEmpty()) throw std::out_of_range("heap priority queue is empty!");
    int res{pq_[1]};
    swap(pq_[1], pq_[n_--]);
    sink(1);
    if (n_ > 0 && n_ == (pq_.size() - 1) / 4) resize(pq_.size() / 2);

    return res;
}

template<typename Key>
void HeapPriorityQueue<Key>::swim(Key k) {
    while (k > 1 && pq_[k / 2] > pq_[k]) {
        swap(pq_[k/2], pq_[k]);
        k /= 2;
    }
}

template<typename Key>
void HeapPriorityQueue<Key>::sink(Key k) {
    while (2 * k <= n_) {
        int i{2 * k};
        if (i < n_ && pq_[i] > pq_[i + 1]) ++i;
        if (pq_[k] <= pq_[i]) break;
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

    std::cout << "(" << pq.size() << " left on pq" << std::endl;
}
#endif
