#ifndef KRUSKAL_H_
#define KRUSKAL_H_

#include <queue>

#include "edge_weighted_graph.h"

namespace algs4 {

class Edge;
/**
 *  The {@code KruskalMST} class represents a data type for computing a
 *  <em>minimum spanning tree</em> in an edge-weighted graph.
 *  The edge weights can be positive, zero, or negative and need not
 *  be distinct. If the graph is not connected, it computes a <em>minimum
 *  spanning forest</em>, which is the union of minimum spanning trees
 *  in each connected component. The {@code weight()} method returns the
 *  weight of a minimum spanning tree and the {@code edges()} method
 *  returns its edges.
 *  <p>
 *  This implementation uses <em>Kruskal's algorithm</em> and the
 *  union-find data type.
 *  The constructor takes &Theta;(<em>E</em> log <em>E</em>) time in
 *  the worst case.
 *  Each instance method takes &Theta;(1) time.
 *  It uses &Theta;(<em>E</em>) extra space (not including the graph).
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
 *  For alternate implementations, see {@link LazyPrimMST}, {@link PrimMST},
 *  and {@link BoruvkaMST}.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */
class KruskalMST {
public:
  static constexpr double FLOATING_POINT_EPSILON = 1.0E-12;

  /**
   * Compute a minimum spanning tree (or forest) of an edge-weighted graph.
   * @param G the edge-weighted graph
   */
  KruskalMST(const EdgeWeightedGraph& G) noexcept;
  KruskalMST() = delete;
  KruskalMST(const KruskalMST& other) = default;
  KruskalMST &operator=(const KruskalMST& other) = default;
  KruskalMST(KruskalMST&& other) = default;
  KruskalMST &operator=(KruskalMST&& other) = default;

  /**
   * Returns the edges in a minimum spanning tree (or forest).
   * @return the edges in a minimum spanning tree (or forest) as
   *    an iterable of edges
   */
  const std::queue<Edge *>& edges() const { return edges_; }

  /**
   * Returns the sum of the edge weights in a minimum spanning tree (or forest).
   * @return the sum of the edge weights in a minimum spanning tree (or forest)
   */
  double weight() const { return weight_; }
private:
  // check optimality conditions (takes time proportional to E V lg* V)
  bool Check(const EdgeWeightedGraph& G) const;

private:
  double weight_;                        // weight of MST
  std::queue<Edge *> edges_;  // edges in MST
};
}

#endif  /* KRUSKAL_H_ */
