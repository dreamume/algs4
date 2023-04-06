/**
 *  The {@code Topological} class represents a data type for 
 *  determining a topological order of a directed acyclic graph (DAG).
 *  Recall, a digraph has a topological order if and only if it is a DAG.
 *  The <em>hasOrder</em> operation determines whether the digraph has
 *  a topological order, and if so, the <em>order</em> operation
 *  returns one.
 *  <p>
 *  This implementation uses depth-first search.
 *  The constructor takes time proportional to <em>V</em> + <em>E</em>
 *  (in the worst case),
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the <em>hasOrder</em> and <em>rank</em> operations takes constant time;
 *  the <em>order</em> operation takes time proportional to <em>V</em>.
 *  <p>
 *  See {@link DirectedCycle}, {@link DirectedCycleX}, and
 *  {@link EdgeWeightedDirectedCycle} to compute a
 *  directed cycle if the digraph is not a DAG.
 *  See {@link TopologicalX} for a nonrecursive queue-based algorithm
 *  to compute a topological order of a DAG.
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/42digraph">Section 4.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#ifndef TOPOLOGICAL_SORT_H_
#define TOPOLOGICAL_SORT_H_

#include <vector>

#include "digraph.h"
#include "edge_weighted_digraph.h"

namespace algs4 {
class Topological {
public:
  /**
   * Determines whether the digraph {@code G} has a topological order and, if so,
   * finds such a topological order.
   * @param G the digraph
   */
  Topological(const Digraph& G) noexcept;
  /**
   * Determines whether the edge-weighted digraph {@code G} has a topological
   * order and, if so, finds such an order.
   * @param G the edge-weighted digraph
   */
  Topological(const EdgeWeightedDigraph& G) noexcept;
  Topological() = delete;
  Topological(const Topological& other) = default;
  Topological &operator=(const Topological& other) = default;
  Topological(Topological&& other) = default;
  Topological &operator=(Topological&& other) = default;
  /**
   * Returns a topological order if the digraph has a topologial order,
   * and {@code null} otherwise.
   * @return a topological order of the vertices (as an interable) if the
   *    digraph has a topological order (or equivalently, if the digraph is a DAG),
   *    and {@code null} otherwise
   */
  std::vector<int>& order() { return order_; }
  /**
   * Does the digraph have a topological order?
   * @return {@code true} if the digraph has a topological order (or equivalently,
   *    if the digraph is a DAG), and {@code false} otherwise
   */
  bool HasOrder() const { return order_.size() > 0; }

  /**
   * Does the digraph have a topological order?
   * @return {@code true} if the digraph has a topological order (or equivalently,
   *    if the digraph is a DAG), and {@code false} otherwise
   * @deprecated Replaced by {@link #hasOrder()}.
   */
  bool IsDAG() const { return HasOrder(); }
  /**
   * The the rank of vertex {@code v} in the topological order;
   * -1 if the digraph is not a DAG
   *
   * @param v the vertex
   * @return the position of vertex {@code v} in a topological order
   *    of the digraph; -1 if the digraph is not a DAG
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  int rank(int v) const;
private:
  // throw an IllegalArgumentException unless {@code 0 <= v < V}
  void ValidateVertex(int v) const;
private:
  std::vector<int> order_;  // topological order
  std::vector<int> rank_;               // rank[v] = rank of vertex v in order
};
}

#endif  // TOPOLOGICAL_SORT_H_
