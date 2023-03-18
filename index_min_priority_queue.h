#ifndef INDEX_MIN_PRIORITY_QUEUE
#define INDEX_MIN_PRIORITY_QUEUE

#include <vector>

template<class Key, class Cmp = std::greater<Key>>
class IndexMinPriorityQueue {
public:
  IndexMinPriorityQueue(int maxn, const Cmp& cmp) noexcept 
      : max_n_(maxn), pq_(maxn + 1), 
        qp_(maxn + 1, -1), keys_(maxn + 1), cmp_(cmp) {}
  //IndexMinPriorityQueue(int maxn) noexcept : max_n_(maxn), pq_(1), qp_(1), keys_(1) {}
    IndexMinPriorityQueue() = delete;
    IndexMinPriorityQueue(const IndexMinPriorityQueue& other) = default;
    IndexMinPriorityQueue &operator=(const IndexMinPriorityQueue& other) = default;
    IndexMinPriorityQueue(IndexMinPriorityQueue&& other) = default;
    IndexMinPriorityQueue &operator=(IndexMinPriorityQueue&& other) = default;

    bool isEmpty() const { return n_ == 0; }
    bool contains(int i) const;
    int size() const { return n_; }
    void insert(int i, Key key);
    int minIndex() const;
    Key minKey() const;
    int delMin();
    Key keyOf(int i) const;
    void changeKey(int i, Key key);
    void change(int i, Key key);
    void decreaseKey(int i, Key key);
    void increaseKey(int i, Key key);
    void deleteIndex(int i);
private:
    void validateIndex(int i) const;
    void exchange(int i, int j);
    void swim(int k);
    void sink(int k);
private:
    int max_n_;
    int n_{0};
    std::vector<int> pq_;
    std::vector<int> qp_;
    std::vector<Key> keys_;
    Cmp cmp_{};
};

#endif
