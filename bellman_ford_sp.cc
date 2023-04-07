/******************************************************************************
 *  Compilation:  clang++ -c -O2 directed_edge.cc -std=c++11
 *                clang++ -c -O2 edge_weighted_digraph.cc -std=c++11
 *                clang++ -c -O2 edge_weighted_directed_cycle.cc -std=c++11
 *                clang++ -O2 -DDebug bellman_ford_sp.cc edge_weighted_directed_cycle.o edge_weighted_digraph.o directed_edge.o -std=c++11 -o bellman_ford_sp
 *  Execution:    bellman_ford_sp filename.txt s
 *  Dependencies: edge_weighted_digraph.h directed_edge.h
 *                edge_weighted_directed_cycle.h
 *  Data files:   https://algs4.cs.princeton.edu/44sp/tinyEWDn.txt
 *                https://algs4.cs.princeton.edu/44sp/mediumEWDnc.txt
 *
 *  Bellman-Ford shortest path algorithm. Computes the shortest path tree in
 *  edge-weighted digraph G from vertex s, or finds a negative cost cycle
 *  reachable from s.
 *
 *  % bellman_ford_sp tinyEWDn.txt 0
 *  0 to 0 ( 0.00)  
 *  0 to 1 ( 0.93)  0->2  0.26   2->7  0.34   7->3  0.39   3->6  0.52   6->4 -1.25   4->5  0.35   5->1  0.32
 *  0 to 2 ( 0.26)  0->2  0.26   
 *  0 to 3 ( 0.99)  0->2  0.26   2->7  0.34   7->3  0.39   
 *  0 to 4 ( 0.26)  0->2  0.26   2->7  0.34   7->3  0.39   3->6  0.52   6->4 -1.25   
 *  0 to 5 ( 0.61)  0->2  0.26   2->7  0.34   7->3  0.39   3->6  0.52   6->4 -1.25   4->5  0.35
 *  0 to 6 ( 1.51)  0->2  0.26   2->7  0.34   7->3  0.39   3->6  0.52   
 *  0 to 7 ( 0.60)  0->2  0.26   2->7  0.34   
 *
 *  % bellman_ford_sp tinyEWDnc.txt 0
 *  4->5  0.35
 *  5->4 -0.66
 *
 *
 ******************************************************************************/

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

/**
 * Computes a shortest paths tree from {@code s} to every other vertex in
 * the edge-weighted digraph {@code G}.
 * @param G the acyclic digraph
 * @param s the source vertex
 * @throws IllegalArgumentException unless {@code 0 <= s < V}
 */

#include "bellman_ford_sp.h"

#include <cassert>
#include <cstdio>
#include <algorithm>
#include <exception>

#include "directed_edge.h"
#include "edge_weighted_directed_cycle.h"

using std::vector;
using std::queue;
using std::stack;

namespace algs4 {
BellmanFordSP::BellmanFordSP(const EdgeWeightedDigraph& G, int s) {
  dist_to_.resize(G.V());
  edge_to_.resize(G.V());
  on_queue_.resize(G.V());
  for (int v = 0; v < G.V(); v++)
	dist_to_[v] = std::numeric_limits<double>::max();
  dist_to_[s] = 0.0;

  // Bellman-Ford algorithm
  queue_.push(s);
  on_queue_[s] = true;
  while (!queue_.empty() && !HasNegativeCycle()) {
	int v = queue_.front();
	queue_.pop();
	on_queue_[v] = false;
	Relax(G, v);
  }

  assert(Check(G, s));
}

// relax vertex v and put other endpoints on queue if changed
void BellmanFordSP::Relax(const EdgeWeightedDigraph& G, int v) {
  for (DirectedEdge* e : G.Adj(v)) {
	int w = e->to();
	if (dist_to_[w] > dist_to_[v] + e->weight()) {
	  dist_to_[w] = dist_to_[v] + e->weight();
	  edge_to_[w] = e;
	  if (!on_queue_[w]) {
		queue_.push(w);
		on_queue_[w] = true;
	  }
	}
	if (cost_++ % G.V() == 0) {
	  FindNegativeCycle();
	  if (HasNegativeCycle()) return;  // found a negative cycle
	}
  }
}

