/**
 *  The {@code AcyclicLP} class represents a data type for solving the
 *  single-source longest paths problem in edge-weighted directed
 *  acyclic graphs (DAGs). The edge weights can be positive, negative, or zero.
 *  <p>
 *  This implementation uses a topological-sort based algorithm.
 *  The constructor takes time proportional to <em>V</em> + <em>E</em>,
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Each call to {@code distTo(int)} and {@code hasPathTo(int)} takes constant time;
 *  each call to {@code pathTo(int)} takes time proportional to the number of
 *  edges in the shortest path returned.
 *  <p>
 *  For additional documentation,   
 *  see <a href="https://algs4.cs.princeton.edu/44sp">Section 4.4</a> of   
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne. 
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#ifndef ACYCLIC_LP_H
#define ACYCLIC_LP_H

#include <vector>
#include <limits>
#include <stack>
#include <exception>
#include <string>

#include "edge_weighted_digraph.h"
#include "directed_edge.h"

class AcyclicLP {
 public:
  /**
   * Computes a longest paths tree from {@code s} to every other vertex in
   * the directed acyclic graph {@code G}.
   * @param G the acyclic digraph
   * @param s the source vertex
   * @throws IllegalArgumentException if the digraph is not acyclic
   * @throws IllegalArgumentException unless {@code 0 <= s < V}
   */
  AcyclicLP(const EdgeWeightedDigraph& G, int s);
  /**
   * Returns the length of a longest path from the source vertex {@code s} to vertex {@code v}.
   * @param  v the destination vertex
   * @return the length of a longest path from the source vertex {@code s} to vertex {@code v};
   *         {@code Double.NEGATIVE_INFINITY} if no such path
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  double distTo(int v) const {
	validateVertex(v);
	return _distTo[v];
  }
  /**
   * Is there a path from the source vertex {@code s} to vertex {@code v}?
   * @param  v the destination vertex
   * @return {@code true} if there is a path from the source vertex
   *         {@code s} to vertex {@code v}, and {@code false} otherwise
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  bool hasPathTo(int v) const {
	validateVertex(v);
	return _distTo[v] > std::numeric_limits<double>::min();
  }
  /**
   * Returns a longest path from the source vertex {@code s} to vertex {@code v}.
   * @param  v the destination vertex
   * @return a longest path from the source vertex {@code s} to vertex {@code v}
   *         as an iterable of edges, and {@code null} if no such path
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   */
  std::vector<DirectedEdge *> pathTo(int v) {
	validateVertex(v);
	if (!hasPathTo(v)) return {};
	std::stack<DirectedEdge *> path;
	for (DirectedEdge* e = _edgeTo[v]; e != nullptr; e = _edgeTo[e->from()]) {
	  path.push(e);
	}
	std::vector<DirectedEdge *> res;
	while (!path.empty()) {
	  res.push_back(path.top());
	  path.pop();
	}

	return res;
  }

 private:
  // relax edge e, but update if you find a *longer* path
  void relax(const DirectedEdge& e);
  // throw an IllegalArgumentException unless {@code 0 <= v < V}
  void validateVertex(int v) const {
	int V = _distTo.size();
	if (v < 0 || v >= V)
	  throw std::invalid_argument("vertex " + std::to_string(v) + 
								  " is not between 0 and " + std::to_string(V-1));
  }

 private:
  std::vector<double> _distTo;          // distTo[v] = distance  of longest s->v path
  std::vector<DirectedEdge *> _edgeTo;    // edgeTo[v] = last edge on longest s->v path
};

#endif
