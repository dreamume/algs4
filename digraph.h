#ifndef DIGRAPH_H
#define DIGRAPH_H

#include <vector>
#include <string>
#include <fstream>

class Digraph {
 public:
  /**
   * Initializes an empty digraph with <em>V</em> vertices.
   *
   * @param  V the number of vertices
   * @throws IllegalArgumentException if {@code V < 0}
   */
  Digraph(int V);
  Digraph(std::fstream& in);
  Digraph(const Digraph& G);
  /**
   * Returns the number of vertices in this digraph.
   *
   * @return the number of vertices in this digraph
   */
  int V() const { return _V; }

  /**
   * Returns the number of edges in this digraph.
   *
   * @return the number of edges in this digraph
   */
  int E() const { return _E; }
  /**
   * Adds the directed edge v→w to this digraph.
   *
   * @param  v the tail vertex
   * @param  w the head vertex
   * @throws IllegalArgumentException unless both {@code 0 <= v < V} and {@code 0 <= w < V}
   */
  void addEdge(int v, int w);
  /**
   * Returns the vertices adjacent from vertex {@code v} in this digraph.
   *
   * @param  v the vertex
   * @return the vertices adjacent from vertex {@code v} in this digraph, as an iterable
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  const std::vector<int>& adj(int v) const;
  /**
   * Returns the number of directed edges incident from vertex {@code v}.
   * This is known as the <em>outdegree</em> of vertex {@code v}.
   *
   * @param  v the vertex
   * @return the outdegree of vertex {@code v}               
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  int outdegree(int v) const;
  /**
   * Returns the number of directed edges incident to vertex {@code v}.
   * This is known as the <em>indegree</em> of vertex {@code v}.
   *
   * @param  v the vertex
   * @return the indegree of vertex {@code v}               
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  int indegree(int v) const;
  /**
   * Returns the reverse of the digraph.
   *
   * @return the reverse of the digraph
   */
  Digraph* reverse() const;
  /**
   * Returns a string representation of the graph.
   *
   * @return the number of vertices <em>V</em>, followed by the number of edges <em>E</em>,  
   *         followed by the <em>V</em> adjacency lists
   */
  std::string toString() const;
 private:
  // throw an IllegalArgumentException unless {@code 0 <= v < V}
  void validateVertex(int v) const;
 private:
  int _V;           // number of vertices in this digraph
  int _E;                 // number of edges in this digraph
  std::vector<std::vector<int>> _adj;    // adj[v] = adjacency list for vertex v
  std::vector<int> _indegree;        // indegree[v] = indegree of vertex v
};

#endif
