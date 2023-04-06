#ifndef ACYCLIC_SP_H_
#define ACYCLIC_SP_H_

#include <vector>
#include <stack>

#include "edge_weighted_digraph.h"

/**
 *  The {@code AcyclicSP} class represents a data type for solving the
 *  single-source shortest paths problem in edge-weighted directed acyclic
 *  graphs (DAGs). The edge weights can be positive, negative, or zero.
 *  <p>
 *  This implementation uses a topological-sort based algorithm.
 *  The constructor takes &Theta;(<em>V</em> + <em>E</em>) time in the
 *  worst case, where <em>V</em> is the number of vertices and
 *  <em>E</em> is the number of edges.
 *  Each instance method takes &Theta;(1) time.
 *  It uses &Theta;(<em>V</em>) extra space (not including the
 *  edge-weighted digraph).
 *  <p>
 *  This correctly computes shortest paths if all arithmetic performed is
 *  without floating-point rounding error or arithmetic overflow.
 *  This is the case if all edge weights are integers and if none of the
 *  intermediate results exceeds 2<sup>52</sup>. Since all intermediate
 *  results are sums of edge weights, they are bounded by <em>V C</em>,
 *  where <em>V</em> is the number of vertices and <em>C</em> is the maximum
 *  absolute value of any edge weight.
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/44sp">Section 4.4</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
namespace algs4 {

class DirectedEdge;

class AcyclicSP {
public:
  /**
   * Computes a shortest paths tree from {@code s} to every other vertex in
   * the directed acyclic graph {@code G}.
   * @param G the acyclic digraph
   * @param s the source vertex
   * @throws IllegalArgumentException if the digraph is not acyclic
   * @throws IllegalArgumentException unless {@code 0 <= s < V}
   */
  AcyclicSP(const EdgeWeightedDigraph& G, int s);
  AcyclicSP() = delete;
  AcyclicSP(const AcyclicSP& other) = delete;
  AcyclicSP &operator=(const AcyclicSP& other) = delete;
  AcyclicSP(AcyclicSP&& other) = default;
  AcyclicSP &operator=(AcyclicSP&& other) = default;

  /**
   * Returns the length of a shortest path from the source vertex {@code s} to vertex {@code v}.
   * @param  v the destination vertex
   * @return the length of a shortest path from the source vertex {@code s} to vertex {@code v};
   *         {@code Double.POSITIVE_INFINITY} if no such path
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  double dist_to(int v) const {
    ValidateVertex(v);
    return dist_to_[v];
  }

  /**
   * Is there a path from the source vertex {@code s} to vertex {@code v}?
   * @param  v the destination vertex
   * @return {@code true} if there is a path from the source vertex
   *         {@code s} to vertex {@code v}, and {@code false} otherwise
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  bool HasPathTo(int v) const;

  /**
   * Returns a shortest path from the source vertex {@code s} to vertex {@code v}.
   * @param  v the destination vertex
   * @return a shortest path from the source vertex {@code s} to vertex {@code v}
   *         as an iterable of edges, and {@code null} if no such path
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  std::stack<DirectedEdge *> PathTo(int v);
private:
  // relax edge e
  void Relax(DirectedEdge* e);

  // throw an IllegalArgumentException unless {@code 0 <= v < V}
  void ValidateVertex(int v) const;

private:
  std::vector<double> dist_to_;         // dist_to_[v] = distance  of shortest s->v path
  std::vector<DirectedEdge *> edge_to_;   // edge_to_[v] = last edge on shortest s->v path
};
}

#endif  // ACYCLIC_SP_H_
