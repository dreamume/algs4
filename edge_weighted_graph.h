#ifndef EDGE_WEIGHTED_GRAPH_H_
#define EDGE_WEIGHTED_GRAPH_H_

#include <list>
#include <vector>
#include <string>

class Edge;

/**
 *  The {@code EdgeWeightedGraph} class represents an edge-weighted
 *  graph of vertices named 0 through <em>V</em> – 1, where each
 *  undirected edge is of type {@link Edge} and has a real-valued weight.
 *  It supports the following two primary operations: add an edge to the graph,
 *  iterate over all of the edges incident to a vertex. It also provides
 *  methods for returning the degree of a vertex, the number of vertices
 *  <em>V</em> in the graph, and the number of edges <em>E</em> in the graph.
 *  Parallel edges and self-loops are permitted.
 *  By convention, a self-loop <em>v</em>-<em>v</em> appears in the
 *  adjacency list of <em>v</em> twice and contributes two to the degree
 *  of <em>v</em>.
 *  <p>
 *  This implementation uses an <em>adjacency-lists representation</em>, which
 *  is a vertex-indexed array of {@link Bag} objects.
 *  It uses &Theta;(<em>E</em> + <em>V</em>) space, where <em>E</em> is
 *  the number of edges and <em>V</em> is the number of vertices.
 *  All instance methods take &Theta;(1) time. (Though, iterating over
 *  the edges returned by {@link #adj(int)} takes time proportional
 *  to the degree of the vertex.)
 *  Constructing an empty edge-weighted graph with <em>V</em> vertices takes
 *  &Theta;(<em>V</em>) time; constructing an edge-weighted graph with
 *  <em>E</em> edges and <em>V</em> vertices takes
 *  &Theta;(<em>E</em> + <em>V</em>) time.
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/43mst">Section 4.3</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
class EdgeWeightedGraph {
public:
  static constexpr char* NEWLINE = "\n";

  /**
   * Initializes an empty edge-weighted graph with {@code V} vertices and 0 edges.
   *
   * @param  V the number of vertices
   * @throws IllegalArgumentException if {@code V < 0}
   */
  EdgeWeightedGraph(int V) noexcept :V_(V), adj_(V) {}

  /**
   * Initializes a random edge-weighted graph with {@code V} vertices and <em>E</em> edges.
   *
   * @param  V the number of vertices
   * @param  E the number of edges
   * @throws IllegalArgumentException if {@code V < 0}
   * @throws IllegalArgumentException if {@code E < 0}
   */
  EdgeWeightedGraph(int V, int E) noexcept;

  /**
   * Initializes an edge-weighted graph from an input stream.
   * The format is the number of vertices <em>V</em>,
   * followed by the number of edges <em>E</em>,
   * followed by <em>E</em> pairs of vertices and edge weights,
   * with each entry separated by whitespace.
   *
   * @param  in the input stream
   * @throws IllegalArgumentException if {@code in} is {@code null}
   * @throws IllegalArgumentException if the endpoints of any edge are not in prescribed range
   * @throws IllegalArgumentException if the number of vertices or edges is negative
   */
  EdgeWeightedGraph(const std::string& filename);

  /**
   * Initializes a new edge-weighted graph that is a deep copy of {@code G}.
   *
   * @param  G the edge-weighted graph to copy
   */
  EdgeWeightedGraph(const EdgeWeightedGraph& G) noexcept;
  EdgeWeightedGraph() = delete;
  EdgeWeightedGraph &operator=(const EdgeWeightedGraph& other) = delete;
  EdgeWeightedGraph(EdgeWeightedGraph&& other) = delete;
  EdgeWeightedGraph &operator=(EdgeWeightedGraph&& other) = delete;

  /**
   * Returns the number of vertices in this edge-weighted graph.
   *
   * @return the number of vertices in this edge-weighted graph
   */
  int V() const { return V_; }

  /**
   * Returns the number of edges in this edge-weighted graph.
   *
   * @return the number of edges in this edge-weighted graph
   */
  int E() const { return E_; }

  // throw an IllegalArgumentException unless {@code 0 <= v < V}
  void ValidateVertex(int v) const;

  /**
   * Adds the undirected edge {@code e} to this edge-weighted graph.
   *
   * @param  e the edge
   * @throws IllegalArgumentException unless both endpoints are between {@code 0} and {@code V-1}
   */
  void AddEdge(Edge* e);

  /**
   * Returns the edges incident on vertex {@code v}.
   *
   * @param  v the vertex
   * @return the edges incident on vertex {@code v} as an Iterable
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  std::list<Edge *>& adj(int v) const {
    ValidateVertex(v);
    return const_cast<std::list<Edge *>&>(adj_[v]);
  }

  /**
   * Returns the degree of vertex {@code v}.
   *
   * @param  v the vertex
   * @return the degree of vertex {@code v}
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  int degree(int v) const {
    ValidateVertex(v);
    return adj_[v].size();
  }

  /**
   * Returns all edges in this edge-weighted graph.
   * To iterate over the edges in this edge-weighted graph, use foreach notation:
   * {@code for (Edge e : G.edges())}.
   *
   * @return all edges in this edge-weighted graph, as an iterable
   */
  std::list<Edge *> Edges() const;

  /**
   * Returns a string representation of the edge-weighted graph.
   * This method takes time proportional to <em>E</em> + <em>V</em>.
   *
   * @return the number of vertices <em>V</em>, followed by the number of edges <em>E</em>,
   *         followed by the <em>V</em> adjacency lists of edges
   */
  std::string ToString() const;

private:
  int V_;
  int E_{0};
  std::vector<std::list<Edge *>> adj_;
};

#endif  // EDGE_WEIGHTED_GRAPH_H_
