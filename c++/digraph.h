#ifndef DIGRAPH_H_
#define DIGRAPH_H_

#include <vector>
#include <string>
#include <fstream>

namespace algs4 {
class Digraph {
public:
  friend class Digraph;
  /**
   * Initializes an empty digraph with <em>V</em> vertices.
   *
   * @param  V the number of vertices
   * @throws IllegalArgumentException if {@code V < 0}
   */
  Digraph(int V);
  Digraph(std::fstream& in);
  Digraph(const Digraph& G) noexcept;
  Digraph() = delete;
  Digraph &operator=(const Digraph& other) = default;
  Digraph(Digraph&& other) = default;
  Digraph &operator=(Digraph&& other) = default;
  /**
   * Returns the number of vertices in this digraph.
   *
   * @return the number of vertices in this digraph
   */
  int V() const { return v_; }

  /**
   * Returns the number of edges in this digraph.
   *
   * @return the number of edges in this digraph
   */
  int E() const { return e_; }
  /**
   * Adds the directed edge v→w to this digraph.
   *
   * @param  v the tail vertex
   * @param  w the head vertex
   * @throws IllegalArgumentException unless both {@code 0 <= v < V} and {@code 0 <= w < V}
   */
  void AddEdge(int v, int w);
  /**
   * Returns the vertices adjacent from vertex {@code v} in this digraph.
   *
   * @param  v the vertex
   * @return the vertices adjacent from vertex {@code v} in this digraph, as an iterable
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  const std::vector<int>& Adj(int v) const;
  /**
   * Returns the number of directed edges incident from vertex {@code v}.
   * This is known as the <em>outdegree</em> of vertex {@code v}.
   *
   * @param  v the vertex
   * @return the outdegree of vertex {@code v}               
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  int Outdegree(int v) const;
  /**
   * Returns the number of directed edges incident to vertex {@code v}.
   * This is known as the <em>indegree</em> of vertex {@code v}.
   *
   * @param  v the vertex
   * @return the indegree of vertex {@code v}               
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  int Indegree(int v) const;
  /**
   * Returns the reverse of the digraph.
   *
   * @return the reverse of the digraph
   */
  Digraph* Reverse() const;
  /**
   * Returns a string representation of the graph.
   *
   * @return the number of vertices <em>V</em>, followed by the number of edges <em>E</em>,  
   *         followed by the <em>V</em> adjacency lists
   */
  std::string ToString() const;
private:
  // throw an IllegalArgumentException unless {@code 0 <= v < V}
  void ValidateVertex(int v) const;
private:
  int v_;           // number of vertices in this digraph
  int e_{0};                 // number of edges in this digraph
  std::vector<std::vector<int>> adj_;    // adj[v] = adjacency list for vertex v
  std::vector<int> indegree_;        // indegree[v] = indegree of vertex v
};
}

#endif  // DIGRAPH_H_
