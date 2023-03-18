/******************************************************************************
 *  Compilation:  clang++ -g -DDebug red_black_bst.cc -std=c++11 -o red_black_bst
 *  Execution:    red_black_bst input.txt
 *  Dependencies: 
 *  Data files:   https://algs4.cs.princeton.edu/33balanced/tinyST.txt  
 *    
 *  A symbol table implemented using a left-leaning red-black BST.
 *  This is the 2-3 version.
 *
 *  Note: commented out assertions because DrJava now enables assertions
 *        by default.
 *
 *  % more tinyST.txt
 *  S E A R C H E X A M P L E
 *  
 *  % java RedBlackBST < tinyST.txt
 *  A 8
 *  C 4
 *  E 12
 *  H 5
 *  L 11
 *  M 9
 *  P 10
 *  R 3
 *  S 0
 *  X 7
 *
 ******************************************************************************/

/**
 *  The {@code BST} class represents an ordered symbol table of generic
 *  key-value pairs.
 *  It supports the usual <em>put</em>, <em>get</em>, <em>contains</em>,
 *  <em>delete</em>, <em>size</em>, and <em>is-empty</em> methods.
 *  It also provides ordered methods for finding the <em>minimum</em>,
 *  <em>maximum</em>, <em>floor</em>, and <em>ceiling</em>.
 *  It also provides a <em>keys</em> method for iterating over all of the keys.
 *  A symbol table implements the <em>associative array</em> abstraction:
 *  when associating a value with a key that is already in the symbol table,
 *  the convention is to replace the old value with the new value.
 *  Unlike {@link java.util.Map}, this class uses the convention that
 *  values cannot be {@code null}—setting the
 *  value associated with a key to {@code null} is equivalent to deleting the key
 *  from the symbol table.
 *  <p>
 *  It requires that
 *  the key type implements the {@code Comparable} interface and calls the
 *  {@code compareTo()} and method to compare two keys. It does not call either
 *  {@code equals()} or {@code hashCode()}.
 *  <p>
 *  This implementation uses a <em>left-leaning red-black BST</em>. 
 *  The <em>put</em>, <em>get</em>, <em>contains</em>, <em>remove</em>,
 *  <em>minimum</em>, <em>maximum</em>, <em>ceiling</em>, <em>floor</em>,
 *  <em>rank</em>, and <em>select</em> operations each take
 *  &Theta;(log <em>n</em>) time in the worst case, where <em>n</em> is the
 *  number of key-value pairs in the symbol table.
 *  The <em>size</em>, and <em>is-empty</em> operations take &Theta;(1) time.
 *  The <em>keys</em> methods take
 *  <em>O</em>(log <em>n</em> + <em>m</em>) time, where <em>m</em> is
 *  the number of keys returned by the iterator.
 *  Construction takes &Theta;(1) time.
 *  <p>
 *  For alternative implementations of the symbol table API, see {@link ST},
 *  {@link BinarySearchST}, {@link SequentialSearchST}, {@link BST},
 *  {@link SeparateChainingHashST}, {@link LinearProbingHashST}, and
 *  {@link AVLTreeST}.
 *  For additional documentation, see
 *  <a href="https://algs4.cs.princeton.edu/33balanced">Section 3.3</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#include "red_black_bst.h"

#include <iostream>
#include <cstdio>
#include <string>

using std::queue;
using std::string;

/***************************************************************************
*  Red-black tree insertion.
***************************************************************************/

/**
 * Inserts the specified key-value pair into the symbol table, overwriting the old 
 * value with the new value if the symbol table already contains the specified key.
 * Deletes the specified key (and its associated value) from this symbol table
 * if the specified value is {@code null}.
 *
 * @param key the key
 * @param val the value
 * @throws IllegalArgumentException if {@code key} is {@code null}
 */
template <typename Key, typename Value>
void RedBlackBST<Key, Value>::put(Key key, Value val) {
    if (key == defaultValue<Key>()) 
        throw std::invalid_argument("first argument to put() is null");
    if (val == defaultValue<Value>()) return;

    root_ = put(root_, key, val);
    root_->color_ = BLACK;
    // assert check();
}

