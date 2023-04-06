/******************************************************************************
 *  Compilation:  clang++ -c -O2 digraph.cc -std=c++20
 *                clang++ -DDebug -O2 directed_cycle.cc digraph.o -std=c++20 -o directed_cycle
 *  Execution:    ./directed_cycle input.txt
 *  Dependencies: digraph.cc
 *  Data files:   https://algs4.cs.princeton.edu/42digraph/tinyDG.txt
 *                https://algs4.cs.princeton.edu/42digraph/tinyDAG.txt
 *
 *  Finds a directed cycle in a digraph.
 *  Runs in O(E + V) time.
 *
 *  % java DirectedCycle tinyDG.txt 
 *  Directed cycle: 3 5 4 3 
 *
 *  %  java DirectedCycle tinyDAG.txt 
 *  No directed cycle
 *
 ******************************************************************************/

/**
 *  The {@code DirectedCycle} class represents a data type for 
 *  determining whether a digraph has a directed cycle.
 *  The <em>hasCycle</em> operation determines whether the digraph has
 *  a simple directed cycle and, if so, the <em>cycle</em> operation
 *  returns one.
 *  <p>
 *  This implementation uses depth-first search.
 *  The constructor takes time proportional to <em>V</em> + <em>E</em>
 *  (in the worst case),
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the <em>hasCycle</em> operation takes constant time;
 *  the <em>cycle</em> operation takes time proportional
 *  to the length of the cycle.
 *  <p>
 *  See {@link Topological} to compute a topological order if the
 *  digraph is acyclic.
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/42digraph">Section 4.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#include "directed_cycle.h"

#include <cassert>
#include <cstdio>

using std::vector;
using std::stack;

namespace algs4 {
/**
 * Determines whether the digraph {@code G} has a directed cycle and, if so,
 * finds such a cycle.
 * @param G the digraph
 */
DirectedCycle::DirectedCycle(const Digraph& G) noexcept {
  marked_.resize(G.V());
  edge_to_.resize(G.V());
  on_stack_.resize(G.V());
  for (int v = 0; v < G.V(); v++)
    if (!marked_[v] && cycle_.empty()) Dfs(G, v);
}

// check that algorithm computes either the topological order or finds a directed cycle
void DirectedCycle::Dfs(const Digraph& G, int v) {
  on_stack_[v] = true;
  marked_[v] = true;
  for_each(G.Adj(v).begin(), G.Adj(v).end(), [&](int w) {

    // short circuit if directed cycle found
    if (!cycle_.empty()) return;

    // found new vertex, so recur
    else if (!marked_[w]) {
      edge_to_[w] = v;
      Dfs(G, w);
    }

    // trace back directed cycle
    else if (on_stack_[w]) {
      for (int x = v; x != w; x = edge_to_[x]) {
        cycle_.push(x);
      }
      cycle_.push(w);
      cycle_.push(v);
      assert(Check());
    }
  });
  on_stack_[v] = false;
}

// certify that digraph has a directed cycle if it reports one
bool DirectedCycle::Check() const {
  if (HasCycle()) {
    // verify cycle
    int first = -1, last = -1;
    stack<int> thecycle_ = cycle();
    while (!thecycle_.empty()) {
      if (first == -1) first = thecycle_.top();
      last = thecycle_.top();
      thecycle_.pop();
    }
    if (first != last) {
      printf("cycle begins with %d and ends with %d\n", first, last);
      return false;
    }
  }

  return true;
}
}

/**
 * Unit tests the {@code DirectedCycle} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <iostream>
#include <fstream>
using namespace algs4;
using std::fstream;
int main(int args, char *argv[]) {
  fstream in(argv[1]);
  if (!in.is_open()) {
    std::cout << "failed to open " << argv[1] << '\n';
    return 1;
  }

  Digraph G(in);
  DirectedCycle finder(G);
  if (finder.HasCycle()) {
    printf("Directed cycle: ");
    stack<int> thecycle_ = finder.cycle();
    while (!thecycle_.empty()) {
      printf("%d ", thecycle_.top());
      thecycle_.pop();
    }
    printf("\n");
  } else {
    printf("No directed cycle\n");
  }
  printf("\n");
}
#endif

/******************************************************************************
 *  Copyright 2002-2018, Robert Sedgewick and Kevin Wayne.
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
