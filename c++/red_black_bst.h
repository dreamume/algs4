#ifndef RED_BLACK_BST_H
#define RED_BLACK_BST_H

#include <algorithm>
#include <queue>
#include <string>
#include <exception>

namespace algs4 {
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
  constexpr static bool RED = true;
  constexpr static bool BLACK = false;

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
  Node *root_{nullptr};
public:
  /**
   * Initializes an empty symbol table.
   */
  RedBlackBST() = default;
  RedBlackBST(const RedBlackBST& other) = delete;
  RedBlackBST &operator=(const RedBlackBST& other) = delete;
  RedBlackBST(RedBlackBST&& other) = default;
  RedBlackBST &operator=(RedBlackBST&& other) = default;
  /**
   * Returns the number of key-value pairs in this symbol table.
   * @return the number of key-value pairs in this symbol table
   */
  int Size() const { return Size(root_); }

  /**
   * Is this symbol table empty?
   * @return {@code true} if this symbol table is empty and {@code false} otherwise
   */
  bool IsEmpty() const { return root_ == nullptr; }

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
  Value Get(Key key) const {
    if (key == defaultValue<Key>()) 
      throw std::invalid_argument("argument to Get() is null");
    return Get(root_, key);
  }

  /**
   * Does this symbol table contain the given key?
   * @param key the key
   * @return {@code true} if this symbol table contains {@code key} and
   *     {@code false} otherwise
   * @throws IllegalArgumentException if {@code key} is {@code null}
   */
  bool Contains(Key key) const { return Get(key) != defaultValue<Key>(); }
  /**
   * Inserts the specified key-value pair into the symbol table, overwriting the old 
   * value with the new value if the symbol table already Contains the specified key.
   * Deletes the specified key (and its associated value) from this symbol table
   * if the specified value is {@code null}.
   *
   * @param key the key
   * @param val the value
   * @throws IllegalArgumentException if {@code key} is {@code null}
   */
  void Put(Key key, Value val) {
    if (key == defaultValue<Key>()) 
      throw std::invalid_argument("first argument to Put() is null");
    if (val == defaultValue<Value>()) return;

    root_ = Put(root_, key, val);
    root_->color_ = BLACK;
    // assert Check();
  }

  /**
   * Removes the smallest key and associated value from the symbol table.
   * @throws NoSuchElementException if the symbol table is empty
   */
  void DeleteMin() {
    if (IsEmpty()) throw std::invalid_argument("BST underflow");

    // if both children of root are black, set root to red
    if (!IsRed(root_->left) && !IsRed(root_->right))
      root_->color_ = RED;

    root_ = DeleteMin(root_);
    if (!IsEmpty()) root_->color_ = BLACK;
    // assert Check();
  }
  /**
   * Removes the largest key and associated value from the symbol table.
   * @throws NoSuchElementException if the symbol table is empty
   */
  void DeleteMax() {
    if (IsEmpty()) throw std::invalid_argument("BST underflow");

    // if both children of root are black, set root to red
    if (!IsRed(root_->left_) && !IsRed(root_->right_))
      root_->color_ = RED;

    root_ = DeleteMax(root_);
    if (!IsEmpty()) root_->color_ = BLACK;
    // assert Check();
  }
  /**
   * Removes the specified key and its associated value from this symbol table     
   * (if the key is in this symbol table).    
   *
   * @param  key the key
   * @throws IllegalArgumentException if {@code key} is {@code null}
   */
  void DeleteItem(Key key) {
    if (key == defaultValue<Key>()) 
      throw std::invalid_argument("argument to DeleteItem() is null");
    if (!Contains(key)) return;

    // if both children of root are black, set root to red
    if (!IsRed(root_->left_) && !IsRed(root_->right_))
      root_->color_ = RED;

    root_ = DeleteItem(root_, key);
    if (!IsEmpty()) root_->color_ = BLACK;
    // assert Check();
  }
  /**
   * Return the key in the symbol table of a given {@code rank}.
   * This key has the property that there are {@code rank} keys in
   * the symbol table that are smaller. In other words, this key is the
   * ({@code rank}+1)st smallest key in the symbol table.
   *
   * @param  rank the order statistic
   * @return the key in the symbol table of given {@code rank}
   * @throws IllegalArgumentException unless {@code rank} is between 0 and
   *        <em>n</em>–1
   */
  Key Select(int rank) const {
    if (rank < 0 || rank >= Size())
      throw std::invalid_argument("argument to Select() is invalid: " + 
                                  std::to_string(rank));

    return Select(root_, rank);
  }
  /**
   * Return the number of keys in the symbol table strictly less than {@code key}.
   * @param key the key
   * @return the number of keys in the symbol table strictly less than {@code key}
   * @throws IllegalArgumentException if {@code key} is {@code null}
   */
  int Rank(Key key) const {
    if (key == defaultValue<Key>()) 
      throw std::invalid_argument("argument to rank() is null");
    return Rank(key, root_);
  } 

