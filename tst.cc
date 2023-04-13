/******************************************************************************
 *  Compilation:  clang++ -DDebug -O2 tst.cc -std=c++20 -o tst
 *  Execution:    cat words.txt | ./tst
 *  Dependencies: 
 *  Data files:   https://algs4.cs.princeton.edu/52trie/shellsST.txt
 *
 *  Symbol table with string keys, implemented using a ternary search
 *  trie (TST).
 *
 *
 *  % cat shellsST.txt | ./tst
 *  keys(""):
 *  by 4
 *  sea 6
 *  sells 1
 *  she 0
 *  shells 3
 *  shore 7
 *  the 5
 *
 *  longestPrefixOf("shellsort"):
 *  shells
 *
 *  keysWithPrefix("shor"):
 *  shore
 *
 *  keysThatMatch(".he.l."):
 *  shells
 *
 *  % ./tst
 *  theory the now is the time for all good men
 *
 *  Remarks
 *  --------
 *    - can't use a key that is the empty string ""
 *
 ******************************************************************************/

#include "tst.h"

#include <iostream>

using namespace std;

/**
 * Unit tests the {@code TST} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
using namespace algs4;
int main(int argc, char *argv[]) {
  // build symbol table from standard input
  TST<int> st;
  string key;
  int i{0};
  while (cin >> key)
    st.put(key, i++);

  // print results
  if (st.size() < 100) {
    cout << "keys(\"\"):" << endl;
    queue<string> keys = st.keys();
    while (!keys.empty()) {
      cout << keys.front() << " " << st.get(keys.front()).value() << endl;
      keys.pop();
    }

    cout << endl;
  }

  cout << "longestPrefixOf(\"shellsort\"):" << endl;
  cout << st.longestPrefixOf("shellsort") << endl;
  cout << endl;

  cout << "longestPrefixOf(\"shell\"):" << endl;
  cout << st.longestPrefixOf("shell") << endl;
  cout << endl;

  cout << "keysWithPrefix(\"shor\"):" << endl;
  queue<string> prefixKeys = st.keysWithPrefix("shor");
  while (!prefixKeys.empty()) {
    cout << prefixKeys.front() << endl;
    prefixKeys.pop();
  }
  cout << endl;

  cout << "keysThatMatch(\".he.l.\"):" << endl;
  queue<string> matches = st.keysThatMatch(".he.l.");
  while (!matches.empty()) {
    cout << matches.front() << endl;
    matches.pop();
  }

  return 0;
}
#endif
