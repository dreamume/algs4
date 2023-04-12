/******************************************************************************
 *  Compilation:  clang++ -DDebug -O2 huffman.cc -std=c++20 -o huffman
 *  Execution:    ./huffman - < input.txt   (compress)
 *  Execution:    ./huffman + < input.txt   (expand)
 *  Dependencies: 
 *  Data files:   https://algs4.cs.princeton.edu/55compression/abra.txt
 *                https://algs4.cs.princeton.edu/55compression/tinytinyTale.txt
 *                https://algs4.cs.princeton.edu/55compression/medTale.txt
 *                https://algs4.cs.princeton.edu/55compression/tale.txt
 *
 *  Compress or expand a binary input stream using the Huffman algorithm.
 *
 *  % ./huffman - < abra.txt | ./binaryDump 60
 *  010100000100101000100010010000110100001101010100101010000100
 *  000000000000000000000000000110001111100101101000111110010100
 *  120 bits
 *
 *  % ./huffman - < abra.txt | ./huffman +
 *  ABRACADABRA!
 *
 ******************************************************************************/

#include "huffman.h"

#include <cassert>
#include <exception>
#include <iostream>
#include <queue>

using std::string;
using std::vector;
using std::priority_queue;
using std::cin;
using std::cout;
using std::endl;
using std::getline;

namespace algs4 {

// is the node a leaf node?
bool Huffman::Node::IsLeaf() const {
  assert((!left_ && !right_) || (left_ && right_));
  return (!left_) && (!right_);
}

[[nodiscard]] std::strong_ordering Huffman::Node::operator<=>(const Huffman::Node& other) const {
  //  return std::strong_order(weight_, other.weight());
  //return compare_strong_order_fallback(weight_, other.weight());
  auto cmp2 = freq_ <=> other.freq_; // might be partial_ordering for double
// map partial_ordering to strong_ordering:
//  assert(cmp2 != std::partial_ordering::unordered); // RUNTIME ERROR if unordered 
  return cmp2 == 0 ? std::strong_ordering::equal
    : cmp2 > 0 ? std::strong_ordering::greater
    : std::strong_ordering::less;
}

void Huffman::compress() {
  // read the input
  string line;
  getline(cin, line);

  // tabulate frequency counts
  vector<int> freq_(R);
  for (int i = 0; i < line.size(); ++i)
    ++freq_[line[i]];

  // build Huffman trie
  Huffman::Node* root = buildTrie(freq_);

  // build code table
  vector<string> st(R);
  buildCode(st, root, "");

  // print trie for decoder
  writeTrie(root);

  // print number of bytes in original uncompressed message
  cout << 'l';
  cout << (int)line.size();
  cout << 'l';

  // use Huffman code to encode input
  for (int i = 0; i < line.size(); ++i) {
    string code = st[line[i]];
    for (int j = 0; j < code.size(); ++j) {
      if (code[j] == '0' || code[j] == '1') {
        cout << code[j];
      } else {
        throw std::domain_error("Illegal state");
      }
    }
  }

  // close output stream
  cout << endl;
}

Huffman::Node* Huffman::buildTrie(const vector<int>& freq) {
  // initialize priority queue with singleton trees
  auto cmp = [](const Huffman::Node* left, const Huffman::Node* right) {
    return *left > *right;
  };
  priority_queue<Huffman::Node*, vector<Huffman::Node *>, decltype(cmp)> pq(cmp);
  for (int c = 0; c < R; ++c)
    if (freq[c] > 0)
      pq.push(new Huffman::Node(c, freq[c], nullptr, nullptr));

  // merge two smallest trees
  while (pq.size() > 1) {
    Huffman::Node* left  = pq.top();
    pq.pop();
    Huffman::Node* right = pq.top();
    pq.pop();
    Huffman::Node* parent = 
      new Huffman::Node('\0', left->freq_ + right->freq_, left, right);
    pq.push(parent);
  }

  return pq.top();
}

void Huffman::buildCode(vector<string>& st, Huffman::Node* x, string s) {
  if (!x->IsLeaf()) {
    buildCode(st, x->left_,  s + '0');
    buildCode(st, x->right_, s + '1');
  } else {
    st[x->ch_] = s;
  }
}

void Huffman::writeTrie(Huffman::Node* x) {
  if (x->IsLeaf()) {
    cout << true;
    cout << x->ch_;
    return;
  }

  cout << false;
  writeTrie(x->left_);
  writeTrie(x->right_);
}

void Huffman::expand() {
  // read in Huffman trie from input stream
  Huffman::Node* root = readTrie();

  // number of bytes to write
  int length{0};
  char begin;
  cin >> begin;
  if (begin != 'l') throw std::domain_error("invalid input data");
  while (true) {
    cin >> begin;
    if (begin == 'l') break;
    length = length * 10 + begin - '0';
  }

  // decode using the Huffman trie
  for (int i = 0; i < length; ++i) {
    Huffman::Node* x = root;
    while (!x->IsLeaf()) {
      char bit;
      cin >> bit;
      if (bit == '1') x = x->right_;
      else     x = x->left_;
    }
    cout << x->ch_;
  }
  cout << endl;
}

Huffman::Node* Huffman::readTrie() {
  char isLeaf;
  cin >> isLeaf;
  if (isLeaf == '1') {
    char ch;
    cin >> ch;
    return new Huffman::Node(ch, -1, nullptr, nullptr);
  } else {
    return new Huffman::Node('\0', -1, readTrie(), readTrie());
  }
}
}

/**
 * Sample client that calls {@code compress()} if the command-line
 * argument is "-" an {@code expand()} if it is "+".
 *
 * @param args the command-line arguments
 */
#ifdef Debug
using namespace algs4;
int main(int argc, char *argv[]) {
  if      (argv[1][0] == '-') Huffman::compress();
  else if (argv[1][0] == '+') Huffman::expand();
  else throw std::invalid_argument("Illegal command line argument");

  return 0;
}
#endif