  /***************************************************************************
   *  Utility functions.
   ***************************************************************************/

  /**
   * Returns the height of the BST (for debugging).
   * @return the height of the BST (a 1-node tree has height 0)
   */
  int Height() const { return Height(root_); }
  /**
   * Returns the smallest key in the symbol table.
   * @return the smallest key in the symbol table
   * @throws NoSuchElementException if the symbol table is empty
   */
  Key Min() const {
    if (IsEmpty()) 
      throw std::invalid_argument("calls Min() with empty symbol table");
    return Min(root_)->key_;
  } 
  /**
   * Returns the largest key in the symbol table.
   * @return the largest key in the symbol table
   * @throws NoSuchElementException if the symbol table is empty
   */
  Key Max() const {
    if (IsEmpty()) 
      throw std::invalid_argument("calls Max() with empty symbol table");
    return Max(root_)->key_;
  } 
  /**
   * Returns the largest key in the symbol table less than or equal to {@code key}.
   * @param key the key
   * @return the largest key in the symbol table less than or equal to {@code key}
   * @throws NoSuchElementException if there is no such key
   * @throws IllegalArgumentException if {@code key} is {@code null}
   */
  Key Floor(Key key) const {
    if (key == defaultValue<Key>()) 
      throw std::invalid_argument("argument to Floor() is null");
    if (IsEmpty()) 
      throw std::out_of_range("calls Floor() with empty symbol table");
    Node* x = Floor(root_, key);
    if (!x) throw std::invalid_argument("argument to Floor() is too small");
    else return x->key_;
  }    
  /**
   * Returns the smallest key in the symbol table greater than or equal to {@code key}.
   * @param key the key
   * @return the smallest key in the symbol table greater than or equal to {@code key}
   * @throws NoSuchElementException if there is no such key
   * @throws IllegalArgumentException if {@code key} is {@code null}
   */
  Key Ceiling(Key key) const {
    if (key == defaultValue<Key>()) 
      throw std::invalid_argument("argument to Ceiling() is null");
    if (IsEmpty()) 
      throw std::out_of_range("calls Ceiling() with empty symbol table");
    Node* x = Ceiling(root_, key);
    if (!x) throw std::invalid_argument("argument to Ceiling() is too small");
    else return x->key_;  
  }
  /**
   * Returns all keys in the symbol table as an {@code Iterable}.
   * To iterate over all of the keys in the symbol table named {@code st},
   * use the foreach notation: {@code for (Key key : st.keys())}.
   * @return all keys in the symbol table as an {@code Iterable}
   */
  std::queue<Key> Keys() const {
    if (IsEmpty()) return std::queue<Key>();
    return Keys(Min(), Max());
  }
  /**
   * Returns all keys in the symbol table in the given range,
   * as an {@code Iterable}.
   *
   * @param  lo minimum endpoint
   * @param  hi maximum endpoint
   * @return all keys in the symbol table between {@code lo} 
   *    (inclusive) and {@code hi} (inclusive) as an {@code Iterable}
   * @throws IllegalArgumentException if either {@code lo} or {@code hi}
   *    is {@code null}
   */
  std::queue<Key> Keys(Key lo, Key hi) const {
    if (lo == defaultValue<Key>()) 
      throw std::invalid_argument("first argument to keys() is null");
    if (hi == defaultValue<Key>()) 
      throw std::invalid_argument("second argument to keys() is null");

    std::queue<Key> keys_queue;
    // if (IsEmpty() || lo.compareTo(hi) > 0) return queue;
    Keys(root_, keys_queue, lo, hi);
    return keys_queue;
  } 
  /**
   * Returns the number of keys in the symbol table in the given range.
   *
   * @param  lo minimum endpoint
   * @param  hi maximum endpoint
   * @return the number of keys in the symbol table between {@code lo} 
   *    (inclusive) and {@code hi} (inclusive)
   * @throws IllegalArgumentException if either {@code lo} or {@code hi}
   *    is {@code null}
   */
  int Size(Key lo, Key hi) const {
    if (lo == defaultValue<Key>())
      throw std::invalid_argument("first argument to Size() is null");
    if (hi == defaultValue<Key>()) 
      throw std::invalid_argument("second argument to Size() is null");

    if (isLess(hi, lo)) return 0;
    if (Contains(hi)) return Rank(hi) - Rank(lo) + 1;
    else return Rank(hi) - Rank(lo);
  }

private:
  /***************************************************************************
   *  Node helper methods.
   ***************************************************************************/
  // is node x red; false if x is null ?
  bool IsRed(Node* x) const {
    if (!x) return false;
    return x->color_ == RED;
  }