// insert the key-value pair in the subtree rooted at h
template <typename Key, typename Value>
typename RedBlackBST<Key, Value>::Node* RedBlackBST<Key, Value>::put(
  typename RedBlackBST<Key, Value>::Node* h, Key key, Value val) {
    if (!h) return new Node(key, val, RED, 1);

    if (isLess(key, h->key_)) h->left_ = put(h->left_,  key, val); 
    else if (isLess(h->key_, key)) h->right_ = put(h->right_, key, val); 
    else h->value_ = val;

    // fix-up any right-leaning links
    if (isRed(h->right_) && !isRed(h->left_)) h = rotateLeft(h);
    if (isRed(h->left_)  &&  isRed(h->left_->left_)) h = rotateRight(h);
    if (isRed(h->left_)  &&  isRed(h->right_)) flipColors(h);
    h->size_ = size(h->left_) + size(h->right_) + 1;

    return h;
}

/***************************************************************************
*  Red-black tree deletion.
***************************************************************************/

/**
 * Removes the smallest key and associated value from the symbol table.
 * @throws NoSuchElementException if the symbol table is empty
 */
template <typename Key, typename Value>
void RedBlackBST<Key, Value>::deleteMin() {
    if (isEmpty()) throw std::invalid_argument("BST underflow");

    // if both children of root are black, set root to red
    if (!isRed(root_->left) && !isRed(root_->right))
        root_->color_ = RED;

    root_ = deleteMin(root_);
    if (!isEmpty()) root_->color_ = BLACK;
    // assert check();
}

// delete the key-value pair with the minimum key rooted at h
template <typename Key, typename Value>
typename RedBlackBST<Key, Value>::Node* RedBlackBST<Key, Value>::deleteMin(
    typename RedBlackBST<Key, Value>::Node* h) {
    if (!h->left_) {
        delete h;
        return nullptr;
    }

    if (!isRed(h->left_) && !isRed(h->left_->left_))
        h = moveRedLeft(h);

    h->left_ = deleteMin(h->left_);
    return balance(h);
}


/**
 * Removes the largest key and associated value from the symbol table.
 * @throws NoSuchElementException if the symbol table is empty
 */
template <typename Key, typename Value>
void RedBlackBST<Key, Value>::deleteMax() {
    if (isEmpty()) throw std::invalid_argument("BST underflow");

    // if both children of root are black, set root to red
    if (!isRed(root_->left_) && !isRed(root_->right_))
        root_->color_ = RED;

    root_ = deleteMax(root_);
    if (!isEmpty()) root_->color_ = BLACK;
    // assert check();
}

// delete the key-value pair with the maximum key rooted at h
template <typename Key, typename Value>
typename RedBlackBST<Key, Value>::Node* RedBlackBST<Key, Value>::deleteMax(
    typename RedBlackBST<Key, Value>::Node* h) {
    if (isRed(h->left_)) h = rotateRight(h);

    if (!h->right_) {
        delete h;
        return nullptr;
    }

    if (!isRed(h->right_) && !isRed(h->right_->left_))
        h = moveRedRight(h);

    h->right_ = deleteMax(h->right_);

    return balance(h);
}

/**
 * Removes the specified key and its associated value from this symbol table     
 * (if the key is in this symbol table).    
 *
 * @param  key the key
 * @throws IllegalArgumentException if {@code key} is {@code null}
 */
template <typename Key, typename Value>
void RedBlackBST<Key, Value>::deleteItem(Key key) {
    if (key == defaultValue<Key>()) 
        throw std::invalid_argument("argument to deleteItem() is null");
    if (!contains(key)) return;

    // if both children of root are black, set root to red
    if (!isRed(root_->left_) && !isRed(root_->right_))
        root_->color_ = RED;

    root_ = deleteItem(root_, key);
    if (!isEmpty()) root_->color_ = BLACK;
    // assert check();
}

