#ifndef TRANSITIVE_CLOSURE
#define TRANSITIVE_CLOSURE

#include <vector>

#include "digraph.h"

class DirectedDFS;

class TransitiveClosure {
public:
  /**
   * Computes the transitive closure of the digraph {@code G}.
   * @param G the digraph
   */
  TransitiveClosure(const Digraph& G);
  TransitiveClosure(const TransitiveClosure& other) = default;
  TransitiveClosure &operator=(const TransitiveClosure& other) = default;
  TransitiveClosure(TransitiveClosure&& other) = default;
  TransitiveClosure &operator=(TransitiveClosure&& other) = default;

  /**
   * Is there a directed path from vertex {@code v} to vertex {@code w} in the digraph?
   * @param  v the source vertex
   * @param  w the target vertex
   * @return {@code true} if there is a directed path from {@code v} to {@code w},
   *         {@code false} otherwise
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   * @throws IllegalArgumentException unless {@code 0 <= w < V}
   */
  bool Reachable(int v, int w) const;
private:
  // throw an IllegalArgumentException unless {@code 0 <= v < V}
  void ValidateVertex(int v) const;

private:
  std::vector<DirectedDFS *> tc_;  // tc[v] = Reachable from v
};

#endif
