#ifndef HUFFMAN_H_
#define HUFFMAN_H_

#include <compare>
#include <vector>
#include <string>

namespace algs4 {
/**
 *  The {@code Huffman} class provides static methods for compressing
 *  and expanding a binary input using Huffman codes over the 8-bit extended
 *  ASCII alphabet.
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/55compression">Section 5.5</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
class Huffman {
public:
  // alphabet size of extended ASCII
  static constexpr int R = 256;
private:
  // Do not instantiate.
  //Huffman() { }

  // Huffman trie node
  class Node {
  public:
    char ch_;
    int freq_;
    Node* left_{nullptr};
    Node* right_{nullptr};

    Node(char ch, int freq, Node* left, Node* right) noexcept :
      ch_(ch), freq_(freq), left_(left), right_(right) {}
    Node() = delete;
    Node(const Node& other) = delete;
    Node &operator=(const Node& other) = delete;
    Node(Node&& other) = default;
    Node &operator=(Node&& other) = default;

    [[nodiscard]] std::strong_ordering operator<=>(const Node& other) const;
    // is the node a leaf node?
    bool IsLeaf() const;
  };

public:
  /**
   * Reads a sequence of 8-bit bytes from standard input; compresses them
   * using Huffman codes with an 8-bit alphabet; and writes the results
   * to standard output.
   */
  static void compress();

  /**
   * Reads a sequence of bits that represents a Huffman-compressed message from
   * standard input; expands them; and writes the results to standard output.
   */
  static void expand();
private:
  // build the Huffman trie given frequencies
  static Node* buildTrie(const std::vector<int>& freq);

  // write bitstring-encoded trie to standard output
  static void writeTrie(Node* x);

  // make a lookup table from symbols and their encodings
  static void buildCode(std::vector<std::string>& st, Node* x, std::string s);

  static Node* readTrie();
};
}

#endif  // HUFFMAN_H_
