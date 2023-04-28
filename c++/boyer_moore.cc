/******************************************************************************
 *  Compilation:  clang++ -DDebug -O2 boyer_moore.cc -std=c++20 -o boyer_moore
 *  Execution:    ./boyer_moore pattern text
 *  Dependencies: 
 *
 *  Reads in two strings, the pattern and the input text, and
 *  searches for the pattern in the input text using the
 *  bad-character rule part of the Boyer-Moore algorithm.
 *  (does not implement the strong good suffix rule)
 *
 *  % ./boyer_moore abracadabra abacadabrabracabracadabrabrabracad
 *  text:    abacadabrabracabracadabrabrabracad
 *  pattern:               abracadabra
 *
 *  % ./boyer_moore rab abacadabrabracabracadabrabrabracad
 *  text:    abacadabrabracabracadabrabrabracad
 *  pattern:         rab
 *
 *  % ./boyer_moore bcara abacadabrabracabracadabrabrabracad
 *  text:    abacadabrabracabracadabrabrabracad
 *  pattern:                                   bcara
 *
 *  % ./boyer_moore rabrabracad abacadabrabracabracadabrabrabracad
 *  text:    abacadabrabracabracadabrabrabracad
 *  pattern:                        rabrabracad
 *
 *  % ./boyer_moore abacad abacadabrabracabracadabrabrabracad
 *  text:    abacadabrabracabracadabrabrabracad
 *  pattern: abacad
 *
 ******************************************************************************/

#include "boyer_moore.h"

#include <algorithm>

using std::vector;
using std::max;
using std::string;

namespace algs4 {
BoyerMoore::BoyerMoore(const string& pat) noexcept : 
  right_(256, -1), pattern_(pat) {
  // position of rightmost occurrence of c in the pattern
  for (int i = 0; i < pattern_.size(); ++i)
    right_[pattern_[i]] = i;
}

int BoyerMoore::search(const string& txt) const {
  int m = pattern_.length();
  int n = txt.length();
  for (int i = 0, skip = 0; i <= n - m; i += skip) {
    skip = 0;
    for (int j = m-1; j >= 0; --j) {
      if (pattern_[j] != txt[i+j]) {
        skip = max(1, j - right_[txt[i+j]]);
        break;
      }
    }
    if (skip == 0) return i;    // found
  }

  return n;                       // not found
}
}

    /**
     * Takes a pattern string and an input string as command-line arguments;
     * searches for the pattern string in the text string; and prints
     * the first occurrence of the pattern string in the text string.
     *
     * @param args the command-line arguments
     */
#ifdef Debug
#include <iostream>
using namespace std;
using namespace algs4;
int main(int argc, char *argv[]) {
  string pat = argv[1];
  string txt = argv[2];

  BoyerMoore boyermoore(pat);
  int offset1 = boyermoore.search(txt);

  // print results
  cout << "text:    " << txt << endl;

  cout << "pattern: ";
  for (int i = 0; i < offset1; i++)
    cout << " ";
  cout << pat << endl;

  return 0;
}
#endif
