/******************************************************************************
 *  Compilation:  clang++ -c -O2 directed_edge.cc -std=c++20
 *                clang++ -c -O2 edge_weighted_digraph.cc -std=c++20
 *                clang++ -c -O2 topological_sort.cc -std=c++20
 *                clang++ -DDebug -O2 acyclic_sp.cc topological_sort.o directed_edge.o edge_weighted_digraph.o -std=c++20 -o acyclic_sp
 *  Execution:    ./acyclic_sp V E
 *  Dependencies: directed_edge.cc edge_weighted_digraph.cc topological_sort.cc
 *  Data files:   https://algs4.cs.princeton.edu/44sp/tinyEWDAG.txt
 *
 *  Computes shortest paths in an edge-weighted acyclic digraph.
 *
 *  % ./acyclic_sp tinyEWDAG.txt 5
 *  5 to 0 (0.73)  5->4  0.35   4->0  0.38
 *  5 to 1 (0.32)  5->1  0.32
 *  5 to 2 (0.62)  5->7  0.28   7->2  0.34
 *  5 to 3 (0.61)  5->1  0.32   1->3  0.29
 *  5 to 4 (0.35)  5->4  0.35
 *  5 to 5 (0.00)
 *  5 to 6 (1.13)  5->1  0.32   1->3  0.29   3->6  0.52
 *  5 to 7 (0.28)  5->7  0.28
 *
 ******************************************************************************/

#include "acyclic_sp.h"

#include <limits>
#include <exception>

#include "directed_edge.h"
#include "topological_sort.h"

using std::vector;
using std::stack;
using std::to_string;
using std::numeric_limits;

namespace algs4 {
  AcyclicSP::AcyclicSP(const EdgeWeightedDigraph& G, int s) : 
    dist_to_(G.V()), edge_to_(G.V()) {

  ValidateVertex(s);

  for (int v = 0; v < G.V(); v++)
    dist_to_[v] = numeric_limits<double>::max();
  dist_to_[s] = 0.0;

  // visit vertices in topological order
  Topological topological(G);
  if (!topological.HasOrder())
    throw std::invalid_argument("Digraph is not acyclic.");
  for (int v : topological.order()) {
    for (DirectedEdge* e : G.Adj(v))
      Relax(e);
  }
}

void AcyclicSP::Relax(DirectedEdge* e) {
  int v = e->from(), w = e->to();
  if (dist_to_[w] > dist_to_[v] + e->weight()) {
    dist_to_[w] = dist_to_[v] + e->weight();
    edge_to_[w] = e;
  }
}

bool AcyclicSP::HasPathTo(int v) const {
  ValidateVertex(v);
  return dist_to_[v] < numeric_limits<double>::max();
}

stack<DirectedEdge *> AcyclicSP::PathTo(int v) {
  ValidateVertex(v);
  stack<DirectedEdge *> path;
  if (HasPathTo(v)) {
    for (DirectedEdge* e = edge_to_[v]; e; e = edge_to_[e->from()])
      path.push(e);
  }

  return path;
}

void AcyclicSP::ValidateVertex(int v) const {
  int V = dist_to_.size();
  if (v < 0 || v >= V)
    throw std::invalid_argument("vertex " + to_string(v) + 
                                " is not between 0 and " + to_string(V-1));
}
}

/**
 * Unit tests the {@code AcyclicSP} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <iostream>
using namespace algs4;
using std::cout;
using std::endl;
int main(int argc, char *argv[]) {
  std::fstream in(argv[1]);
  EdgeWeightedDigraph G(in);
  int s = strtol(argv[2], nullptr, 10);

  // find shortest path from s to each other vertex in DAG
  AcyclicSP sp(G, s);
  for (int v = 0; v < G.V(); v++) {
    if (sp.HasPathTo(v)) {
      cout << s << " to " << v << " ( << " << sp.dist_to(v) << ")  ";
      stack<DirectedEdge *> data = sp.PathTo(v);
      while (!data.empty()) {
        cout << data.top()->ToString() << "   ";
        data.pop();
      }
      cout << endl;
    } else {
      cout << s << " to " << v << "         no path" << endl;
    }
  }

  return 0;
}
#endif
