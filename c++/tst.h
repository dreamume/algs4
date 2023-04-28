#ifndef TST_H_
#define TST_H_

#include <string>
#include <queue>
#include <exception>
#include <optional>

namespace algs4 {

/**
 *  The {@code TST} class represents a symbol table of key-value
 *  pairs, with string keys and generic values.
 *  It supports the usual <em>put</em>, <em>get</em>, <em>contains</em>,
 *  <em>delete</em>, <em>size</em>, and <em>is-empty</em> methods.
 *  It also provides character-based methods for finding the string
 *  in the symbol table that is the <em>longest prefix</em> of a given prefix,
 *  finding all strings in the symbol table that <em>start with</em> a given prefix,
 *  and finding all strings in the symbol table that <em>match</em> a given pattern.
 *  A symbol table implements the <em>associative array</em> abstraction:
 *  when associating a value with a key that is already in the symbol table,
 *  the convention is to replace the old value with the new value.
 *  Unlike {@link java.util.Map}, this class uses the convention that
 *  values cannot be {@code null}—setting the
 *  value associated with a key to {@code null} is equivalent to deleting the key
 *  from the symbol table.
 *  <p>
 *  This implementation uses a ternary search trie.
 *  <p>
 *  For additional documentation, see <a href="https://algs4.cs.princeton.edu/52trie">Section 5.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 */
template <class Value>
class TST {
private:
  struct Node {
    char c_{};                        // character
    Node* left_{nullptr};                    // left subtree
    Node* mid_{nullptr};                     // middle subtree
    Node* right_{nullptr};                   // right subtree
    std::optional<Value> val_{};                     // value associated with string
  };

public:
  TST() = default;
  TST(const TST& other) = delete;
  TST &operator=(const TST& other) = delete;
  TST(TST&& other) = default;
  TST &operator=(TST&& other) = default;
  /**
   * Returns the number of key-value pairs in this symbol table.
   * @return the number of key-value pairs in this symbol table
   */
  int size() const {
    return n_;
  }

  /**
   * Does this symbol table contain the given key?
   * @param key the key
   * @return {@code true} if this symbol table contains {@code key} and
   *     {@code false} otherwise
   * @throws IllegalArgumentException if {@code key} is {@code null}
   */
  bool contains(const std::string& key) const {
    // if (key == null) {
    //     throw new IllegalArgumentException("argument to contains() is null");
    // }
    return get(key) != std::nullopt;
  }

  /**
   * Returns the value associated with the given key.
   * @param key the key
   * @return the value associated with the given key if the key is in the symbol table
   *     and {@code null} if the key is not in the symbol table
   * @throws IllegalArgumentException if {@code key} is {@code null}
   */
  std::optional<Value> get(const std::string& key) const {
    // if (key == null) {
    //     throw new IllegalArgumentException("calls get() with null argument");
    // }
    if (key.size() == 0) throw std::invalid_argument("key must have length >= 1");
    Node* x = get(root_, key, 0);
    if (!x) return std::nullopt;
    return x->val_;
  }

  /**
   * Inserts the key-value pair into the symbol table, overwriting the old value
   * with the new value if the key is already in the symbol table.
   * If the value is {@code null}, this effectively deletes the key from the symbol table.
   * @param key the key
   * @param val the value
   * @throws IllegalArgumentException if {@code key} is {@code null}
   */
  void put(const std::string& key, const std::optional<Value>& val) {
    // if (key == null) {
    //   throw new IllegalArgumentException("calls put() with null key");
    // }
    if (!contains(key)) n_++;
    else if (val == std::nullopt) --n_;       // delete existing key
    root_ = put(root_, key, val, 0);
  }

  /**
   * Returns the string in the symbol table that is the longest prefix of {@code query},
   * or {@code null}, if no such string.
   * @param query the query string
   * @return the string in the symbol table that is the longest prefix of {@code query},
   *     or {@code null} if no such string
   * @throws IllegalArgumentException if {@code query} is {@code null}
   */
  std::string longestPrefixOf(const std::string& query) {
    // if (query == null) {
    //     throw new IllegalArgumentException("calls longestPrefixOf() with null argument");
    // }
    if (query.size() == 0) return "";
    int length = 0;
    Node* x{root_};
    int i = 0;
    while (x && i < query.size()) {
      char c = query[i];
      if      (c < x->c_) x = x->left_;
      else if (c > x->c_) x = x->right_;
      else {
        i++;
        if (x->val_ != std::nullopt) length = i;
        x = x->mid_;
      }
    }

    return query.substr(0, length);
  }

