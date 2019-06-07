/**
 *  The {@code BellmanFordSP} class represents a data type for solving the
 *  single-source shortest paths problem in edge-weighted digraphs with
 *  no negative cycles. 
 *  The edge weights can be positive, negative, or zero.
 *  This class finds either a shortest path from the source vertex <em>s</em>
 *  to every other vertex or a negative cycle reachable from the source vertex.
 *  <p>
 *  This implementation uses the Bellman-Ford-Moore algorithm.
 *  The constructor takes time proportional to <em>V</em> (<em>V</em> + <em>E</em>)
 *  in the worst case, where <em>V</em> is the number of vertices and <em>E</em>
 *  is the number of edges.
 *  Each call to {@code distTo(int)} and {@code hasPathTo(int)},
 *  {@code hasNegativeCycle} takes constant time;
 *  each call to {@code pathTo(int)} and {@code negativeCycle()}
 *  takes time proportional to length of the path returned.
 *  <p>
 *  For additional documentation,    
 *  see <a href="https://algs4.cs.princeton.edu/44sp">Section 4.4</a> of    
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne. 
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#ifndef BELLMAN_FORD_SP_H
#define BELLMAN_FORD_SP_H

#include <vector>
#include <queue>
#include <stack>
#include <limits>

#include "edge_weighted_digraph.h"

class DirectedEdge;

class BellmanFordSP {
 public:
  /**
   * Computes a shortest paths tree from {@code s} to every other vertex in
   * the edge-weighted digraph {@code G}.
   * @param G the acyclic digraph
   * @param s the source vertex
   * @throws IllegalArgumentException unless {@code 0 <= s < V}
   */
  BellmanFordSP(const EdgeWeightedDigraph& G, int s);
  /**
   * Is there a negative cycle reachable from the source vertex {@code s}?
   * @return {@code true} if there is a negative cycle reachable from the
   *    source vertex {@code s}, and {@code false} otherwise
   */
  bool hasNegativeCycle() const { return !_cycle.empty(); }
  /**
   * Returns a negative cycle reachable from the source vertex {@code s}, or {@code null}
   * if there is no such cycle.
   * @return a negative cycle reachable from the soruce vertex {@code s} 
   *    as an iterable of edges, and {@code null} if there is no such cycle
   */
  std::stack<DirectedEdge *> negativeCycle() const { return _cycle; }
  /**
   * Returns the length of a shortest path from the source vertex {@code s} to vertex {@code v}.
   * @param  v the destination vertex
   * @return the length of a shortest path from the source vertex {@code s} to vertex {@code v};
   *         {@code Double.POSITIVE_INFINITY} if no such path
   * @throws UnsupportedOperationException if there is a negative cost cycle reachable
   *         from the source vertex {@code s}
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  double distTo(int v) const;
  /**
   * Is there a path from the source {@code s} to vertex {@code v}?
   * @param  v the destination vertex
   * @return {@code true} if there is a path from the source vertex
   *         {@code s} to vertex {@code v}, and {@code false} otherwise
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  bool hasPathTo(int v) const { 
	validateVertex(v); 
	return _distTo[v] < std::numeric_limits<double>::max(); 
  }
  /**
   * Returns a shortest path from the source {@code s} to vertex {@code v}.
   * @param  v the destination vertex
   * @return a shortest path from the source {@code s} to vertex {@code v}
   *         as an iterable of edges, and {@code null} if no such path
   * @throws UnsupportedOperationException if there is a negative cost cycle reachable
   *         from the source vertex {@code s}
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  std::vector<DirectedEdge *> pathTo(int v) const;

 private:
  // relax vertex v and put other endpoints on queue if changed
  void relax(const EdgeWeightedDigraph& G, int v);
  // by finding a cycle in predecessor graph
  void findNegativeCycle();
  // check optimality conditions: either 
  // (i) there exists a negative cycle reacheable from s
  //     or 
  // (ii)  for all edges e = v->w:            distTo[w] <= distTo[v] + e.weight()
  // (ii') for all edges e = v->w on the SPT: distTo[w] == distTo[v] + e.weight()
  bool check(const EdgeWeightedDigraph& G, int s);
  // throw an IllegalArgumentException unless {@code 0 <= v < V}
  void validateVertex(int v) const;

 private:
  std::vector<double> _distTo;               // distTo[v] = distance  of shortest s->v path
  std::vector<DirectedEdge *> _edgeTo;         // edgeTo[v] = last edge on shortest s->v path
  std::vector<bool> _onQueue;             // onQueue[v] = is v currently on the queue?
  std::queue<int> _queue;          // queue of vertices to relax
  int _cost;                      // number of calls to relax()
  std::stack<DirectedEdge *> _cycle;  // negative cycle (or null if no such cycle)
};

#endif
