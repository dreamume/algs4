#ifndef PRIM_MST_H_
#define PRIM_MST_H_

#include <vector>

#include "index_min_priority_queue.h"
#include "edge_weighted_graph.h"

/**
 *  The {@code PrimMST} class represents a data type for computing a
 *  <em>minimum spanning tree</em> in an edge-weighted graph.
 *  The edge weights can be positive, zero, or negative and need not
 *  be distinct. If the graph is not connected, it computes a <em>minimum
 *  spanning forest</em>, which is the union of minimum spanning trees
 *  in each connected component. The {@code weight()} method returns the
 *  weight of a minimum spanning tree and the {@code edges()} method
 *  returns its edges.
 *  <p>
 *  This implementation uses <em>Prim's algorithm</em> with an indexed
 *  binary heap.
 *  The constructor takes &Theta;(<em>E</em> log <em>V</em>) time in
 *  the worst case, where <em>V</em> is the number of
 *  vertices and <em>E</em> is the number of edges.
 *  Each instance method takes &Theta;(1) time.
 *  It uses &Theta;(<em>V</em>) extra space (not including the
 *  edge-weighted graph).
 *  <p>
 *  This {@code weight()} method correctly computes the weight of the MST
 *  if all arithmetic performed is without floating-point rounding error
 *  or arithmetic overflow.
 *  This is the case if all edge weights are non-negative integers
 *  and the weight of the MST does not exceed 2<sup>52</sup>.
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/43mst">Section 4.3</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *  For alternate implementations, see {@link LazyPrimMST}, {@link KruskalMST},
 *  and {@link BoruvkaMST}.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

namespace algs4 {
class PrimMST {
public:
  static constexpr double FLOATING_POINT_EPSILON = 1.0E-12;

  /**
   * Compute a minimum spanning tree (or forest) of an edge-weighted graph.
   * @param G the edge-weighted graph
   */
  PrimMST(const EdgeWeightedGraph& G) noexcept;
  PrimMST() = delete;
  PrimMST(const PrimMST& other) = default;
  PrimMST &operator=(const PrimMST& other) = default;
  PrimMST(PrimMST&& other) = default;
  PrimMST &operator=(PrimMST&& other) = default;

  /**
   * Returns the edges in a minimum spanning tree (or forest).
   * @return the edges in a minimum spanning tree (or forest) as
   *    an iterable of edges
   */
  const std::vector<Edge *>& edges() const { return edgeTo_; }

  /**
   * Returns the sum of the edge weights in a minimum spanning tree (or forest).
   * @return the sum of the edge weights in a minimum spanning tree (or forest)
   */
  double weight() const;

private:
  // run Prim's algorithm in graph G, starting from vertex s
  void prim(const EdgeWeightedGraph& G, int s);

  // scan vertex v
  void Scan(const EdgeWeightedGraph& G, int v);

  // check optimality conditions (takes time proportional to E V lg* V)
  bool Check(const EdgeWeightedGraph& G) const;

private:
  std::vector<Edge *> edgeTo_;        // edgeTo_[v] = shortest edge from tree vertex to non-tree vertex
  std::vector<double> distTo_;      // distTo_[v] = weight of shortest such edge
  std::vector<bool> marked_;     // marked_[v] = true if v on tree, false otherwise
  IndexMinPriorityQueue<double> pq_;
};
}

#endif  // PRIM_MST_H_