// delete the key-value pair with the given key rooted at h
template <typename Key, typename Value>
typename RedBlackBST<Key, Value>::Node* RedBlackBST<Key, Value>::deleteItem(
    typename RedBlackBST<Key, Value>::Node* h, Key key) {
    // assert get(h, key) != null;

    if (isLess(key, h->key_)) {
        if (!isRed(h->left_) && !isRed(h->left_->left_))
            h = moveRedLeft(h);
        h->left_ = deleteItem(h->left_, key);
    } else {
        if (isRed(h->left_))
            h = rotateRight(h);
        if (!isLess(key, h->key_) && !isLess(h->key_, key) && !h->right_) {
            delete h;
            return nullptr;
        }
        if (!isRed(h->right_) && !isRed(h->right_->left_))
            h = moveRedRight(h);
        if (!isLess(key, h->key_) && !isLess(h->key_, key)) {
            Node* x = min(h->right_);
            h->key_ = x->key_;
            h->_val = x->_val;
            // h.val = get(h.right, min(h.right).key);
            // h.key = min(h.right).key;
            h->right_ = deleteMin(h->right_);
        } else {
            h->right_ = deleteItem(h->right_, key);
        }
    }

    return balance(h);
}

/***************************************************************************
*  Red-black tree helper functions.
***************************************************************************/

// make a left-leaning link lean to the right
template <typename Key, typename Value>
typename RedBlackBST<Key, Value>::Node* RedBlackBST<Key, Value>::rotateRight(
    typename RedBlackBST<Key, Value>::Node* h) {
    // assert (h != null) && isRed(h.left);
    Node* x = h->left_;
    h->left_ = x->right_;
    x->right_ = h;
    x->color_ = x->right_->color_;
    x->right_->color_ = RED;
    x->size_ = h->size_;
    h->size_ = size(h->left_) + size(h->right_) + 1;

    return x;
}

// make a right-leaning link lean to the left
template <typename Key, typename Value>
typename RedBlackBST<Key, Value>::Node* RedBlackBST<Key, Value>::rotateLeft(
    typename RedBlackBST<Key, Value>::Node* h) {
    // assert (h != null) && isRed(h.right);
    Node* x = h->right_;
    h->right_ = x->left_;
    x->left_ = h;
    x->color_ = x->left_->color_;
    x->left_->color_ = RED;
    x->size_ = h->size_;
    h->size_ = size(h->left_) + size(h->right_) + 1;

    return x;
}

// flip the colors of a node and its two children
template <typename Key, typename Value>
void RedBlackBST<Key, Value>::flipColors(
    typename RedBlackBST<Key, Value>::Node* h) {
    // h must have opposite color of its two children
    // assert (h != null) && (h.left != null) && (h.right != null);
    // assert (!isRed(h) &&  isRed(h.left) &&  isRed(h.right))
    //    || (isRed(h)  && !isRed(h.left) && !isRed(h.right));
    h->color_ = !h->color_;
    h->left_->color_ = !h->left_->color_;
    h->right_->color_ = !h->right_->color_;
}

// Assuming that h is red and both h.left and h.left.left
// are black, make h.left or one of its children red.
template <typename Key, typename Value>
typename RedBlackBST<Key, Value>::Node* RedBlackBST<Key, Value>::moveRedLeft(
    typename RedBlackBST<Key, Value>::Node* h) {
    // assert (h != null);
    // assert isRed(h) && !isRed(h.left) && !isRed(h.left.left);

    flipColors(h);
    if (isRed(h->right_->left_)) { 
        h->right_ = rotateRight(h->right_);
        h = rotateLeft(h);
        flipColors(h);
    }

    return h;
}

// Assuming that h is red and both h.right and h.right.left
// are black, make h.right or one of its children red.
template <typename Key, typename Value>
typename RedBlackBST<Key, Value>::Node* RedBlackBST<Key, Value>::moveRedRight(
    typename RedBlackBST<Key, Value>::Node* h) {
    // assert (h != null);
    // assert isRed(h) && !isRed(h.right) && !isRed(h.right.left);
    flipColors(h);
    if (isRed(h->left_->left_)) { 
        h = rotateRight(h);
        flipColors(h);
    }

    return h;
}

