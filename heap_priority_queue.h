#ifndef HEAP_PRIORITY_QUEUE
#define HEAP_PRIORITY_QUEUE

#include <vector>

template<typename Key, class Cmp = std::less<Key>>
class HeapPriorityQueue {
public:
    HeapPriorityQueue(int capacity) noexcept : pq_(capacity + 1) {}
    HeapPriorityQueue(const std::vector<Key>& keys) noexcept;
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
    void swim(Key k);
    void sink(Key k);

private:
    std::vector<Key> pq_;
    int n_{0};
    Cmp cmp_{};
};

#endif
