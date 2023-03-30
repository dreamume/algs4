#ifndef HEAP_PRIORITY_QUEUE_
#define HEAP_PRIORITY_QUEUE_

#include <vector>

template<class Key, class Cmp = std::greater<Key>>
class HeapPriorityQueue {
public:
  HeapPriorityQueue(int capacity, const Cmp& cmp) noexcept : pq_(capacity + 1), cmp_(cmp) {}
  HeapPriorityQueue(const std::vector<Key>& keys, const Cmp& cmp) noexcept;
  HeapPriorityQueue() noexcept : pq_(1) {}
  HeapPriorityQueue(const HeapPriorityQueue& other) = default;
  HeapPriorityQueue &operator=(const HeapPriorityQueue& other) = default;
  HeapPriorityQueue(HeapPriorityQueue&& other) = default;
  HeapPriorityQueue &operator=(HeapPriorityQueue&& other) = default;

  bool IsEmpty() const { return n_ == 0; }
  Key Min() const;
  void Insert(Key x);
  Key DelMin();
  int Size() const { return n_; }

private:
  void Resize(int capacity);
  void Swim(int k);
  void Sink(int k);
  bool IsMinHeap() const;
  bool IsMinHeapOrdered(int k) const;

private:
  std::vector<Key> pq_;
  size_t n_{0};
  Cmp cmp_{};
};

#endif  // HEAP_PRIORITY_QUEUE_
