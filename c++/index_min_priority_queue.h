#ifndef INDEX_MIN_PRIORITY_QUEUE_
#define INDEX_MIN_PRIORITY_QUEUE_

#include <vector>
#include <exception>
#include <cassert>
#include <string>

namespace algs4 {
template<class Key, class Cmp = std::greater<Key>>
class IndexMinPriorityQueue {
public:
  IndexMinPriorityQueue(int maxn, const Cmp& cmp) noexcept 
    : max_n_(maxn), pq_(maxn + 1), 
      qp_(maxn + 1, -1), keys_(maxn + 1), cmp_(cmp) {}

  IndexMinPriorityQueue() = delete;
  IndexMinPriorityQueue(const IndexMinPriorityQueue& other) = default;
  IndexMinPriorityQueue &operator=(const IndexMinPriorityQueue& other) = default;
  IndexMinPriorityQueue(IndexMinPriorityQueue&& other) = default;
  IndexMinPriorityQueue &operator=(IndexMinPriorityQueue&& other) = default;

  bool IsEmpty() const { return n_ == 0; }
  bool Contains(int i) const {
    ValidateIndex(i);
    return qp_[i] != -1;
  }

  int Size() const { return n_; }
  void Insert(int i, Key key) {
    ValidateIndex(i);
    if (Contains(i)) 
      throw std::invalid_argument("index is already in the priority queue");
    ++n_;
    qp_[i] = n_;
    pq_[n_] = i;
    keys_[i] = key;
    Swim(n_);
  }

  int MinIndex() const {
    if (n_ == 0) throw std::out_of_range("priority queue underflow");
    return pq_[1];
  }

  Key MinKey() const {
    if (n_ == 0) throw std::out_of_range("priority queue underflow");
    return keys_[pq_[1]];
  }

  int DelMin() {
    if (n_ == 0) throw std::out_of_range("priority queue underflow");
    int minIndex = pq_[1];
    Exchange(1, n_--);
    Sink(1);
    assert(minIndex == pq_[n_+1]);
    qp_[minIndex] = -1;
    keys_[minIndex] = -1;
    pq_[n_ + 1] = -1;

    return minIndex;
  }

  Key KeyOf(int i) const {
    ValidateIndex(i);
    if (!Contains(i)) 
      throw std::invalid_argument("index is not in the priority queue");
    return keys_[i];
  }

  void ChangeKey(int i, Key key) {
    ValidateIndex(i);
    if (!Contains(i)) 
      throw std::invalid_argument("index is not in the priority queue");
    keys_[i] = key;
    Swim(qp_[i]);
    Sink(qp_[i]);
  }

  void Change(int i, Key key) { ChangeKey(i, key); }

  void DecreaseKey(int i, Key key) {
    ValidateIndex(i);
    if (!Contains(i)) 
      throw std::invalid_argument("index is not in the priority queue");
    if (!cmp_(keys_[i], key))
      throw std::invalid_argument("calling DecreaseKey() with a key equal or greater than the key in the priority queue");
    keys_[i] = key;
    Swim(qp_[i]);
  }

  void IncreaseKey(int i, Key key) {
    ValidateIndex(i);
    if (!Contains(i)) 
      throw std::invalid_argument("index is not in the priority queue");
    if (!cmp_(key, keys_[i]))
      throw std::invalid_argument("calling DecreaseKey() with a key equal or less than the key in the priority queue");
    keys_[i] = key;
    Sink(qp_[i]);
  }

  void DeleteIndex(int i) {
    ValidateIndex(i);
    if (!Contains(i)) throw std::invalid_argument("index is not in the priority queue");
    int index = qp_[i];
    Exchange(index, n_--);
    Swim(index);
    Sink(index);
    keys_[i] = -1;
    qp_[i] = -1;
  }

private:
  void ValidateIndex(int i) const {
    if (i < 0) throw std::out_of_range("index is negative: " + std::to_string(i));
    if (i >= max_n_) throw std::out_of_range("index >= capacity: " + std::to_string(i));
  }

  void Exchange(int i, int j) {
    std::swap(pq_[i], pq_[j]);
    qp_[pq_[i]] = i;
    qp_[pq_[j]] = j;
  }

  void Swim(int k) {
    while (k > 1 && cmp_(keys_[pq_[k / 2]], keys_[pq_[k]])) {
      Exchange(k, k / 2);
      k = k / 2;
    }
  }

  void Sink(int k) {
    while (2 * k <= static_cast<int>(n_)) {
      int i = 2 * k;
      if (i < n_ && cmp_(keys_[pq_[i]], keys_[pq_[i + 1]])) ++i;
      if (!cmp_(keys_[pq_[k]], keys_[pq_[i]])) break;
      Exchange(k, i);
      k = i;
    }
  }

private:
  int max_n_;
  int n_{0};
  std::vector<int> pq_;
  std::vector<int> qp_;
  std::vector<Key> keys_;
  Cmp cmp_{};
};
}

#endif  // INDEX_MIN_PRIORITY_QUEUE_
