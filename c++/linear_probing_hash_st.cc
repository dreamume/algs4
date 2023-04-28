#include "linear_probing_hash_st.h"

#include <exception>
#include <cassert>
#include <string>
#include <functional>

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
using std::queue;
using std::nullopt;
using std::hash;

template<class Key, class Value>
bool LinearProbingHashST<Key, Value>::contains(Key key) const {
    return get(key) != nullopt;
}

template<class Key, class Value>
std::optional<Value> LinearProbingHashST<Key, Value>::get(Key key) const {
    for (int i = hash(key); keys_[i] != nullopt; i = (i + 1) % m_) {
        if (keys_[i].has_value() && keys_[i].value() == key) 
            return vals_[i];
    }

    return nullopt;
}

template<class Key, class Value>
void LinearProbingHashST<Key, Value>::deleteKey(Key key) {
    if (!contains(key)) return;

    int i = hash(key);
    while (key != keys_[i]) i = (i + 1) % m_;

    keys_[i] = nullopt;
    
    i = (i + 1) % m_;
    while (keys_[i] != nullopt) {
        Key rehashKey = keys_[i].value();
        Value value = vals_[i];
        keys_[i] = nullopt;
        --n_;
        put(rehashKey, value);
        i = (i + 1) % m_;
    }

    --n_;
    if (n_ > 0 && n_ <= m_ / 8) resize(m_ / 2);

    assert(check());
}

template<class Key, class Value>
void LinearProbingHashST<Key, Value>::swap(LinearProbingHashST<Key, Value>& other) noexcept {
    std::swap(n_, other.n_);
    std::swap(m_, other.m_);
    std::swap(keys_, other.keys_);
    std::swap(vals_, other.vals_);
}

template<class Key, class Value>
void LinearProbingHashST<Key, Value>::resize(int capacity) {
    LinearProbingHashST temp(capacity);
    for (int i = 0; i < m_; ++i) {
        if (keys_[i].has_value()) temp.put(keys_[i].value(), vals_[i]);
    }
    swap(temp);
}

template<class Key, class Value>
int LinearProbingHashST<Key, Value>::hash(Key key) const {
    int h = std::hash<Key>{}(key);
    return h & (m_ - 1);
}

template<class Key, class Value>
void LinearProbingHashST<Key, Value>::put(Key key, Value value) {
    if (n_ >= m_ / 2) resize(2 * m_);

    int i;
    for (i = hash(key); keys_[i].has_value(); i = (i + 1) % m_) {
        if (keys_[i].value() == key) {
            vals_[i] = value;
            return;
        }
    }
    keys_[i] = key;
    vals_[i] = value;
    ++n_;
}

template<class Key, class Value>
queue<Key> LinearProbingHashST<Key, Value>::keys() const {
    queue<Key> res;
    for (int i = 0; i < m_; ++i) {
        if (keys_[i].has_value()) res.push(keys_[i].value());
    }

    return res;
}

template<class Key, class Value>
bool LinearProbingHashST<Key, Value>::check() const {
    if (m_ < 2 * n_) return false;

    for (int i = 0; i < m_; ++i) {
        if (!keys_[i].has_value()) continue;
        if (get(keys_[i].value()) != vals_[i]) return false;
    }

    return true;
}

#ifdef Debug
int main(int args, char *argv[]) {
    LinearProbingHashST<string, int> st;
    fstream in(argv[1]);
    if (!in.is_open()) {
        std::cout << "failed to open " << argv[1] << '\n';
        return 1;
    }

    string line;
    getline(in, line);
    for (int i = 0; i < line.size(); ++i) {
        if (line[i] != ' ') st.put(string(1, line[i]), i);
    }

    queue<string> keys = st.keys();
    while (!keys.empty()) {
        std::cout << keys.front() << " " << st.get(keys.front()).value_or(-1) << std::endl;
        keys.pop();
    }

    return 0;
}
#endif

