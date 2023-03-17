#ifndef HEAP_PRIORITY_QUEUE
#define HEAP_PRIORITY_QUEUE

#include <vector>

template<class Key, class Cmp = std::less<Key>>
class HeapPriorityQueue {
public:
    HeapPriorityQueue(int capacity, const Cmp& cmp) noexcept : pq_(capacity + 1), cmp_(cmp) {}
    HeapPriorityQueue(const std::vector<Key>& keys, const Cmp& cmp) noexcept;
    HeapPriorityQueue() noexcept : pq_(1) {}
    HeapPriorityQueue(const HeapPriorityQueue& other) = default;
    HeapPriorityQueue &operator=(const HeapPriorityQueue& other) = default;
    HeapPriorityQueue(HeapPriorityQueue&& other) = default;
    HeapPriorityQueue &operator=(HeapPriorityQueue&& other) = default;

    bool isEmpty() const { return n_ == 0; }
    Key min() const;
    void insert(Key x);
    Key delMin();
    int size() const { return n_; }

private:
    void resize(int capacity);
    void swim(int k);
    void sink(int k);

private:
    std::vector<Key> pq_;
    size_t n_{0};
    Cmp cmp_{};
};

#endif
