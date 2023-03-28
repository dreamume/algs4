/******************************************************************************
 *  Compilation:  clang++ -c -O2 digraph.cc -std=c++20
 *                clang++ -DDebug -O2 directed_dfs.cc digraph.o -std=c++20 -o directed_dfs
 *  Execution:    ./directed_dfs digraph.txt s
 *  Dependencies: digraph.cc
 *  Data files:   https://algs4.cs.princeton.edu/42digraph/tinyDG.txt
 *                https://algs4.cs.princeton.edu/42digraph/mediumDG.txt
 *                https://algs4.cs.princeton.edu/42digraph/largeDG.txt
 *
 *  Determine single-source or multiple-source reachability in a digraph
 *  using depth first search.
 *  Runs in O(E + V) time.
 *
 *  % ./directed_dfs tinyDG.txt 1
 *  1
 *
 *  % ./directed_dfs tinyDG.txt 2
 *  0 1 2 3 4 5
 *
 *  % ./directed_dfs tinyDG.txt 1 2 6
 *  0 1 2 3 4 5 6 8 9 10 11 12
 *
 ******************************************************************************/
/**
 *  The {@code DirectedDFS} class represents a data type for
 *  determining the vertices reachable from a given source vertex <em>s</em>
 *  (or set of source vertices) in a digraph. For versions that find the paths,
 *  see {@link DepthFirstDirectedPaths} and {@link BreadthFirstDirectedPaths}.
 *  <p>
 *  This implementation uses depth-first search.
 *  The constructor takes time proportional to <em>V</em> + <em>E</em>
 *  (in the worst case),
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Each instance method takes &Theta;(1) time.
 *  It uses &Theta;(<em>V</em>) extra space (not including the digraph).
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/42digraph">Section 4.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#include "directed_dfs.h"

using std::vector;

DirectedDFS::DirectedDFS(const Digraph& G, int s) noexcept : marked_(G.V()) {
  Dfs(G, s);
}

DirectedDFS::DirectedDFS(const Digraph& G, const vector<int>& sources) noexcept:
  marked_(G.V()) {
  //        ValidateVertices(sources);
  for (int v : sources) {
    if (!marked_[v]) Dfs(G, v);
  }
}

void DirectedDFS::Dfs(const Digraph& G, int v) {
  count_++;
  marked_[v] = true;
  for (int w : G.Adj(v)) {
    if (!marked_[w]) Dfs(G, w);
  }
}

void DirectedDFS::ValidateVertex(int v) const {
  int V = marked_.size();
  if (v < 0 || v >= V)
    throw std::invalid_argument("vertex " + std::to_string(v) + 
                                " is not between 0 and " + std::to_string(V-1));
}

void DirectedDFS::ValidateVertices(const vector<int>& vertices) const {
  int vertexCount = 0;
  for (const auto& v : vertices) {
    vertexCount++;
    ValidateVertex(v);
  }
  if (vertexCount == 0)
    throw std::invalid_argument("zero vertices");
}

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

  // read in sources from command-line arguments
  vector<int> sources;
  for (int i = 2; i < argc; ++i)
    sources.push_back(strtol(argv[i], nullptr, 10));

  // multiple-source reachability
  DirectedDFS dfs(G, sources);

  // print out vertices reachable from sources
  for (int v = 0; v < G.V(); v++) {
    if (dfs.Marked(v)) std::cout << v << " ";
  }
  std::cout << std::endl;

  return 0;
}
#endif
