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
  Node(Key key, Value value, bool color, int size): _key(key), _value(value),
      _color(color), _size(size) {}
    Key _key;
    Value _value;
    Node *_left = nullptr;
    Node *_right = nullptr;
    bool _color;
    int _size;
  };
 public:
    /**
     * Initializes an empty symbol table.
     */
 RedBlackBST(): _root(nullptr) {}
    /**
     * Returns the number of key-value pairs in this symbol table.
     * @return the number of key-value pairs in this symbol table
     */
  int size() { return size(_root); }

   /**
     * Is this symbol table empty?
     * @return {@code true} if this symbol table is empty and {@code false} otherwise
     */
  bool isEmpty() { return _root == nullptr; }

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
  Value get(Key key) {
    if (key == defaultValue<Key>()) throw std::invalid_argument("argument to get() is null");
    return get(_root, key);
  }

    /**
     * Does this symbol table contain the given key?
     * @param key the key
     * @return {@code true} if this symbol table contains {@code key} and
     *     {@code false} otherwise
     * @throws IllegalArgumentException if {@code key} is {@code null}
     */
  bool contains(Key key) { return get(key) != defaultValue<Key>(); }
  void put(Key key, Value val);
  void deleteMin();
  void deleteMax();
  void deleteItem(Key key);
  Key select(int rank);
  int rank(Key key);

/***************************************************************************
*  Utility functions.
***************************************************************************/

/**
 * Returns the height of the BST (for debugging).
 * @return the height of the BST (a 1-node tree has height 0)
 */
  int height() { return height(_root); }
  Key min();
  Key max();
  Key floor(Key key);
  Key ceiling(Key key);
  std::queue<Key> keys();
  std::queue<Key> keys(Key lo, Key hi);
  int size(Key lo, Key hi);

 private:
   /***************************************************************************
    *  Node helper methods.
    ***************************************************************************/
    // is node x red; false if x is null ?
  bool isRed(Node* x) {
    if (!x) return false;
    return x->_color == RED;
  }

    // number of node in subtree rooted at x; 0 if x is null
  int size(Node* x) {
    if (!x) return 0;
    return x->_size;
  } 

  // value associated with the given key in subtree rooted at x; null if no such key
  Value get(Node* x, Key key) {
    while (x) {
      bool cmp = isLess(key, x->_key);
      if      (cmp) x = x->_left;
      else if (isLess(x->_key, key)) x = x->_right;
      else              return x->_value;
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

  int height(Node* x) {
    if (!x) return -1;
    return 1 + std::max(height(x->_left), height(x->_right));
  }

  // the smallest key in subtree rooted at x; null if no such key
  Node* min(Node* x) {
    // assert x != null;
    if (!x->_left) return x; 
    else                return min(x->_left); 
  } 

  // the largest key in the subtree rooted at x; null if no such key
  Node* max(Node* x) {
    // assert x != null;
    if (!x->_right) return x; 
    else                 return max(x->_right); 
  } 
  Node* floor(Node* x, Key key);
  Node* ceiling(Node* x, Key key);
  Key select(Node* x, int rank);
  int rank(Key key, Node* x);
  void keys(Node* x, std::queue<Key>& keys_queue, Key lo, Key hi);
  bool check();
// does this binary tree satisfy symmetric order?
// Note: this test also ensures that data structure is a binary tree since order is strict
  bool isBST() {
    return isBST(_root, defaultValue<Key>(), defaultValue<Key>());
  }
  bool isBST(Node* x, Key min, Key max);
  // are the size fields correct?
  bool isSizeConsistent() { return isSizeConsistent(_root); }
  bool isSizeConsistent(Node* x);
  bool isRankConsistent();
  bool is23() { return is23(_root); }
  bool is23(Node* x);
  bool isBalanced();
  bool isBalanced(Node* x, int black);


 private:
  /* struct Node { */
  /* Node(Key key, Value value, bool color, int size): _key(key), _value(value), */
  /*     _color(color), _size(size) {} */
  /*   Key _key; */
  /*   Value _value; */
  /*   Node *_left; */
  /*   Node *_right; */
  /*   bool _color; */
  /*   int _size; */
  /* }; */

  const static bool RED = true;
  const static bool BLACK = false;
  Node *_root;
};

#endif
