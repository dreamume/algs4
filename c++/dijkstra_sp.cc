/******************************************************************************
 *  Compilation:  clang++ -c -O2 directed_edge.cc -std=c++20
 *                clang++ -c -O2 edge_weighted_digraph.cc -std=c++20
 *                clang++ -DDebug -O2 dijkstra_sp.cc directed_edge.o edge_weighted_digraph.o -std=c++20 -o dijkstra_sp
 *  Execution:    ./dijkstra_sp input.txt s
 *  Dependencies: directed_edge.cc edge_weighted_digraph.cc
 *  Data files:   https://algs4.cs.princeton.edu/44sp/tinyEWD.txt
 *                https://algs4.cs.princeton.edu/44sp/mediumEWD.txt
 *                https://algs4.cs.princeton.edu/44sp/largeEWD.txt
 *
 *  Dijkstra's algorithm. Computes the shortest path tree.
 *  Assumes all weights are non-negative.
 *
 *  % ./dijkstra_sp tinyEWD.txt 0
 *  0 to 0 (0.00)
 *  0 to 1 (1.05)  0->4  0.38   4->5  0.35   5->1  0.32
 *  0 to 2 (0.26)  0->2  0.26
 *  0 to 3 (0.99)  0->2  0.26   2->7  0.34   7->3  0.39
 *  0 to 4 (0.38)  0->4  0.38
 *  0 to 5 (0.73)  0->4  0.38   4->5  0.35
 *  0 to 6 (1.51)  0->2  0.26   2->7  0.34   7->3  0.39   3->6  0.52
 *  0 to 7 (0.60)  0->2  0.26   2->7  0.34
 *
 *  % ./dijkstra_sp mediumEWD.txt 0
 *  0 to 0 (0.00)
 *  0 to 1 (0.71)  0->44  0.06   44->93  0.07   ...  107->1  0.07
 *  0 to 2 (0.65)  0->44  0.06   44->231  0.10  ...  42->2  0.11
 *  0 to 3 (0.46)  0->97  0.08   97->248  0.09  ...  45->3  0.12
 *  0 to 4 (0.42)  0->44  0.06   44->93  0.07   ...  77->4  0.11
 *  ...
 *
 ******************************************************************************/

#include "dijkstra_sp.h"

#include <exception>
#include <limits>
#include <string>
#include <iostream>
#include <cassert>

#include "directed_edge.h"

using std::vector;
using std::cerr;
using std::endl;
using std::numeric_limits;
using std::to_string;
using std::stack;

namespace algs4 {
DijkstraSP::DijkstraSP(const EdgeWeightedDigraph& G, int s) : 
  distTo_(G.V(), numeric_limits<double>::max()), edgeTo_(G.V()), 
  pq_(G.V(), std::greater<double>()) {
  for (const auto e : G.edges()) {
    if (e->weight() < 0)
      throw std::invalid_argument("edge " + e->ToString() + " has negative weight");
  }

  ValidateVertex(s);

  // for (int v = 0; v < G.V(); v++)
  //   distTo_[v] = numeric_limits<double>::max();
  distTo_[s] = 0.0;

  // relax vertices in order of distance from s
  pq_.Insert(s, distTo_[s]);
  while (!pq_.IsEmpty()) {
    int v = pq_.DelMin();
    for (DirectedEdge* e : G.Adj(v))
      Relax(e);
  }

  // check optimality conditions
  assert(Check(G, s));
}

void DijkstraSP::Relax(const DirectedEdge* e) {
  int v = e->from(), w = e->to();
  if (distTo_[w] > distTo_[v] + e->weight()) {
    distTo_[w] = distTo_[v] + e->weight();
    edgeTo_[w] = const_cast<DirectedEdge *>(e);
    if (pq_.Contains(w)) pq_.DecreaseKey(w, distTo_[w]);
    else                pq_.Insert(w, distTo_[w]);
  }
}

bool DijkstraSP::Check(const EdgeWeightedDigraph& G, int s) const {
  // check that edge weights are non-negative
  for (DirectedEdge* e : G.edges()) {
    if (e->weight() < 0) {
      cerr << "negative edge weight detected" << endl;
      return false;
    }
  }

  // check that distTo_[v] and edgeTo_[v] are consistent
  if (distTo_[s] != 0.0 || edgeTo_[s]) {
    cerr << "distTo_[s] and edgeTo_[s] inconsistent" << endl;
    return false;
  }
  for (int v = 0; v < G.V(); v++) {
    if (v == s) continue;
    if (!edgeTo_[v] && distTo_[v] != numeric_limits<double>::max()) {
      cerr << "distTo_[] and edgeTo_[] inconsistent" << endl;
      return false;
    }
  }

  // check that all edges e = v->w satisfy distTo_[w] <= distTo_[v] + e.weight()
  for (int v = 0; v < G.V(); v++) {
    for (DirectedEdge* e : G.Adj(v)) {
      int w = e->to();
      if (distTo_[v] + e->weight() < distTo_[w]) {
        cerr << "edge " + e->ToString() + " not relaxed" << endl;
        return false;
      }
    }
  }

  // check that all edges e = v->w on SPT satisfy distTo_[w] == distTo_[v] + e.weight()
  for (int w = 0; w < G.V(); w++) {
    if (!edgeTo_[w]) continue;
    DirectedEdge* e = edgeTo_[w];
    int v = e->from();
    if (w != e->to()) return false;
    if (distTo_[v] + e->weight() != distTo_[w]) {
      cerr << "edge " + e->ToString() + " on shortest path not tight" << endl;
      return false;
    }
  }

  return true;
}

void DijkstraSP::ValidateVertex(int v) const {
  int V = distTo_.size();
  if (v < 0 || v >= V)
    throw std::invalid_argument("vertex " + to_string(v) + 
                                " is not between 0 and " + to_string(V-1));
}

bool DijkstraSP::hasPathTo(int v) const {
  ValidateVertex(v);
  return distTo_[v] < numeric_limits<double>::max();
}

stack<DirectedEdge *> DijkstraSP::pathTo(int v) const {
  stack<DirectedEdge *> path;
  ValidateVertex(v);
  if (hasPathTo(v)) {
    for (DirectedEdge* e = edgeTo_[v]; e; e = edgeTo_[e->from()])
      path.push(e);
  }

  return path;
}
}

/**
 * Unit tests the {@code DijkstraSP} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <cstdlib>
using namespace algs4;
using std::cout;
using std::fstream;
int main(int args, char *argv[]) {
  fstream in(argv[1]);
  EdgeWeightedDigraph G(in);
  int s = strtol(argv[2], nullptr, 10);

  // compute shortest paths
  DijkstraSP sp(G, s);

  // print shortest path
  for (int t = 0; t < G.V(); t++) {
    if (sp.hasPathTo(t)) {
      cout << to_string(s) << " to " << to_string(t) << " (" << sp.distTo(t)
           << ")  " << endl;
      stack<DirectedEdge *> data = sp.pathTo(t);
      while (!data.empty()) {
        cout << data.top()->ToString() << "   ";
        data.pop();
      }

      cout << endl;
    } else {
      cout << to_string(s) << " to " << to_string(t) << "         no path" 
           << endl;
    }
  }

  return 0;
}
#endif
