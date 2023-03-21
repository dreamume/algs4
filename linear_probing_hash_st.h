#ifndef LINEAR_PROBING_HASH_ST
#define LINEAR_PROBING_HASH_ST

#include <vector>
#include <optional>
#include <queue>

template<class Key, class Value>
class LinearProbingHashST {
public:
    friend class LinearProbingHashST;

    LinearProbingHashST(int capacity) noexcept : m_(capacity), keys_(capacity), vals_(capacity) {}
    LinearProbingHashST() noexcept: LinearProbingHashST(INIT_CAPACITY) {}
    LinearProbingHashST(const LinearProbingHashST& other) = default;
    LinearProbingHashST &operator=(const LinearProbingHashST& other) = default;
    LinearProbingHashST(LinearProbingHashST&& other) = default;
    LinearProbingHashST &operator=(LinearProbingHashST&& other) = default;

    int size() const { return n_; }
    bool isEmpty() const { return n_ == 0; }
    bool contains(Key key) const;
    std::optional<Value> get(Key key) const;
    void deleteKey(Key key);
    std::queue<Key> keys() const;
    void put(Key key, Value value);
private:
    bool check() const;
    void resize(int capacity);
    void swap(LinearProbingHashST& other) noexcept;
    int hash(Key key) const;
private:
    static constexpr int INIT_CAPACITY = 4;
    int n_{0};
    int m_;
    std::vector<std::optional<Key>> keys_;
    std::vector<Value> vals_;
};

#endif
