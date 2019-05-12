#include <vector>
#include <stack>

#include "digraph.h"

class DirectedCycle {
 public:
  /**
   * Determines whether the digraph {@code G} has a directed cycle and, if so,
   * finds such a cycle.
   * @param G the digraph
   */
  DirectedCycle(const Digraph& G);
  /**
   * Does the digraph have a directed cycle?
   * @return {@code true} if the digraph has a directed cycle, {@code false} otherwise
   */
  bool hasCycle() const { return !_cycle.empty(); }
  /**
   * Returns a directed cycle if the digraph has a directed cycle, and {@code null} otherwise.
   * @return a directed cycle (as an iterable) if the digraph has a directed cycle,
   *    and {@code null} otherwise
   */
  std::stack<int> cycle() const { return _cycle; }

 private:
  // check that algorithm computes either the topological order or finds a directed cycle
  void dfs(const Digraph& G, int v);
  // certify that digraph has a directed cycle if it reports one
  bool check() const;

 private:
  std::vector<bool> _marked;        // marked[v] = has vertex v been marked?
  std::vector<int> _edge_to;            // edgeTo[v] = previous vertex on path to v
  std::vector<bool> _on_stack;       // onStack[v] = is vertex on the stack?
  std::stack<int> _cycle;    // directed cycle (or null if no such cycle)
};
