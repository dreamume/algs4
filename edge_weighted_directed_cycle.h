/**
 *  The {@code EdgeWeightedDirectedCycle} class represents a data type for 
 *  determining whether an edge-weighted digraph has a directed cycle.
 *  The <em>hasCycle</em> operation determines whether the edge-weighted
 *  digraph has a directed cycle and, if so, the <em>cycle</em> operation
 *  returns one.
 *  <p>
 *  This implementation uses depth-first search.
 *  The constructor takes time proportional to <em>V</em> + <em>E</em>
 *  (in the worst case),
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the <em>hasCycle</em> operation takes constant time;
 *  the <em>cycle</em> operation takes time proportional
 *  to the length of the cycle.
 *  <p>
 *  See {@link Topological} to compute a topological order if the edge-weighted
 *  digraph is acyclic.
 *  <p>
 *  For additional documentation,   
 *  see <a href="https://algs4.cs.princeton.edu/44sp">Section 4.4</a> of   
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne. 
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#ifndef EDGE_WEIGHTED_DIRECTED_CYCLE_H_
#define EDGE_WEIGHTED_DIRECTED_CYCLE_H_

#include <vector>
#include <stack>

#include "edge_weighted_digraph.h"

namespace algs4 {
class EdgeWeightedDirectedCycle {
public:
  /**
   * Determines whether the edge-weighted digraph {@code G} has a directed cycle and,
   * if so, finds such a cycle.
   * @param G the edge-weighted digraph
   */
  EdgeWeightedDirectedCycle(const EdgeWeightedDigraph& G) noexcept;
  EdgeWeightedDirectedCycle() = delete;
  EdgeWeightedDirectedCycle(const EdgeWeightedDirectedCycle& other) = default;
  EdgeWeightedDirectedCycle &operator=(const EdgeWeightedDirectedCycle& other) = default;
  EdgeWeightedDirectedCycle(EdgeWeightedDirectedCycle&& other) = default;
  EdgeWeightedDirectedCycle &operator=(EdgeWeightedDirectedCycle&& other) = default;
  /**
   * Does the edge-weighted digraph have a directed cycle?
   * @return {@code true} if the edge-weighted digraph has a directed cycle,
   * {@code false} otherwise
   */
  bool HasCycle() const { return !cycle_.empty(); }
  /**
   * Returns a directed cycle if the edge-weighted digraph has a directed cycle,
   * and {@code null} otherwise.
   * @return a directed cycle (as an iterable) if the edge-weighted digraph
   *    has a directed cycle, and {@code null} otherwise
   */
  std::stack<DirectedEdge *> cycle() const { return cycle_; }


private:
  // check that algorithm computes either the topological order or finds a directed cycle
  void Dfs(const EdgeWeightedDigraph& G, int v);
  // certify that digraph is either acyclic or has a directed cycle
  bool Check() const;
private:
  std::vector<bool> marked_;             // marked[v] = has vertex v been marked?
  std::vector<DirectedEdge *> edge_to_;        // edgeTo[v] = previous edge on path to v
  std::vector<bool> on_stack_;            // onStack[v] = is vertex on the stack?
  std::stack<DirectedEdge *> cycle_;    // directed cycle (or null if no such cycle)
};
}

#endif  // EDGE_WEIGHTED_DIRECTED_CYCLE_H_
