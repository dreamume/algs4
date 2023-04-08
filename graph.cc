/******************************************************************************
 *  Compilation:  clang++ -g -DDebug graph.cc -std=c++11 -o graph
 *  Execution:    ./graph
 *  Dependencies: 
 *  Data files:   https://algs4.cs.princeton.edu/41graph/tinyG.txt
 *                https://algs4.cs.princeton.edu/41graph/mediumG.txt
 *                https://algs4.cs.princeton.edu/41graph/largeG.txt
 *
 *  A graph, implemented using an array of sets.
 *  Parallel edges and self-loops allowed.
 *
 *  % java Graph tinyG.txt
 *  13 vertices, 13 edges 
 *  0: 6 2 1 5 
 *  1: 0 
 *  2: 0 
 *  3: 5 4 
 *  4: 5 6 3 
 *  5: 3 4 0 
 *  6: 0 4 
 *  7: 8 
 *  8: 7 
 *  9: 11 10 12 
 *  10: 9 
 *  11: 9 12 
 *  12: 11 9 
 *
 *  % java Graph mediumG.txt
 *  250 vertices, 1273 edges 
 *  0: 225 222 211 209 204 202 191 176 163 160 149 114 97 80 68 59 58 49 44 24 15 
 *  1: 220 203 200 194 189 164 150 130 107 72 
 *  2: 141 110 108 86 79 51 42 18 14 
 *  ...
 *  
 ******************************************************************************/

/**
 *  The {@code Graph} class represents an undirected graph of vertices
 *  named 0 through <em>V</em> – 1.
 *  It supports the following two primary operations: add an edge to the graph,
 *  iterate over all of the vertices adjacent to a vertex. It also provides
 *  methods for returning the number of vertices <em>V</em> and the number
 *  of edges <em>E</em>. Parallel edges and self-loops are permitted.
 *  By convention, a self-loop <em>v</em>-<em>v</em> appears in the
 *  adjacency list of <em>v</em> twice and contributes two to the degree
 *  of <em>v</em>.
 *  <p>
 *  This implementation uses an adjacency-lists representation, which 
 *  is a vertex-indexed array of {@link Bag} objects.
 *  All operations take constant time (in the worst case) except
 *  iterating over the vertices adjacent to a given vertex, which takes
 *  time proportional to the number of such vertices.
 *  <p>
 *  For additional documentation, see <a href="https://algs4.cs.princeton.edu/41graph">Section 4.1</a>
 *  of <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#include "graph.h"

#include <exception>

#ifdef Debug
#include <iostream>
#endif

using std::vector;
using std::string;
using std::fstream;

namespace algs4 {
/**  
 * Initializes a graph from the specified input stream.
 * The format is the number of vertices <em>V</em>,
 * followed by the number of edges <em>E</em>,
 * followed by <em>E</em> pairs of vertices, with each entry separated by whitespace.
 *
 * @param  in the input stream
 * @throws IllegalArgumentException if the endpoints of any edge are not in prescribed range
 * @throws IllegalArgumentException if the number of vertices or edges is negative
 * @throws IllegalArgumentException if the input stream is in the wrong format
 */
Graph::Graph(fstream& in) {
  try {
    string line;
    std::getline(in, line);
    v_ = stoi(line);

    if (v_ < 0) 
      throw std::invalid_argument("number of vertices in a Graph must be nonnegative");
    adj_.resize(v_);
    std::getline(in, line);
    int E = stoi(line);
    if (E < 0) 
      throw std::invalid_argument("number of edges in a Graph must be nonnegative");
    for (int i = 0; i < E; i++) {
      std::getline(in, line);
      const char* str = line.c_str();
      char *end;
      int v = std::strtol(str, &end, 10);
      str = end;
      int w = std::strtol(str, &end, 10);
      ValidateVertex(v);
      ValidateVertex(w);
      AddEdge(v, w); 
    }
  }
  catch (...) {
    throw;
  }
}


/**
 * Initializes a new graph that is a deep copy of {@code G}.
 *
 * @param  G the graph to copy
 */
Graph::Graph(const Graph& G) noexcept : Graph(G.V()) {
  e_ = G.E();
  for (int i = 0; i < v_; ++i) adj_[i] = G.adj(i);
}

// throw an IllegalArgumentException unless {@code 0 <= v < V}
void Graph::ValidateVertex(int v) const {
  if (v < 0 || v >= v_)
    throw std::invalid_argument("vertex " + std::to_string(v) + 
                                " is not between 0 and " + std::to_string(v_-1));
}

/**
 * Adds the undirected edge v-w to this graph.
 *
 * @param  v one vertex in the edge
 * @param  w the other vertex in the edge
 * @throws IllegalArgumentException unless both {@code 0 <= v < V} and {@code 0 <= w < V}
 */
void Graph::AddEdge(int v, int w) {
  ValidateVertex(v);
  ValidateVertex(w);
  e_++;
  adj_[v].push_back(w);
  adj_[w].push_back(v);
}

/**
 * Returns the vertices adjacent to vertex {@code v}.
 *
 * @param  v the vertex
 * @return the vertices adjacent to vertex {@code v}, as an iterable
 * @throws IllegalArgumentException unless {@code 0 <= v < V}
 */
const vector<int>& Graph::adj(int v) const {
  ValidateVertex(v);
  return adj_[v];
}

/**
 * Returns the degree of vertex {@code v}.
 *
 * @param  v the vertex
 * @return the degree of vertex {@code v}
 * @throws IllegalArgumentException unless {@code 0 <= v < V}
 */
int Graph::Degree(int v) const {
  ValidateVertex(v);
  return adj_[v].size();
}

/**
 * Returns a string representation of this graph.
 *
 * @return the number of vertices <em>V</em>, followed by the number of edges <em>E</em>,
 *         followed by the <em>V</em> adjacency lists
 */
string Graph::ToString() const {
  string s(std::to_string(v_) + " vertices, " + std::to_string(e_) + " edges \n");
  for (int v = 0; v < v_; v++) {
    s += std::to_string(v) + ": ";
    for (int w : adj_[v]) s += std::to_string(w) + " ";
    s += "\n";
  }

  return s;
}
}

/**
 * Unit tests the {@code Graph} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
using namespace algs4;
int main(int args, char* argv[]) {
  fstream in(argv[1]);
  if (!in.is_open()) {
    std::cout << "failed to open " << argv[1] << '\n';
    return 1;
  }

  Graph G(in);
  printf("%s\n", G.ToString().c_str());

  return 0;
}
#endif
