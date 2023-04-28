/******************************************************************************
 *  Compilation:  clang++ -c -O2 edge_weighted_digraph.cc -std=c++11
 *                clang++ -c -O2 directed_edge.cc -std=c++11
 *                clang++ -c -O2 digraph.cc -std=c++11
 *                clang++ -c -O2 directed_cycle.cc -std=c++11
 *                clang++ -c -O2 depth_first_order.cc -std=c++11
 *                clang++ -c -O2 edge_weighted_directed_cycle.cc -std=c++11
 *                clang++ -c -O2 topological_sort.cc -std=c++11
 *                clang+ -DDebug -O2 acyclic_lp.cc edge_weighted_digraph.o directed_cycle.o directed_edge.o depth_first_order.o digraph.o edge_weighted_directed_cycle.o topological_sort.o -std=c++11 -o acyclic_lp
 *  Execution:    acyclic_lp V E
 *  Dependencies: edge_weighted_digraph.h directed_edge.h topological_sort.h
 *                digraph.h directed_cycle.h depth_first_order.h
 *                edge_weighted_directed_cycle.h
 *  Data files:   https://algs4.cs.princeton.edu/44sp/tinyEWDAG.txt
 *  
 *  Computes longeset paths in an edge-weighted acyclic digraph.
 *
 *  Remark: should probably check that graph is a DAG before running
 *
 *  % java AcyclicLP tinyEWDAG.txt 5
 *  5 to 0 (2.44)  5->1  0.32   1->3  0.29   3->6  0.52   6->4  0.93   4->0  0.38   
 *  5 to 1 (0.32)  5->1  0.32   
 *  5 to 2 (2.77)  5->1  0.32   1->3  0.29   3->6  0.52   6->4  0.93   4->7  0.37   7->2  0.34   
 *  5 to 3 (0.61)  5->1  0.32   1->3  0.29   
 *  5 to 4 (2.06)  5->1  0.32   1->3  0.29   3->6  0.52   6->4  0.93   
 *  5 to 5 (0.00)  
 *  5 to 6 (1.13)  5->1  0.32   1->3  0.29   3->6  0.52   
 *  5 to 7 (2.43)  5->1  0.32   1->3  0.29   3->6  0.52   6->4  0.93   4->7  0.37   
 *
 ******************************************************************************/

#include "acyclic_lp.h"

#include "topological_sort.h"

using std::vector;
using std::string;

namespace algs4 {
/**
 * Computes a longest paths tree from {@code s} to every other vertex in
 * the directed acyclic graph {@code G}.
 * @param G the acyclic digraph
 * @param s the source vertex
 * @throws IllegalArgumentException if the digraph is not acyclic
 * @throws IllegalArgumentException unless {@code 0 <= s < V}
 */
AcyclicLP::AcyclicLP(const EdgeWeightedDigraph& G, int s) : 
  dist_to_(G.V()), edge_to_(G.V()) {
  ValidateVertex(s);

  for (int v = 0; v < G.V(); v++)
	dist_to_[v] = std::numeric_limits<double>::min();
  dist_to_[s] = 0.0;

  // relax vertices in topological order
  Topological topological(G);
  if (!topological.HasOrder())
	throw std::invalid_argument("Digraph is not acyclic.");
  for (int v : topological.order()) {
	for (DirectedEdge* e : G.Adj(v))
	  Relax(*e);
  }
}

// relax edge e, but update if you find a *longer* path
void AcyclicLP::Relax(const DirectedEdge& e) {
  int v = e.from(), w = e.to();
  if (dist_to_[w] < dist_to_[v] + e.weight()) {
	dist_to_[w] = dist_to_[v] + e.weight();
	edge_to_[w] = const_cast<DirectedEdge *>(&e);
  }       
}

vector<DirectedEdge *> AcyclicLP::PathTo(int v) {
  ValidateVertex(v);
  if (!HasPathTo(v)) return {};
  std::stack<DirectedEdge *> path;
  for (DirectedEdge* e = edge_to_[v]; e != nullptr; e = edge_to_[e->from()]) {
    path.push(e);
  }
  std::vector<DirectedEdge *> res;
  while (!path.empty()) {
    res.push_back(path.top());
    path.pop();
  }

  return res;
}

void AcyclicLP::ValidateVertex(int v) const {
  int V = dist_to_.size();
  if (v < 0 || v >= V)
    throw std::invalid_argument("vertex " + std::to_string(v) + 
                                " is not between 0 and " + std::to_string(V-1));
}
}

/**
 * Unit tests the {@code AcyclicLP} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <fstream>
#include <cstdio>
using namespace algs4;
using std::fstream;
int main(int args, char *argv[]) {
  int s = std::stoi(argv[2]);
  fstream in(argv[1]);
  EdgeWeightedDigraph G(in);

  AcyclicLP lp(G, s);

  for (int v = 0; v < G.V(); v++) {
	if (lp.HasPathTo(v)) {
	  printf("%d to %d (%.2f)  ", s, v, lp.distTo(v));
	  for (DirectedEdge* e : lp.PathTo(v)) {
		printf("%s   ", e->ToString().c_str());
	  }
	  printf("\n");
	}
	else {
	  printf("%d to %d         no path\n", s, v);
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