  /**
   * Returns all keys in the symbol table as an {@code Iterable}.
   * To iterate over all of the keys in the symbol table named {@code st},
   * use the foreach notation: {@code for (Key key : st.keys())}.
   * @return all keys in the symbol table as an {@code Iterable}
   */
  std::queue<std::string> keys() const {
    std::queue<std::string> q;
    std::string prefix;
    collect(root_, prefix, q);

    return q;
  }

  /**
   * Returns all of the keys in the set that start with {@code prefix}.
   * @param prefix the prefix
   * @return all of the keys in the set that start with {@code prefix},
   *     as an iterable
   * @throws IllegalArgumentException if {@code prefix} is {@code null}
   */
  std::queue<std::string> keysWithPrefix(std::string prefix) const {
    // if (prefix == null) {
    //     throw new IllegalArgumentException("calls keysWithPrefix() with null argument");
    // }
    std::queue<std::string> q;
    Node* x = get(root_, prefix, 0);
    if (!x) return q;
    if (x->val_ != std::nullopt) q.push(prefix);
    collect(x->mid_, prefix, q);
    return q;
  }

  /**
   * Returns all of the keys in the symbol table that match {@code pattern},
   * where the character '.' is interpreted as a wildcard character.
   * @param pattern the pattern
   * @return all of the keys in the symbol table that match {@code pattern},
   *     as an iterable, where . is treated as a wildcard character.
   */
  std::queue<std::string> keysThatMatch(std::string pattern) const {
    std::queue<std::string> q;
    std::string prefix;
    collect(root_, prefix, 0, pattern, q);

    return q;
  }
private:
  // return subtrie corresponding to given key
  Node* get(Node* x, const std::string& key, int d) const {
    if (!x) return nullptr;
    if (key.size() == 0) throw std::invalid_argument("key must have length >= 1");

    char c = key[d];
    if      (c < x->c_)              return get(x->left_,  key, d);
    else if (c > x->c_)              return get(x->right_, key, d);
    else if (d < key.size() - 1) return get(x->mid_,   key, d+1);
    else                           return x;
  }

  Node* put(Node* x, const std::string& key, const std::optional<Value>& val, int d) {
    char c = key[d];
    if (!x) {
      x = new Node;
      x->c_ = c;
    }
    if      (c < x->c_)               x->left_  = put(x->left_,  key, val, d);
    else if (c > x->c_)               x->right_ = put(x->right_, key, val, d);
    else if (d < key.length() - 1)  x->mid_   = put(x->mid_,   key, val, d+1);
    else                            x->val_   = val;
    
    return x;
  }

  // all keys in subtrie rooted at x with given prefix
  void collect(Node* x, std::string& prefix, std::queue<std::string>& q) const {
    if (!x) return;
    collect(x->left_,  prefix, q);
    prefix += x->c_;
    if (x->val_ != std::nullopt) q.push(prefix);
    collect(x->mid_, prefix, q);
    prefix.pop_back();
    collect(x->right_, prefix, q);
  }

  void collect(Node* x, std::string& prefix, int i, std::string& pattern, std::queue<std::string>& q) const {
    if (!x) return;
    char c = pattern[i];
    if (c == '.' || c < x->c_) collect(x->left_, prefix, i, pattern, q);
    if (c == '.' || c == x->c_) {
      if (i == pattern.length() - 1 && x->val_ != std::nullopt) 
        q.push(prefix + x->c_);
      if (i < pattern.length() - 1) {
        prefix += x->c_;
        collect(x->mid_, prefix, i+1, pattern, q);
        prefix.pop_back();
      }
    }
    if (c == '.' || c > x->c_) collect(x->right_, prefix, i, pattern, q);
  }


private:
  int n_{0};              // size
  Node* root_{nullptr};   // root of TST
};
}

#endif  /* TST_H_ */
