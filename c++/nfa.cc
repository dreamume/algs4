/******************************************************************************
 *  Compilation:  clang++ -c -O2 digraph.cc -std=c++20
 *                clang++ -c -O2 directed_dfs.cc -std=c++20
 *                clang++ -DDebug -O2 nfa.cc digraph.o directed_dfs.o -std=c++20 -o nfa
 *  Execution:    ./nfa regexp text
 *  Dependencies: digraph.h directed_dfs.h
 *
 *  % ./nfa "(A*B|AC)D" AAAABD
 *  true
 *
 *  % ./nfa "(A*B|AC)D" AAAAC
 *  false
 *
 *  % ./nfa "(a|(bc)*d)*" abcbcd
 *  true
 *
 *  % ./nfa "(a|(bc)*d)*" abcbcbcdaaaabcbcdaaaddd
 *  true
 *
 *  Remarks
 *  -----------
 *  The following features are not supported:
 *    - The + operator
 *    - Multiway or
 *    - Metacharacters in the text
 *    - Character classes.
 *
 ******************************************************************************/

#include "nfa.h"

#include <cassert>
#include <list>
#include <vector>
#include <stack>
#include <exception>

#include "directed_dfs.h"

using std::string;
using std::vector;
using std::stack;
using std::list;

namespace algs4 {
NFA::NFA(const string& regexp) noexcept : graph_(regexp.size() + 1), 
                                          regexp_(regexp), m_(regexp.size()) {
  stack<int> ops;
  for (int i = 0; i < m_; i++) {
    int lp = i;
    if (regexp[i] == '(' || regexp[i] == '|')
      ops.push(i);
    else if (regexp[i] == ')') {
      int or_index = ops.top();
      ops.pop();

      // 2-way or operator
      if (regexp[or_index] == '|') {
        lp = ops.top();
        ops.pop();
        graph_.AddEdge(lp, or_index+1);
        graph_.AddEdge(or_index, i);
      }
      else if (regexp[or_index] == '(')
        lp = or_index;
      else assert(false);
    }

    // closure operator (uses 1-character lookahead)
    if (i < m_-1 && regexp[i+1] == '*') {
      graph_.AddEdge(lp, i+1);
      graph_.AddEdge(i+1, lp);
    }
    if (regexp[i] == '(' || regexp[i] == '*' || regexp[i] == ')')
      graph_.AddEdge(i, i+1);
  }
  if (ops.size() != 0)
    assert("Invalid regular expression");
}

bool NFA::Recognizes(const string& txt) const {
  DirectedDFS dfs(graph_, 0);
  list<int> pc;
  for (int v = 0; v < graph_.V(); v++)
    if (dfs.Marked(v)) pc.push_back(v);

  // Compute possible NFA states for txt[i+1]
  for (int i = 0; i < txt.length(); i++) {
    if (txt[i] == '*' || txt[i] == '|' || txt[i] == '(' || txt[i] == ')')
      throw std::invalid_argument("text contains the metacharacter '" + string(1, txt[i]) + "'");

    vector<int> match;
    for (int v : pc) {
      if (v == m_) continue;
      if ((regexp_[v] == txt[i]) || regexp_[v] == '.')
        match.push_back(v+1);
    }
    if (match.empty()) continue;

    dfs = DirectedDFS(graph_, match);
    pc = list<int>();
    for (int v = 0; v < graph_.V(); v++)
      if (dfs.Marked(v)) pc.push_back(v);

    // optimization if no states reachable
    if (pc.size() == 0) return false;
  }

  // check for accept state
  for (int v : pc)
    if (v == m_) return true;
  return false;
}
}


/**
 * Unit tests the {@code NFA} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
using namespace algs4;
#include <iostream>
using std::cout;
using std::endl;
int main(int argc, char *argv[]) {
  string regexp = "(" + string(argv[1]) + ")";
  string txt = argv[2];
  NFA nfa(regexp);
  cout << nfa.Recognizes(txt) << endl;

  return 0;
}
#endif
