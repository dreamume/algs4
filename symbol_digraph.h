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
  SymbolDigraph(const std::string& filename, const std::string& delimiter);
  /**
   * Does the digraph contain the vertex named {@code s}?
   * @param s the name of a vertex
   * @return {@code true} if {@code s} is the name of a vertex, and {@code false} otherwise
   */
  bool contains(const std::string& s) const { return _vertex.find(s) != _vertex.end(); }
  /**
   * Returns the integer associated with the vertex named {@code s}.
   * @param s the name of a vertex
   * @return the integer (between 0 and <em>V</em> - 1) associated with the vertex named {@code s}
   * @deprecated Replaced by {@link #indexOf(String)}.
   */
  int index(const std::string& s) { return _vertex[s]; }
  /**
   * Returns the integer associated with the vertex named {@code s}.
   * @param s the name of a vertex
   * @return the integer (between 0 and <em>V</em> - 1) associated with the vertex named {@code s}
   */
  int indexOf(const std::string& s) { return _vertex[s]; }
  /**
   * Returns the name of the vertex associated with the integer {@code v}.
   * @param  v the integer corresponding to a vertex (between 0 and <em>V</em> - 1) 
   * @return the name of the vertex associated with the integer {@code v}
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   * @deprecated Replaced by {@link #nameOf(int)}.
   */
  std::string name(int v) const { 
	validateVertex(v);
	return _keys[v];
  }
  /**
   * Returns the name of the vertex associated with the integer {@code v}.
   * @param  v the integer corresponding to a vertex (between 0 and <em>V</em> - 1) 
   * @return the name of the vertex associated with the integer {@code v}
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  std::string nameOf(int v) const {
      validateVertex(v);
      return _keys[v];
  }

  /**
   * Returns the digraph assoicated with the symbol graph. It is the client's responsibility
   * not to mutate the digraph.
   *
   * @return the digraph associated with the symbol digraph
   * @deprecated Replaced by {@link #digraph()}.
   */
  Digraph* G() const { return _graph; }
  /**
   * Returns the digraph assoicated with the symbol graph. It is the client's responsibility
   * not to mutate the digraph.
   *
   * @return the digraph associated with the symbol digraph
   */
  Digraph* digraph() const { return _graph; }
 private:
  // throw an IllegalArgumentException unless {@code 0 <= v < V}
  void validateVertex(int v) const;

 private:
  std::map<std::string, int> _vertex;  // string -> index
  std::vector<std::string> _keys;           // index  -> string
  Digraph* _graph;           // the underlying digraph
};
