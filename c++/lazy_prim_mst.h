#ifndef LAZY_PRIM_MST_H_
#define LAZY_PRIM_MST_H_

#include <queue>
#include <vector>

#include "edge.h"
#include "edge_weighted_graph.h"
#include "heap_priority_queue.h"

namespace algs4 {
class EdgeCompare {
public:
  bool operator() (Edge* left, Edge* right) const {
    return left->weight() > right->weight();
  }
};

/**
 *  The {@code LazyPrimMST} class represents a data type for computing a
 *  <em>minimum spanning tree</em> in an edge-weighted graph.
 *  The edge weights can be positive, zero, or negative and need not
 *  be distinct. If the graph is not connected, it computes a <em>minimum
 *  spanning forest</em>, which is the union of minimum spanning trees
 *  in each connected component. The {@code weight()} method returns the
 *  weight of a minimum spanning tree and the {@code edges()} method
 *  returns its edges.
 *  <p>
 *  This implementation uses a lazy version of <em>Prim's algorithm</em>
 *  with a binary heap of edges.
 *  The constructor takes &Theta;(<em>E</em> log <em>E</em>) time in
 *  the worst case, where <em>V</em> is the number of vertices and
 *  <em>E</em> is the number of edges.
 *  Each instance method takes &Theta;(1) time.
 *  It uses &Theta;(<em>E</em>) extra space in the worst case
 *  (not including the edge-weighted graph).
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/43mst">Section 4.3</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *  For alternate implementations, see {@link PrimMST}, {@link KruskalMST},
 *  and {@link BoruvkaMST}.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

class LazyPrimMST {
public:
  static constexpr double FLOATING_POINT_EPSILON{1.0E-12};

  /**
   * Compute a minimum spanning tree (or forest) of an edge-weighted graph.
   * @param G the edge-weighted graph
   */
  LazyPrimMST(const EdgeWeightedGraph& G) noexcept;
  LazyPrimMST() = delete;
  LazyPrimMST(const LazyPrimMST& other) = delete;
  LazyPrimMST &operator=(const LazyPrimMST& other) = delete;
  LazyPrimMST(LazyPrimMST&& other) = delete;
  LazyPrimMST &operator=(LazyPrimMST&& other) = delete;

  /**
   * Returns the edges in a minimum spanning tree (or forest).
   * @return the edges in a minimum spanning tree (or forest) as
   *    an iterable of edges
   */
  std::queue<Edge *>& edges() { return edges_;}

  /**
   * Returns the sum of the edge weights in a minimum spanning tree (or forest).
   * @return the sum of the edge weights in a minimum spanning tree (or forest)
   */
  double weight() const { return weight_; }

private:
  // run Prim's algorithm
  void Prim(const EdgeWeightedGraph& G, int s);

  // add all edges e incident to v onto pq if the other endpoint has not yet been scanned
  void Scan(const EdgeWeightedGraph& G, int v);

  // check optimality conditions (takes time proportional to E V lg* V)
  bool Check(const EdgeWeightedGraph& G);

private:
  double weight_;       // total weight of MST
  std::queue<Edge *> edges_;     // edges in the MST
  std::vector<bool> marked_;    // marked[v] = true iff v on tree
  HeapPriorityQueue<Edge *, EdgeCompare> pq_;      // edges with one endpoint in tree
};
}

#endif  /* LAZY_PRIM_MST_H_ */