// restore red-black tree invariant
template <typename Key, typename Value>
typename RedBlackBST<Key, Value>::Node* RedBlackBST<Key, Value>::balance(
    typename RedBlackBST<Key, Value>::Node* h) {
    // assert (h != null);

    if (isRed(h->right_)) h = rotateLeft(h);
    if (isRed(h->left_) && isRed(h->left_->left_)) h = rotateRight(h);
    if (isRed(h->left_) && isRed(h->right_)) flipColors(h);

    h->size_ = size(h->left_) + size(h->right_) + 1;
    return h;
}


/***************************************************************************
*  Ordered symbol table methods.
***************************************************************************/

/**
 * Returns the smallest key in the symbol table.
 * @return the smallest key in the symbol table
 * @throws NoSuchElementException if the symbol table is empty
 */
template <typename Key, typename Value>
Key RedBlackBST<Key, Value>::min() const {
    if (isEmpty()) 
        throw std::invalid_argument("calls min() with empty symbol table");
    return min(root_)->key_;
} 

/**
 * Returns the largest key in the symbol table.
 * @return the largest key in the symbol table
 * @throws NoSuchElementException if the symbol table is empty
 */
template <typename Key, typename Value>
Key RedBlackBST<Key, Value>::max() const {
    if (isEmpty()) 
        throw std::invalid_argument("calls max() with empty symbol table");
    return max(root_)->key_;
} 


/**
 * Returns the largest key in the symbol table less than or equal to {@code key}.
 * @param key the key
 * @return the largest key in the symbol table less than or equal to {@code key}
 * @throws NoSuchElementException if there is no such key
 * @throws IllegalArgumentException if {@code key} is {@code null}
 */
template <typename Key, typename Value>
Key RedBlackBST<Key, Value>::floor(Key key) const {
    if (key == defaultValue<Key>()) 
        throw std::invalid_argument("argument to floor() is null");
    if (isEmpty()) 
        throw std::out_of_range("calls floor() with empty symbol table");
    Node* x = floor(root_, key);
    if (!x) throw std::invalid_argument("argument to floor() is too small");
    else return x->key_;
}    

// the largest key in the subtree rooted at x less than or equal to the given key
template <typename Key, typename Value>
typename RedBlackBST<Key, Value>::Node* RedBlackBST<Key, Value>::floor(
    typename RedBlackBST<Key, Value>::Node* x, Key key) const {
    if (!x) return nullptr;
    bool cmp = isLess(key, x->key_);
    if (!cmp && !isLess(x->key_, key)) return x;
    if (cmp)  return floor(x->left_, key);
    Node t = floor(x->right_, key);
    if (t) return t; 
    else return x;
}

/**
 * Returns the smallest key in the symbol table greater than or equal to {@code key}.
 * @param key the key
 * @return the smallest key in the symbol table greater than or equal to {@code key}
 * @throws NoSuchElementException if there is no such key
 * @throws IllegalArgumentException if {@code key} is {@code null}
 */
template <typename Key, typename Value>
Key RedBlackBST<Key, Value>::ceiling(Key key) const {
    if (key == defaultValue<Key>()) 
        throw std::invalid_argument("argument to ceiling() is null");
    if (isEmpty()) 
        throw std::out_of_range("calls ceiling() with empty symbol table");
    Node* x = ceiling(root_, key);
    if (!x) throw std::invalid_argument("argument to ceiling() is too small");
    else return x->key_;  
}