  // number of node in subtree rooted at x; 0 if x is null
  int Size(Node* x) const {
    if (!x) return 0;
    return x->size_;
  } 

  // value associated with the given key in subtree rooted at x; null if no such key
  Value Get(Node* x, Key key) const {
    while (x) {
      if (isLess(key, x->key_)) x = x->left_;
      else if (isLess(x->key_, key)) x = x->right_;
      else return x->value_;
    }

    return defaultValue<Value>();
  }
  Node* Put(Node* h, Key key, Value val) {
    if (!h) return new Node(key, val, RED, 1);

    if (isLess(key, h->key_)) h->left_ = Put(h->left_,  key, val); 
    else if (isLess(h->key_, key)) h->right_ = Put(h->right_, key, val); 
    else h->value_ = val;

    // fix-up any right-leaning links
    if (IsRed(h->right_) && !IsRed(h->left_)) h = RotateLeft(h);
    if (IsRed(h->left_)  &&  IsRed(h->left_->left_)) h = RotateRight(h);
    if (IsRed(h->left_)  &&  IsRed(h->right_)) FlipColors(h);
    h->size_ = Size(h->left_) + Size(h->right_) + 1;

    return h;
  }
  Node* DeleteMin(Node* h) {
    if (!h->left_) {
      delete h;
      return nullptr;
    }

    if (!IsRed(h->left_) && !IsRed(h->left_->left_))
      h = MoveRedLeft(h);

    h->left_ = DeleteMin(h->left_);
    return Balance(h);
  }
  Node* DeleteMax(Node* h) {
    if (IsRed(h->left_)) h = RotateRight(h);

    if (!h->right_) {
      delete h;
      return nullptr;
    }

    if (!IsRed(h->right_) && !IsRed(h->right_->left_))
      h = MoveRedRight(h);

    h->right_ = DeleteMax(h->right_);

    return Balance(h);
  }
  Node* DeleteItem(Node* h, Key key) {
    // assert Get(h, key) != null;

    if (isLess(key, h->key_)) {
      if (!IsRed(h->left_) && !IsRed(h->left_->left_))
        h = MoveRedLeft(h);
      h->left_ = DeleteItem(h->left_, key);
    } else {
      if (IsRed(h->left_))
        h = RotateRight(h);
      if (!isLess(key, h->key_) && !isLess(h->key_, key) && !h->right_) {
        delete h;
        return nullptr;
      }
      if (!IsRed(h->right_) && !IsRed(h->right_->left_))
        h = MoveRedRight(h);
      if (!isLess(key, h->key_) && !isLess(h->key_, key)) {
        Node* x = Min(h->right_);
        h->key_ = x->key_;
        h->_val = x->_val;
        // h.val = Get(h.right, Min(h.right).key);
        // h.key = Min(h.right).key;
        h->right_ = DeleteMin(h->right_);
      } else {
        h->right_ = DeleteItem(h->right_, key);
      }
    }

    return Balance(h);
  }
  Node* RotateRight(Node* h) {
    // assert (h != null) && IsRed(h.left);
    Node* x = h->left_;
    h->left_ = x->right_;
    x->right_ = h;
    x->color_ = x->right_->color_;
    x->right_->color_ = RED;
    x->size_ = h->size_;
    h->size_ = Size(h->left_) + Size(h->right_) + 1;

    return x;
  }
  Node* RotateLeft(Node* h) {
    // assert (h != null) && IsRed(h.right);
    Node* x = h->right_;
    h->right_ = x->left_;
    x->left_ = h;
    x->color_ = x->left_->color_;
    x->left_->color_ = RED;
    x->size_ = h->size_;
    h->size_ = Size(h->left_) + Size(h->right_) + 1;

    return x;
  }
  void FlipColors(Node* h) {
    // h must have opposite color of its two children
    // assert (h != null) && (h.left != null) && (h.right != null);
    // assert (!IsRed(h) &&  IsRed(h.left) &&  IsRed(h.right))
    //    || (IsRed(h)  && !IsRed(h.left) && !IsRed(h.right));
    h->color_ = !h->color_;
    h->left_->color_ = !h->left_->color_;
    h->right_->color_ = !h->right_->color_;
  }
  Node* MoveRedLeft(Node* h) {
    // assert (h != null);
    // assert IsRed(h) && !IsRed(h.left) && !IsRed(h.left.left);

    FlipColors(h);
    if (IsRed(h->right_->left_)) { 
      h->right_ = RotateRight(h->right_);
      h = RotateLeft(h);
      FlipColors(h);
    }

    return h;
  }
  Node* MoveRedRight(Node* h) {
    // assert (h != null);
    // assert IsRed(h) && !IsRed(h.right) && !IsRed(h.right.left);
    FlipColors(h);
    if (IsRed(h->left_->left_)) { 
      h = RotateRight(h);
      FlipColors(h);
    }

    return h;
  }
  Node* Balance(Node* h) {
    // assert (h != null);

    if (IsRed(h->right_)) h = RotateLeft(h);
    if (IsRed(h->left_) && IsRed(h->left_->left_)) h = RotateRight(h);
    if (IsRed(h->left_) && IsRed(h->right_)) FlipColors(h);

    h->size_ = Size(h->left_) + Size(h->right_) + 1;
    return h;
  }

