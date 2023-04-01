#ifndef HEAP_PRIORITY_QUEUE_
#define HEAP_PRIORITY_QUEUE_

#include <vector>
#include <cassert>
#include <exception>

template<class Key, class Cmp = std::greater<Key>>
class HeapPriorityQueue {
public:
  HeapPriorityQueue(int capacity, const Cmp& cmp) noexcept : pq_(capacity + 1), cmp_(cmp) {}
  HeapPriorityQueue(const std::vector<Key>& keys, const Cmp& cmp) noexcept :
    pq_(keys.size() + 1), n_(keys.size()), cmp_(cmp) {
    for (size_t i = 0; i < n_; ++i) pq_[i + 1] = keys[i];
    for (size_t i = n_ / 2; i >= 1; --i) Sink(i);
    assert(IsMinHeap());
  }
  HeapPriorityQueue() noexcept : pq_(1) {}
  HeapPriorityQueue(const HeapPriorityQueue& other) = default;
  HeapPriorityQueue &operator=(const HeapPriorityQueue& other) = default;
  HeapPriorityQueue(HeapPriorityQueue&& other) = default;
  HeapPriorityQueue &operator=(HeapPriorityQueue&& other) = default;

  bool IsEmpty() const { return n_ == 0; }
  Key Min() const {
    if (IsEmpty()) throw std::out_of_range("heap priority queue is empty!");
    return pq_[1];
  }
  void Insert(Key x) {
    if (n_ == pq_.size() - 1) Resize(pq_.size() * 2);
    pq_[++n_] = x;
    Swim(n_);
    assert(IsMinHeap());
  }
  Key DelMin() {
    if (IsEmpty()) throw std::out_of_range("heap priority queue is empty!");
    Key res{pq_[1]};
    std::swap(pq_[1], pq_[n_--]);
    Sink(1);
    if (n_ > 0 && n_ == (pq_.size() - 1) / 4) Resize(pq_.size() / 2);
    assert(IsMinHeap());

    return res;
  }

  int Size() const { return n_; }

private:
  void Resize(int capacity) {
    assert(capacity > static_cast<int>(n_));
    pq_.resize(capacity + 1);
  }

  void Swim(int k) {
    while (k > 1 && cmp_(pq_[k/2], pq_[k])) {
      std::swap(pq_[k/2], pq_[k]);
      k /= 2;
    }
  }

  void Sink(int k) {
    while (2 * k <= static_cast<int>(n_)) {
      size_t i = 2 * k;
      if (i < n_ && cmp_(pq_[i], pq_[i+1])) ++i;
      if (!(cmp_(pq_[k], pq_[i]))) break;
      std::swap(pq_[k], pq_[i]);
      k = i;
    }
  }

  bool IsMinHeap() const { return IsMinHeapOrdered(1); }

  bool IsMinHeapOrdered(int k) const {
    if (k > static_cast<int>(n_)) return true;
    int left = 2 * k;
    int right = left + 1;
    if (left <= static_cast<int>(n_) && cmp_(pq_[k], pq_[left])) return false;
    if (right <= static_cast<int>(n_) && cmp_(pq_[k], pq_[right])) return false;
    return IsMinHeapOrdered(left) && IsMinHeapOrdered(right);
  }

private:
  std::vector<Key> pq_;
  size_t n_{0};
  Cmp cmp_{};
};

#endif  // HEAP_PRIORITY_QUEUE_
