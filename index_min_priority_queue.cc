/******************************************************************************
 *  Compilation: clang++ -g -DDebug index_min_priority_queue.cc -W -Wall -Wfatal-errors -std=c++20 -o index_min_priority_queue
 *  Execution:  index_min_priority_queue
 *  Dependencies: 
 *
 *  Index Min Priority Queue implementation.
 *
 ******************************************************************************/

#include "index_min_priority_queue.h"

#include <exception>
#include <cassert>
#include <string>

#ifdef Debug
#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <fstream>
#include <utility>

using std::fstream;
using std::string;

#endif

using std::vector;
using std::swap;
using std::to_string;

template<class Key, class Cmp>
void IndexMinPriorityQueue<Key, Cmp>::validateIndex(int i) const {
    if (i < 0) throw std::out_of_range("index is negative: " + to_string(i));
    if (i >= max_n_) throw std::out_of_range("index >= capacity: " + to_string(i));
}

template<class Key, class Cmp>
bool IndexMinPriorityQueue<Key, Cmp>::contains(int i) const {
    validateIndex(i);
    return qp_[i] != -1;
}

template<class Key, class Cmp>
void IndexMinPriorityQueue<Key, Cmp>::insert(int i, Key key) {
    validateIndex(i);
    if (contains(i)) 
        throw std::invalid_argument("index is already in the priority queue");
    ++n_;
    qp_[i] = n_;
    pq_[n_] = i;
    keys_[i] = key;
    swim(n_);
}

template<class Key, class Cmp>
int IndexMinPriorityQueue<Key, Cmp>::minIndex() const {
    if (n_ == 0) throw std::out_of_range("priority queue underflow");
    return pq_[1];
}

template<class Key, class Cmp>
Key IndexMinPriorityQueue<Key, Cmp>::minKey() const {
    if (n_ == 0) throw std::out_of_range("priority queue underflow");
    return keys_[pq_[1]];
}

template<class Key, class Cmp>
Key IndexMinPriorityQueue<Key, Cmp>::keyOf(int i) const {
    validateIndex(i);
    if (!contains(i)) throw std::invalid_argument("index is not in the priority queue");
    return keys_[i];
}

template<class Key, class Cmp>
void IndexMinPriorityQueue<Key, Cmp>::changeKey(int i, Key key) {
    validateIndex(i);
    if (!contains(i)) throw std::invalid_argument("index is not in the priority queue");
    keys_[i] = key;
    swim(qp_[i]);
    sink(qp_[i]);
}

template<class Key, class Cmp>
void IndexMinPriorityQueue<Key, Cmp>::change(int i, Key key) {
    changeKey(i, key);
}

template<class Key, class Cmp>
void IndexMinPriorityQueue<Key, Cmp>::decreaseKey(int i, Key key) {
    validateIndex(i);
    if (!contains(i)) throw std::invalid_argument("index is not in the priority queue");
    if (!cmp_(keys_[i], key))
        throw std::invalid_argument("calling decreaseKey() with a key equal or greater than the key in the priority queue");
    keys_[i] = key;
    swim(qp_[i]);
}

template<class Key, class Cmp>
void IndexMinPriorityQueue<Key, Cmp>::increaseKey(int i, Key key) {
    validateIndex(i);
    if (!contains(i)) throw std::invalid_argument("index is not in the priority queue");
    if (!cmp_(key, keys_[i]))
        throw std::invalid_argument("calling decreaseKey() with a key equal or less than the key in the priority queue");
    keys_[i] = key;
    sink(qp_[i]);
}

template<class Key, class Cmp>
void IndexMinPriorityQueue<Key, Cmp>::deleteIndex(int i) {
    validateIndex(i);
    if (!contains(i)) throw std::invalid_argument("index is not in the priority queue");
    int index = qp_[i];
    exchange(index, n_--);
    swim(index);
    sink(index);
    keys_[i] = -1;
    qp_[i] = -1;
}

template<class Key, class Cmp>
int IndexMinPriorityQueue<Key, Cmp>::delMin() {
    if (n_ == 0) throw std::out_of_range("priority queue underflow");
    int minIndex = pq_[1];
    exchange(1, n_--);
    sink(1);
    assert(minIndex == pq_[n_+1]);
    qp_[minIndex] = -1;
    keys_[minIndex] = -1;
    pq_[n_ + 1] = -1;

    return minIndex;
}

template<class Key, class Cmp>
void IndexMinPriorityQueue<Key, Cmp>::exchange(int i, int j) {
    swap(pq_[i], pq_[j]);
    qp_[pq_[i]] = i;
    qp_[pq_[j]] = j;
}

template<class Key, class Cmp>
void IndexMinPriorityQueue<Key, Cmp>::swim(int k) {
    while (k > 1 && cmp_(keys_[pq_[k / 2]], keys_[pq_[k]])) {
        exchange(k, k / 2);
        k = k / 2;
    }
}

template<class Key, class Cmp>
void IndexMinPriorityQueue<Key, Cmp>::sink(int k) {
    while (2 * k <= static_cast<int>(n_)) {
        int i = 2 * k;
        if (i < n_ && cmp_(keys_[pq_[i]], keys_[pq_[i + 1]])) ++i;
        if (!cmp_(keys_[pq_[k]], keys_[pq_[i]])) break;
        exchange(k, i);
        k = i;
    }
}

#ifdef Debug
int main(int args, char *argv[]) {
    vector<string> strings{ "it", "was", "the", "best", "of", "times", "it", "was", "the", "worst" };
    auto cmp = [](const string& lhs, const string& rhs) { return lhs > rhs; };
    IndexMinPriorityQueue<string, decltype(cmp)> pq(static_cast<int>(strings.size()), cmp);
    for (size_t i = 0; i < strings.size(); ++i)
        pq.insert(static_cast<int>(i), strings[i]);

    while (!pq.isEmpty()) {
        auto item = pq.delMin();
        std::cout << item << ' ' << strings[item] << ' ';
    }
    std::cout << std::endl;
}
#endif