  int Height(Node* x) const {
    if (!x) return -1;
    return 1 + std::max(Height(x->left_), Height(x->right_));
  }

  // the smallest key in subtree rooted at x; null if no such key
  Node* Min(Node* x) const {
    // assert x != null;
    if (!x->left_) return x; 
    else return Min(x->left_); 
  } 

  // the largest key in the subtree rooted at x; null if no such key
  Node* Max(Node* x) const {
    // assert x != null;
    if (!x->right_) return x; 
    else return Max(x->right_); 
  } 
  Node* Floor(Node* x, Key key) const {
    if (!x) return nullptr;
    bool cmp = isLess(key, x->key_);
    if (!cmp && !isLess(x->key_, key)) return x;
    if (cmp)  return Floor(x->left_, key);
    Node t = Floor(x->right_, key);
    if (t) return t; 
    else return x;
  }
  Node* Ceiling(Node* x, Key key) const {
    if (!x) return nullptr;
    bool cmp = isLess(key, x->key_);
    if (!cmp && !isLess(x->key_, key)) return x;
    if (!cmp)  return Ceiling(x->right_, key);
    Node* t = Ceiling(x->left_, key);
    if (t) return t; 
    else return x;
  }
  Key Select(Node* x, int rank) const {
    if (!x) return nullptr;
    int leftSize = Size(x->left_);
    if (leftSize > rank) return Select(x->left_,  rank);
    else if (leftSize < rank) return Select(x->right_, rank - leftSize - 1); 
    else return x->key_;
  }
  int Rank(Key key, Node* x) const {
    if (!x) return 0; 
    bool cmp = isLess(key, x->key_); 
    if (!cmp && !isLess(x->key_, key)) return Size(x->left_);
    else if (cmp) return Rank(key, x->left_); 
    else return 1 + Size(x->left_) + Rank(key, x->right_); 
  } 
  // add the keys between lo and hi in the subtree rooted at x
  // to the queue
  void Keys(Node* x, std::queue<Key>& keys_queue, Key lo, Key hi) const {
    if (!x) return; 
    bool cmplo = isLess(lo, x->key_); 
    bool cmphi = isLess(hi, x->key_); 
    if (cmplo) Keys(x->left_, keys_queue, lo, hi); 
    if ((cmplo || !isLess(x->key_, lo)) && !cmphi) keys_queue.push(x->key_); 
    if (isLess(x->key_, hi)) Keys(x->right_, keys_queue, lo, hi); 
  } 
  bool Check() const {
    if (!IsBST())            printf("Not in symmetric order\n");
    if (!IsSizeConsistent()) printf("Subtree counts not consistent\n");
    if (!IsRankConsistent()) printf("Ranks not consistent\n");
    if (!Is23())             printf("Not a 2-3 tree\n");
    if (!IsBalanced())       printf("Not balanced\n");
    return IsBST() && IsSizeConsistent() && IsRankConsistent() && Is23() && IsBalanced();
  }
  // does this binary tree satisfy symmetric order?
  // Note: this test also ensures that data structure is a binary tree since order is strict
  bool IsBST() const {
    return IsBST(root_, defaultValue<Key>(), defaultValue<Key>());
  }
  bool IsBST(Node* x, Key min, Key max) const {
    if (!x) return true;
    if (min != defaultValue<Key>() && !isLess(min, x->key_)) return false;
    if (max != defaultValue<Key>() && !isLess(x->key_, max)) return false;
    return IsBST(x->left_, min, x->key_) && IsBST(x->right_, x->key_, max);
  } 
  // are the size fields correct?
  bool IsSizeConsistent() const { return IsSizeConsistent(root_); }
  bool IsSizeConsistent(Node* x) const {
      if (!x) return true;
      if (x->size_ != Size(x->left_) + Size(x->right_) + 1) return false;
      return IsSizeConsistent(x->left_) && IsSizeConsistent(x->right_);
    } 
  bool IsRankConsistent() const {
    for (int i = 0; i < Size(); i++)
      if (i != Rank(Select(i))) return false;
    for (const Key& key : Keys())
      if (isLess(key, Select(Rank(key))) || isLess(Select(Rank(key)), key))
        return false;

    return true;
  }
  bool Is23() const { return Is23(root_); }
  bool Is23(Node* x) const {
    if (!x) return true;
    if (IsRed(x->right_)) return false;
    if (x != root_ && IsRed(x) && IsRed(x->left_)) return false;
    return Is23(x->left_) && Is23(x->right_);
  } 
  bool IsBalanced() const {
    int black = 0;     // number of black links on path from root to min
    Node* x = root_;
    while (x) {
      if (!IsRed(x)) black++;
      x = x->left_;
    }

    return IsBalanced(root_, black);
  }
  bool IsBalanced(Node* x, int black) const {
    if (!x) return black == 0;
    if (!IsRed(x)) black--;
    return IsBalanced(x->left_, black) && IsBalanced(x->right_, black);
  } 
};
}

#endif
