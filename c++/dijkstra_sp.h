#ifndef DIJKSTRA_SP_H_
#define DIJKSTRA_SP_H_

#include <vector>
#include <stack>

#include "index_min_priority_queue.h"
#include "edge_weighted_digraph.h"

/**
 *  The {@code DijkstraSP} class represents a data type for solving the
 *  single-source shortest paths problem in edge-weighted digraphs
 *  where the edge weights are non-negative.
 *  <p>
 *  This implementation uses <em>Dijkstra's algorithm</em> with a
 *  <em>binary heap</em>. The constructor takes
 *  &Theta;(<em>E</em> log <em>V</em>) time in the worst case,
 *  where <em>V</em> is the number of vertices and <em>E</em> is
 *  the number of edges. Each instance method takes &Theta;(1) time.
 *  It uses &Theta;(<em>V</em>) extra space (not including the
 *  edge-weighted digraph).
 *  <p>
 *  This correctly computes shortest paths if all arithmetic performed is
 *  without floating-point rounding error or arithmetic overflow.
 *  This is the case if all edge weights are integers and if none of the
 *  intermediate results exceeds 2<sup>52</sup>. Since all intermediate
 *  results are sums of edge weights, they are bounded by <em>V C</em>,
 *  where <em>V</em> is the number of vertices and <em>C</em> is the maximum
 *  weight of any edge.
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/44sp">Section 4.4</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

namespace algs4 {

class DirectedEdge;

class DijkstraSP {
public:
    /**
     * Computes a shortest-paths tree from the source vertex {@code s} to every other
     * vertex in the edge-weighted digraph {@code G}.
     *
     * @param  G the edge-weighted digraph
     * @param  s the source vertex
     * @throws IllegalArgumentException if an edge weight is negative
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     */
  DijkstraSP(const EdgeWeightedDigraph& G, int s);
  DijkstraSP() = delete;
  DijkstraSP(const DijkstraSP& other) = delete;
  DijkstraSP &operator=(const DijkstraSP& other) = delete;
  DijkstraSP(DijkstraSP&& other) = default;
  DijkstraSP &operator=(DijkstraSP&& other) = default;

    /**
     * Returns the length of a shortest path from the source vertex {@code s} to vertex {@code v}.
     * @param  v the destination vertex
     * @return the length of a shortest path from the source vertex {@code s} to vertex {@code v};
     *         {@code Double.POSITIVE_INFINITY} if no such path
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
  double distTo(int v) const {
    ValidateVertex(v);
    return distTo_[v];
  }

    /**
     * Returns true if there is a path from the source vertex {@code s} to vertex {@code v}.
     *
     * @param  v the destination vertex
     * @return {@code true} if there is a path from the source vertex
     *         {@code s} to vertex {@code v}; {@code false} otherwise
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
  bool hasPathTo(int v) const;

    /**
     * Returns a shortest path from the source vertex {@code s} to vertex {@code v}.
     *
     * @param  v the destination vertex
     * @return a shortest path from the source vertex {@code s} to vertex {@code v}
     *         as an iterable of edges, and {@code null} if no such path
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
  std::stack<DirectedEdge *> pathTo(int v) const;

private:
    // relax edge e and update pq_ if changed
  void Relax(const DirectedEdge* e);

    // check optimality conditions:
    // (i) for all edges e:            distTo_[e.to()] <= distTo_[e.from()] + e.weight()
    // (ii) for all edge e on the SPT: distTo_[e.to()] == distTo_[e.from()] + e.weight()
  bool Check(const EdgeWeightedDigraph& G, int s) const;

    // throw an IllegalArgumentException unless {@code 0 <= v < V}
  void ValidateVertex(int v) const;

private:
  std::vector<double> distTo_;          // distTo_[v] = distance  of shortest s->v path
  std::vector<DirectedEdge *> edgeTo_;    // edgeTo_[v] = last edge on shortest s->v path
  IndexMinPriorityQueue<double> pq_;    // priority queue of vertices
};
}

#endif  /* DIJKSTRA_SP_H_ */
