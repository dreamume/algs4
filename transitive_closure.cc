/******************************************************************************
 *  Compilation:  clang++ -c -O2 digraph.cc -std=c++20
 *                clang++ -c -O2 directed_dfs.cc -std=c++20
 *                clang++ -DDebug -O2 transitive_closure.cc directed_dfs.o digraph.o -std=c++20 -o transitive_closure
 *  Execution:    ./transitive_closure filename.txt
 *  Dependencies: digraph.cc directed_dfs.cc
 *  Data files:   https://algs4.cs.princeton.edu/42digraph/tinyDG.txt
 *
 *  Compute transitive closure of a digraph and support
 *  reachability queries.
 *
 *  Preprocessing time: O(V(E + V)) time.
 *  Query time: O(1).
 *  Space: O(V^2).
 *
 *  % ./transitive_closure tinyDG.txt
 *         0  1  2  3  4  5  6  7  8  9 10 11 12
 *  --------------------------------------------
 *    0:   T  T  T  T  T  T
 *    1:      T
 *    2:   T  T  T  T  T  T
 *    3:   T  T  T  T  T  T
 *    4:   T  T  T  T  T  T
 *    5:   T  T  T  T  T  T
 *    6:   T  T  T  T  T  T  T        T  T  T  T
 *    7:   T  T  T  T  T  T  T  T  T  T  T  T  T
 *    8:   T  T  T  T  T  T  T  T  T  T  T  T  T
 *    9:   T  T  T  T  T  T           T  T  T  T
 *   10:   T  T  T  T  T  T           T  T  T  T
 *   11:   T  T  T  T  T  T           T  T  T  T
 *   12:   T  T  T  T  T  T           T  T  T  T
 *
 ******************************************************************************/

/**
 *  The {@code TransitiveClosure} class represents a data type for
 *  computing the transitive closure of a digraph.
 *  <p>
 *  This implementation runs depth-first search from each vertex.
 *  The constructor takes &Theta;(<em>V</em>(<em>V</em> + <em>E</em>))
 *  in the worst case, where <em>V</em> is the number of vertices and
 *  <em>E</em> is the number of edges.
 *  Each instance method takes &Theta;(1) time.
 *  It uses &Theta;(<em>V</em><sup>2</sup>) extra space (not including the digraph).
 *  <p>
 *  For large digraphs, you may want to consider a more sophisticated algorithm.
 *  <a href = "http://www.cs.hut.fi/~enu/thesis.html">Nuutila</a> proposes two
 *  algorithm for the problem (based on strong components and an interval representation)
 *  that runs in &Theta;(<em>E</em> + <em>V</em>) time on typical digraphs.
 *
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/42digraph">Section 4.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#include "transitive_closure.h"

#include <exception>
#include <string>

using std::vector;
using std::to_string;

TransitiveClosure::TransitiveClosure(const Digraph& G) : tc_(G.V()) {
  for (int v = 0; v < G.V(); v++)
    tc_[v] = new DirectedDFS(G, v);
}

bool TransitiveClosure::Reachable(int v, int w) {
  ValidateVertex(v);
  ValidateVertex(w);
  return tc_[v]->Marked(w);
}

void TransitiveClosure::ValidateVertex(int v) {
  int V = tc_.size();
  if (v < 0 || v >= V)
    throw std::invalid_argument("vertex " + to_string(v) + " is not between 0 and " + to_string(V-1));
}

/**
 * Unit tests the {@code TransitiveClosure} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <iostream>
#include <cstdlib>
int main(int argc, char *argv[]) {
  std::fstream in(argv[1]);
  if (!in.is_open()) {
    std::cout << "failed to open " << argv[1] << '\n';
    return 1;
  }

  Digraph G(in);

  TransitiveClosure tc(G);

  // print header
  std::cout << "     ";
  for (int v = 0; v < G.V(); v++)
    std::cout << v;
  std::cout << std::endl;
  std::cout << "--------------------------------------------" << std::endl;

  // print transitive closure
  for (int v = 0; v < G.V(); v++) {
    std::cout << v;
    for (int w = 0; w < G.V(); w++) {
      if (tc.Reachable(v, w)) std::cout << "  T";
      else                    std::cout << "   ";
    }
    std::cout << std::endl;
  }

  return 0;
}
#endif
