#ifndef EDGE_WEIGHTED_DIGRAPH_H
#define EDGE_WEIGHTED_DIGRAPH_H

#include <vector>
#include <random>
#include <fstream>
#include <string>

class DirectedEdge;

class EdgeWeightedDigraph {
 public:
  /**
   * Initializes an empty edge-weighted digraph with {@code V} vertices and 0 edges.
   *
   * @param  V the number of vertices
   * @throws IllegalArgumentException if {@code V < 0}
   */
  EdgeWeightedDigraph(int V);
  /**
   * Initializes a random edge-weighted digraph with {@code V} vertices and <em>E</em> edges.
   *
   * @param  V the number of vertices
   * @param  E the number of edges
   * @throws IllegalArgumentException if {@code V < 0}
   * @throws IllegalArgumentException if {@code E < 0}
   */
  EdgeWeightedDigraph(int V, int E);
  /**
   * Initializes a new edge-weighted digraph that is a deep copy of {@code G}.
   *
   * @param  G the edge-weighted digraph to copy
   */
  EdgeWeightedDigraph(const EdgeWeightedDigraph& G);
  /**  
   * Initializes an edge-weighted digraph from the specified input stream.
   * The format is the number of vertices <em>V</em>,
   * followed by the number of edges <em>E</em>,
   * followed by <em>E</em> pairs of vertices and edge weights,
   * with each entry separated by whitespace.
   *
   * @param  in the input stream
   * @throws IllegalArgumentException if the endpoints of any edge are not in prescribed range
   * @throws IllegalArgumentException if the number of vertices or edges is negative
   */
  EdgeWeightedDigraph(std::fstream& in);
  /**
   * Returns the number of vertices in this edge-weighted digraph.
   *
   * @return the number of vertices in this edge-weighted digraph
   */
  int V() const { return _V; }

  /**
   * Returns the number of edges in this edge-weighted digraph.
   *
   * @return the number of edges in this edge-weighted digraph
   */
  int E() const { return _E; }
  /**
   * Adds the directed edge {@code e} to this edge-weighted digraph.
   *
   * @param  e the edge
   * @throws IllegalArgumentException unless endpoints of edge are between {@code 0}
   *         and {@code V-1}
   */
  void addEdge(DirectedEdge* e);
  /**
   * Returns the directed edges incident from vertex {@code v}.
   *
   * @param  v the vertex
   * @return the directed edges incident from vertex {@code v} as an Iterable
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  const std::vector<DirectedEdge *>& adj(int v) const;
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
   * Returns all directed edges in this edge-weighted digraph.
   * To iterate over the edges in this edge-weighted digraph, use foreach notation:
   * {@code for (DirectedEdge e : G.edges())}.
   *
   * @return all edges in this edge-weighted digraph, as an iterable
   */
  std::vector<DirectedEdge*> edges() const;
  /**
   * Returns a string representation of this edge-weighted digraph.
   *
   * @return the number of vertices <em>V</em>, followed by the number of edges <em>E</em>,
   *         followed by the <em>V</em> adjacency lists of edges
   */
  std::string toString() const;

 private:
  // throw an IllegalArgumentException unless {@code 0 <= v < V}
  void validateVertex(int v) const;

private:
  int _V;                // number of vertices in this digraph
  int _E;                      // number of edges in this digraph
  std::vector<std::vector<DirectedEdge *>> _adj;    // adj[v] = adjacency list for vertex v
  std::vector<int> _indegree;             // indegree[v] = indegree of vertex v
  std::random_device _rd;
  std::mt19937 _gen;
};

#endif
