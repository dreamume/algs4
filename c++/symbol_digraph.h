#ifndef SYMBO_DIGRAPH_H_
#define SYMBO_DIGRAPH_H_

/**
 *  The {@code SymbolDigraph} class represents a digraph, where the
 *  vertex names are arbitrary strings.
 *  By providing mappings between string vertex names and integers,
 *  it serves as a wrapper around the
 *  {@link Digraph} data type, which assumes the vertex names are integers
 *  between 0 and <em>V</em> - 1.
 *  It also supports initializing a symbol digraph from a file.
 *  <p>
 *  This implementation uses an {@link ST} to map from strings to integers,
 *  an array to map from integers to strings, and a {@link Digraph} to store
 *  the underlying graph.
 *  The <em>indexOf</em> and <em>contains</em> operations take time 
 *  proportional to log <em>V</em>, where <em>V</em> is the number of vertices.
 *  The <em>nameOf</em> operation takes constant time.
 *  <p>
 *  For additional documentation, see <a href="https://algs4.cs.princeton.edu/42digraph">Section 4.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#include <string>
#include <vector>
#include <map>

namespace algs4 {

class Digraph;

class SymbolDigraph {
public:
  /**  
   * Initializes a digraph from a file using the specified delimiter.
   * Each line in the file contains
   * the name of a vertex, followed by a list of the names
   * of the vertices adjacent to that vertex, separated by the delimiter.
   * @param filename the name of the file
   * @param delimiter the delimiter between fields
   */
  SymbolDigraph(const std::string& filename, const std::string& delimiter) noexcept;
  SymbolDigraph() = delete;
  SymbolDigraph(const SymbolDigraph& other) = delete;
  SymbolDigraph &operator=(const SymbolDigraph& other) = delete;
  SymbolDigraph(SymbolDigraph&& other) = delete;
  SymbolDigraph &operator=(SymbolDigraph&& other) = delete;
  /**
   * Does the digraph contain the vertex named {@code s}?
   * @param s the name of a vertex
   * @return {@code true} if {@code s} is the name of a vertex, and {@code false} otherwise
   */
  bool Contains(const std::string& s) const { return vertex_.find(s) != vertex_.end(); }
  /**
   * Returns the integer associated with the vertex named {@code s}.
   * @param s the name of a vertex
   * @return the integer (between 0 and <em>V</em> - 1) associated with the vertex named {@code s}
   * @deprecated Replaced by {@link #indexOf(String)}.
   */
  int Index(const std::string& s) { return vertex_[s]; }
  /**
   * Returns the integer associated with the vertex named {@code s}.
   * @param s the name of a vertex
   * @return the integer (between 0 and <em>V</em> - 1) associated with the vertex named {@code s}
   */
  int IndexOf(const std::string& s) { return vertex_[s]; }
  /**
   * Returns the name of the vertex associated with the integer {@code v}.
   * @param  v the integer corresponding to a vertex (between 0 and <em>V</em> - 1) 
   * @return the name of the vertex associated with the integer {@code v}
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   * @deprecated Replaced by {@link #nameOf(int)}.
   */
  std::string Name(int v) const { 
    ValidateVertex(v);
    return keys_[v];
  }
  /**
   * Returns the name of the vertex associated with the integer {@code v}.
   * @param  v the integer corresponding to a vertex (between 0 and <em>V</em> - 1) 
   * @return the name of the vertex associated with the integer {@code v}
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  std::string NameOf(int v) const {
    ValidateVertex(v);
    return keys_[v];
  }

  /**
   * Returns the digraph assoicated with the symbol graph. It is the client's responsibility
   * not to mutate the digraph.
   *
   * @return the digraph associated with the symbol digraph
   * @deprecated Replaced by {@link #digraph()}.
   */
  Digraph* G() const { return graph_; }
  /**
   * Returns the digraph assoicated with the symbol graph. It is the client's responsibility
   * not to mutate the digraph.
   *
   * @return the digraph associated with the symbol digraph
   */
  Digraph* digraph() const { return graph_; }
private:
  // throw an IllegalArgumentException unless {@code 0 <= v < V}
  void ValidateVertex(int v) const;

private:
  std::map<std::string, int> vertex_;  // string -> index
  std::vector<std::string> keys_;           // index  -> string
  Digraph* graph_;           // the underlying digraph
};
}

#endif  // SYMBO_DIGRAPH_H_
