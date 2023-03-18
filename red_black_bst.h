#ifndef RED_BLACK_BST_H
#define RED_BLACK_BST_H

#include <algorithm>
#include <queue>

template<typename T>
bool isLess(T x, T y) {
    return x < y;
}

template<class T>
T defaultValue() {
    return T();
}

template <>
int defaultValue<int>() {
    return -1;
}


template <typename Key, typename Value>
class RedBlackBST {
private:
    struct Node {
        Node(Key key, Value value, bool color, int size): key_(key), value_(value),
                                                          color_(color), size_(size) {}
        Key key_;
        Value value_;
        Node *left_{nullptr};
        Node *right_{nullptr};
        bool color_;
        int size_;
    };
public:
    /**
     * Initializes an empty symbol table.
     */
    RedBlackBST() noexcept {}
    RedBlackBST(const RedBlackBST& other) = delete;
    RedBlackBST &operator=(const RedBlackBST& other) = delete;
    RedBlackBST(RedBlackBST&& other) = default;
    RedBlackBST &operator=(RedBlackBST&& other) = default;
    /**
     * Returns the number of key-value pairs in this symbol table.
     * @return the number of key-value pairs in this symbol table
     */
    int size() const { return size(root_); }

    /**
     * Is this symbol table empty?
     * @return {@code true} if this symbol table is empty and {@code false} otherwise
     */
    bool isEmpty() const { return root_ == nullptr; }

    /***************************************************************************
     *  Standard BST search.
     ***************************************************************************/

    /**
     * Returns the value associated with the given key.
     * @param key the key
     * @return the value associated with the given key if the key is in the symbol table
     *     and {@code null} if the key is not in the symbol table
     * @throws IllegalArgumentException if {@code key} is {@code null}
     */
    Value get(Key key) const {
        if (key == defaultValue<Key>()) 
            throw std::invalid_argument("argument to get() is null");
        return get(root_, key);
    }

    /**
     * Does this symbol table contain the given key?
     * @param key the key
     * @return {@code true} if this symbol table contains {@code key} and
     *     {@code false} otherwise
     * @throws IllegalArgumentException if {@code key} is {@code null}
     */
    bool contains(Key key) const { return get(key) != defaultValue<Key>(); }
    void put(Key key, Value val);
    void deleteMin();
    void deleteMax();
    void deleteItem(Key key);
    Key select(int rank) const;
    int rank(Key key) const;

/***************************************************************************
 *  Utility functions.
 ***************************************************************************/

/**
 * Returns the height of the BST (for debugging).
 * @return the height of the BST (a 1-node tree has height 0)
 */
    int height() const { return height(root_); }
    Key min() const;
    Key max() const;
    Key floor(Key key) const;
    Key ceiling(Key key) const;
    std::queue<Key> keys() const;
    std::queue<Key> keys(Key lo, Key hi) const;
    int size(Key lo, Key hi) const;

private:
    /***************************************************************************
     *  Node helper methods.
     ***************************************************************************/
    // is node x red; false if x is null ?
    bool isRed(Node* x) const {
        if (!x) return false;
        return x->color_ == RED;
    }

    // number of node in subtree rooted at x; 0 if x is null
    int size(Node* x) const {
        if (!x) return 0;
        return x->size_;
    } 

    // value associated with the given key in subtree rooted at x; null if no such key
    Value get(Node* x, Key key) const {
        while (x) {
            if (isLess(key, x->key_)) x = x->left_;
            else if (isLess(x->key_, key)) x = x->right_;
            else return x->value_;
        }

        return defaultValue<Value>();
    }
    Node* put(Node* h, Key key, Value val);
    Node* deleteMin(Node* h);
    Node* deleteMax(Node* h);
    Node* deleteItem(Node* h, Key key);
    Node* rotateRight(Node* h);
    Node* rotateLeft(Node* h);
    void flipColors(Node* h);
    Node* moveRedLeft(Node* h);
    Node* moveRedRight(Node* h);
    Node* balance(Node* h);

    int height(Node* x) const {
        if (!x) return -1;
        return 1 + std::max(height(x->left_), height(x->right_));
    }

    // the smallest key in subtree rooted at x; null if no such key
    Node* min(Node* x) const {
        // assert x != null;
        if (!x->left_) return x; 
        else return min(x->left_); 
    } 

    // the largest key in the subtree rooted at x; null if no such key
    Node* max(Node* x) const {
        // assert x != null;
        if (!x->right_) return x; 
        else return max(x->right_); 
    } 
    Node* floor(Node* x, Key key) const;
    Node* ceiling(Node* x, Key key) const;
    Key select(Node* x, int rank) const;
    int rank(Key key, Node* x) const;
    void keys(Node* x, std::queue<Key>& keys_queue, Key lo, Key hi) const;
    bool check() const;
// does this binary tree satisfy symmetric order?
// Note: this test also ensures that data structure is a binary tree since order is strict
    bool isBST() const {
        return isBST(root_, defaultValue<Key>(), defaultValue<Key>());
    }
    bool isBST(Node* x, Key min, Key max) const;
    // are the size fields correct?
    bool isSizeConsistent() const { return isSizeConsistent(root_); }
    bool isSizeConsistent(Node* x) const;
    bool isRankConsistent() const;
    bool is23() const { return is23(root_); }
    bool is23(Node* x) const;
    bool isBalanced() const;
    bool isBalanced(Node* x, int black) const;

private:
    constexpr static bool RED = true;
    constexpr static bool BLACK = false;
    Node *root_{nullptr};
};

#endif