  // by finding a cycle in predecessor graph
void BellmanFordSP::FindNegativeCycle() {
  int V = edge_to_.size();
  EdgeWeightedDigraph spt(V);
  for (int v = 0; v < V; v++)
	if (edge_to_[v] != nullptr) spt.AddEdge(edge_to_[v]);

  EdgeWeightedDirectedCycle finder(spt);
  cycle_ = finder.cycle();
}

/**
 * Returns the length of a shortest path from the source vertex {@code s} to vertex {@code v}.
 * @param  v the destination vertex
 * @return the length of a shortest path from the source vertex {@code s} to vertex {@code v};
 *         {@code Double.POSITIVE_INFINITY} if no such path
 * @throws UnsupportedOperationException if there is a negative cost cycle reachable
 *         from the source vertex {@code s}
 * @throws IllegalArgumentException unless {@code 0 <= v < V}
 */
double BellmanFordSP::distTo(int v) const {
  ValidateVertex(v);
  if (HasNegativeCycle())
	throw "Negative cost cycle exists";
  return dist_to_[v];
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
vector<DirectedEdge *> BellmanFordSP::PathTo(int v) const {
  ValidateVertex(v);
  if (HasNegativeCycle())
	throw "Negative cost cycle exists";
  if (!HasPathTo(v)) return {};
  vector<DirectedEdge *> path;
  for (DirectedEdge* e = edge_to_[v]; e != nullptr; e = edge_to_[e->from()])
	path.push_back(e);
  std::reverse(path.begin(), path.end());

  return path;
}

// check optimality conditions: either 
// (i) there exists a negative cycle reacheable from s
//     or 
// (ii)  for all edges e = v->w:            distTo[w] <= distTo[v] + e.weight()
// (ii') for all edges e = v->w on the SPT: distTo[w] == distTo[v] + e.weight()
bool BellmanFordSP::Check(const EdgeWeightedDigraph& G, int s) {
  // has a negative cycle
  if (HasNegativeCycle()) {
	double weight = 0.0;
	stack<DirectedEdge *> edges(NegativeCycle());
	while (!edges.empty()) {
	  weight += edges.top()->weight();
	  edges.pop();
	}
	if (weight >= 0.0) {
	  printf("error: weight of negative cycle = %lf\n", weight);
	  return false;
	}
  } else { // no negative cycle reachable from source
	// check that distTo[v] and edgeTo[v] are consistent
	if (dist_to_[s] != 0.0 || edge_to_[s] != nullptr) {
	  printf("distanceTo[s] and edgeTo[s] inconsistent\n");
	  return false;
	}
	for (int v = 0; v < G.V(); v++) {
	  if (v == s) continue;
	  if (edge_to_[v] == nullptr && dist_to_[v] != std::numeric_limits<double>::max()) {
		printf("distTo[] and edgeTo[] inconsistent\n");
		return false;
	  }
	}

	// check that all edges e = v->w satisfy distTo[w] <= distTo[v] + e.weight()
	for (int v = 0; v < G.V(); v++) {
	  for (DirectedEdge* e : G.Adj(v)) {
		int w = e->to();
		if (dist_to_[v] + e->weight() < dist_to_[w]) {
		  printf("edge %s not relaxed\n", e->ToString().c_str());
		  return false;
		}
	  }
	}

	// check that all edges e = v->w on SPT satisfy distTo[w] == distTo[v] + e.weight()
	for (int w = 0; w < G.V(); w++) {
	  if (edge_to_[w] == nullptr) continue;
	  DirectedEdge* e = edge_to_[w];
	  int v = e->from();
	  if (w != e->to()) return false;
	  if (dist_to_[v] + e->weight() != dist_to_[w]) {
		printf("edge %s on shortest path not tight\n", e->ToString().c_str());
		return false;
	  }
	}
  }

  printf("Satisfies optimality conditions\n\n");
  return true;
}

// throw an IllegalArgumentException unless {@code 0 <= v < V}
void BellmanFordSP::ValidateVertex(int v) const {
  int V = dist_to_.size();
  if (v < 0 || v >= V)
	throw std::invalid_argument("vertex " + std::to_string(v) + " is not between 0 and " + std::to_string(V-1));
}
}

/**
 * Unit tests the {@code BellmanFordSP} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug

#include <fstream>

using namespace algs4;
using std::fstream;

int main(int args, char *argv[]) {
  fstream in(argv[1]);
  int s = std::stoi(argv[2]);
  EdgeWeightedDigraph G(in);

  BellmanFordSP sp(G, s);

  // print negative cycle
  if (sp.HasNegativeCycle()) {
	stack<DirectedEdge *> edges(sp.NegativeCycle());
	while (!edges.empty()) {
	  printf("%s\n", edges.top()->ToString().c_str());
	  edges.pop();
	}
  } else { // print shortest paths
	for (int v = 0; v < G.V(); v++) {
	  if (sp.HasPathTo(v)) {
		printf("%d to %d (%5.2f)  ", s, v, sp.distTo(v));
		for (DirectedEdge* e : sp.PathTo(v)) {
		  printf("%s   ", e->ToString().c_str());
		}
		printf("\n");
	  } else {
		printf("%d to %d           no path\n", s, v);
	  }
	}
  }

  return 0;
}
#endif

/******************************************************************************
 *  Copyright 2002-2018, Robert Sedgewick and Kevin Wayne.
 *
 *  This file is part of algs4.jar, which accompanies the textbook
 *
 *      Algorithms, 4th edition by Robert Sedgewick and Kevin Wayne,
 *      Addison-Wesley Professional, 2011, ISBN 0-321-57351-X.
 *      http://algs4.cs.princeton.edu
 *
 *
 *  algs4.jar is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  algs4.jar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with algs4.jar.  If not, see http://www.gnu.org/licenses.
 ******************************************************************************/