// the smallest key in the subtree rooted at x greater than or equal to the given key
template <typename Key, typename Value>
typename RedBlackBST<Key, Value>::Node* RedBlackBST<Key, Value>::ceiling(
    typename RedBlackBST<Key, Value>::Node* x, Key key) const {
    if (!x) return nullptr;
    bool cmp = isLess(key, x->key_);
    if (!cmp && !isLess(x->key_, key)) return x;
    if (!cmp)  return ceiling(x->right_, key);
    Node* t = ceiling(x->left_, key);
    if (t) return t; 
    else return x;
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
template <typename Key, typename Value>
Key RedBlackBST<Key, Value>::select(int rank) const {
    if (rank < 0 || rank >= size())
        throw std::invalid_argument("argument to select() is invalid: " + 
                                    std::to_string(rank));

    return select(root_, rank);
}

// Return key in BST rooted at x of given rank.
// Precondition: rank is in legal range.
template <typename Key, typename Value>
Key RedBlackBST<Key, Value>::select(
    typename RedBlackBST<Key, Value>::Node* x, int rank) const {
    if (!x) return nullptr;
    int leftSize = size(x->left_);
    if (leftSize > rank) return select(x->left_,  rank);
    else if (leftSize < rank) return select(x->right_, rank - leftSize - 1); 
    else return x->key_;
}

/**
 * Return the number of keys in the symbol table strictly less than {@code key}.
 * @param key the key
 * @return the number of keys in the symbol table strictly less than {@code key}
 * @throws IllegalArgumentException if {@code key} is {@code null}
 */
template <typename Key, typename Value>
int RedBlackBST<Key, Value>::rank(Key key) const {
    if (key == defaultValue<Key>()) 
        throw std::invalid_argument("argument to rank() is null");
    return rank(key, root_);
} 

// number of keys less than key in the subtree rooted at x
template <typename Key, typename Value>
int RedBlackBST<Key, Value>::rank(Key key, 
                                  typename RedBlackBST<Key, Value>::Node* x) const {
    if (!x) return 0; 
    bool cmp = isLess(key, x->key_); 
    if (!cmp && !isLess(x->key_, key)) return size(x->left_);
    else if (cmp) return rank(key, x->left_); 
    else return 1 + size(x->left_) + rank(key, x->right_); 
} 

/***************************************************************************
*  Range count and range search.
***************************************************************************/

/**
 * Returns all keys in the symbol table as an {@code Iterable}.
 * To iterate over all of the keys in the symbol table named {@code st},
 * use the foreach notation: {@code for (Key key : st.keys())}.
 * @return all keys in the symbol table as an {@code Iterable}
 */
template <typename Key, typename Value>
queue<Key> RedBlackBST<Key, Value>::keys() const {
    if (isEmpty()) return queue<Key>();
    return keys(min(), max());
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
template <typename Key, typename Value>
queue<Key> RedBlackBST<Key, Value>::keys(Key lo, Key hi) const {
    if (lo == defaultValue<Key>()) 
        throw std::invalid_argument("first argument to keys() is null");
    if (hi == defaultValue<Key>()) 
        throw std::invalid_argument("second argument to keys() is null");

    queue<Key> keys_queue;
    // if (isEmpty() || lo.compareTo(hi) > 0) return queue;
    keys(root_, keys_queue, lo, hi);
    return keys_queue;
} 

// add the keys between lo and hi in the subtree rooted at x
// to the queue
template <typename Key, typename Value>
void RedBlackBST<Key, Value>::keys(typename RedBlackBST<Key, Value>::Node* x,
                                   queue<Key>& keys_queue, Key lo, Key hi) const {
    if (!x) return; 
    bool cmplo = isLess(lo, x->key_); 
    bool cmphi = isLess(hi, x->key_); 
    if (cmplo) keys(x->left_, keys_queue, lo, hi); 
    if ((cmplo || !isLess(x->key_, lo)) && !cmphi) keys_queue.push(x->key_); 
    if (isLess(x->key_, hi)) keys(x->right_, keys_queue, lo, hi); 
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
template <typename Key, typename Value>
int RedBlackBST<Key, Value>::size(Key lo, Key hi) const {
    if (lo == defaultValue<Key>())
        throw std::invalid_argument("first argument to size() is null");
    if (hi == defaultValue<Key>()) 
        throw std::invalid_argument("second argument to size() is null");

    if (isLess(hi, lo)) return 0;
    if (contains(hi)) return rank(hi) - rank(lo) + 1;
    else return rank(hi) - rank(lo);
}


/***************************************************************************
*  Check integrity of red-black tree data structure.
***************************************************************************/
template <typename Key, typename Value>
bool RedBlackBST<Key, Value>::check() const {
    if (!isBST())            printf("Not in symmetric order\n");
    if (!isSizeConsistent()) printf("Subtree counts not consistent\n");
    if (!isRankConsistent()) printf("Ranks not consistent\n");
    if (!is23())             printf("Not a 2-3 tree\n");
    if (!isBalanced())       printf("Not balanced\n");
    return isBST() && isSizeConsistent() && isRankConsistent() && is23() && isBalanced();
}

// is the tree rooted at x a BST with all keys strictly between min and max
// (if min or max is null, treat as empty constraint)
// Credit: Bob Dondero's elegant solution
template <typename Key, typename Value>
bool RedBlackBST<Key, Value>::isBST(
    typename RedBlackBST<Key, Value>::Node* x, Key min, Key max) const {
    if (!x) return true;
    if (min != defaultValue<Key>() && !isLess(min, x->key_)) return false;
    if (max != defaultValue<Key>() && !isLess(x->key_, max)) return false;
    return isBST(x->left_, min, x->key_) && isBST(x->right_, x->key_, max);
} 

template <typename Key, typename Value>
bool RedBlackBST<Key, Value>::isSizeConsistent(
    typename RedBlackBST<Key, Value>::Node* x) const {
    if (!x) return true;
    if (x->size_ != size(x->left_) + size(x->right_) + 1) return false;
    return isSizeConsistent(x->left_) && isSizeConsistent(x->right_);
} 

// check that ranks are consistent
template <typename Key, typename Value>
bool RedBlackBST<Key, Value>::isRankConsistent() const {
    for (int i = 0; i < size(); i++)
        if (i != rank(select(i))) return false;
    for (Key key : keys())
        if (isLess(key, select(rank(key))) || isLess(select(rank(key)), key))
            return false;

    return true;
}

// Does the tree have no red right links, and at most one (left)
// red links in a row on any path?
template <typename Key, typename Value>
bool RedBlackBST<Key, Value>::is23(typename RedBlackBST<Key, Value>::Node* x) const {
    if (!x) return true;
    if (isRed(x->right_)) return false;
    if (x != root_ && isRed(x) && isRed(x->left_)) return false;
    return is23(x->left_) && is23(x->right_);
} 

// do all paths from root to leaf have same number of black edges?
template <typename Key, typename Value>
bool RedBlackBST<Key, Value>::isBalanced() const {
    int black = 0;     // number of black links on path from root to min
    Node* x = root_;
    while (x) {
        if (!isRed(x)) black++;
        x = x->left_;
    }

    return isBalanced(root_, black);
}

// does every path from the root to a leaf have the given number of black links?
template <typename Key, typename Value>
bool RedBlackBST<Key, Value>::isBalanced(
    typename RedBlackBST<Key, Value>::Node* x, int black) const {
    if (!x) return black == 0;
    if (!isRed(x)) black--;
    return isBalanced(x->left_, black) && isBalanced(x->right_, black);
} 


/**
 * Unit tests the {@code RedBlackBST} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug

#include <fstream>
#include <sstream>

using std::fstream;

int main(int argc, char *argv[]) { 
    fstream in(argv[1]);
    if (!in.is_open()) {
        std::cout << "failed to open " << argv[1] << '\n';
        return 1;
    }

    string line;
    int count = 0;

    RedBlackBST<string, int> st;
    while (!in.eof()) {
        std::getline(in, line);
        if (line == "") break;
        std::stringstream ss(line);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> vstrings(begin, end);

        for (const auto& str: vstrings) st.put(str, count++);
    }

    queue<string> keys = st.keys();
    while (!keys.empty()) {
        printf("%s %d\n", keys.front().c_str(), st.get(keys.front()));
        keys.pop();
    }
    printf("\n");

    return 0;
}
#endif

/******************************************************************************
 *  Copyright 2002-2020, Robert Sedgewick and Kevin Wayne.
 *
 *  This file is part of algs4.jar, which accompanies the textbook
 *
 *      Algorithms, 4th edition by Robert Sedgewick and Kevin Wayne,
 *      Addison-Wesley Professional, 2011, ISBN 0-321-57351-X.
 *      http://algs4.cs.princeton.edu
 *
 *
 *  algs4.jar is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  algs4.jar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with algs4.jar.  If not, see http://www.gnu.org/licenses.
 ******************************************************************************/
