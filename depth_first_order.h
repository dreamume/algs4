#ifndef DEPTH_FIRST_ORDER_H
#define DEPTH_FIRST_ORDER_H

#include <vector>
#include <queue>

#include "digraph.h"
#include "edge_weighted_digraph.h"

class DepthFirstOrder {
 public:
  /**
   * Determines a depth-first order for the digraph {@code G}.
   * @param G the digraph
   */
  DepthFirstOrder(const Digraph& G);
  /**
   * Determines a depth-first order for the edge-weighted digraph {@code G}.
   * @param G the edge-weighted digraph
   */
  DepthFirstOrder(const EdgeWeightedDigraph& G);
  /**
   * Returns the preorder number of vertex {@code v}.
   * @param  v the vertex
   * @return the preorder number of vertex {@code v}
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  int pre(int v) const;
  /**
   * Returns the postorder number of vertex {@code v}.
   * @param  v the vertex
   * @return the postorder number of vertex {@code v}
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  int post(int v) const;
  /**
   * Returns the vertices in postorder.
   * @return the vertices in postorder, as an iterable of vertices
   */
  const std::queue<int>& post() const { return _postorder; }
  /**
   * Returns the vertices in preorder.
   * @return the vertices in preorder, as an iterable of vertices
   */
  const std::queue<int>& pre() const { return _preorder; }
  /**
   * Returns the vertices in reverse postorder.
   * @return the vertices in reverse postorder, as an iterable of vertices
   */
  std::vector<int> reversePost() const;
 private:
  // run DFS in digraph G from vertex v and compute preorder/postorder
  void dfs(const Digraph& G, int v);
  // run DFS in edge-weighted digraph G from vertex v and compute preorder/postorder
  void dfs(const EdgeWeightedDigraph& G, int v);
  // check that pre() and post() are consistent with pre(v) and post(v)
  bool check() const;
 private:
  // throw an IllegalArgumentException unless {@code 0 <= v < V}
  void validateVertex(int v) const;
 private:
  std::vector<bool> _marked;          // marked[v] = has v been marked in dfs?
  std::vector<int> _pre;                 // pre[v]    = preorder  number of v
  std::vector<int> _post;                // post[v]   = postorder number of v
  std::queue<int> _preorder;   // vertices in preorder
  std::queue<int> _postorder;  // vertices in postorder
  int _pre_counter = 0;            // counter or preorder numbering
  int _post_counter = 0;           // counter for postorder numbering
};


#endif
