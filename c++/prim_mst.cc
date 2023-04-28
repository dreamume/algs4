/******************************************************************************
 *  Compilation:  clang++ -c -O2 edge.cc -std=c++20
 *                clang++ -c -O2 edge_weighted_graph.cc -std=c++20
 *                clang++ -DDebug -O2 edge.o edge_weighted_graph.o prim_mst.cc -std=c++20 -o prim_mst
 *  Execution:    ./prim_mst filename.txt
 *  Dependencies: edge.cc edge_weighted_graph.cc
 *  Data files:   https://algs4.cs.princeton.edu/43mst/tinyEWG.txt
 *                https://algs4.cs.princeton.edu/43mst/mediumEWG.txt
 *                https://algs4.cs.princeton.edu/43mst/largeEWG.txt
 *
 *  Compute a minimum spanning forest using Prim's algorithm.
 *
 *  %  ./prim_mst tinyEWG.txt
 *  1-7 0.19000
 *  0-2 0.26000
 *  2-3 0.17000
 *  4-5 0.35000
 *  5-7 0.28000
 *  6-2 0.40000
 *  0-7 0.16000
 *  1.81000
 *
 *  % ./prim_mst mediumEWG.txt
 *  1-72   0.06506
 *  2-86   0.05980
 *  3-67   0.09725
 *  4-55   0.06425
 *  5-102  0.03834
 *  6-129  0.05363
 *  7-157  0.00516
 *  ...
 *  10.46351
 *
 *  % ./prim_mst largeEWG.txt
 *  ...
 *  647.66307
 *
 ******************************************************************************/

#include "prim_mst.h"

#include <limits>
#include <iostream>

#include "edge.h"
#include "quick_union_uf.h"

using std::vector;
using std::cerr;
using std::endl;

namespace algs4 {
PrimMST::PrimMST(const EdgeWeightedGraph& G) noexcept : edgeTo_(G.V()), distTo_(G.V()), 
                                                        marked_(G.V()), 
                                                        pq_(G.V(), std::greater<double>()) {
  for (int v = 0; v < G.V(); v++)
    distTo_[v] = std::numeric_limits<double>::max();

  for (int v = 0; v < G.V(); v++)      // run from each vertex to find
    if (!marked_[v]) prim(G, v);      // minimum spanning forest

  // check optimality conditions
  assert(Check(G));
}

void PrimMST::prim(const EdgeWeightedGraph& G, int s) {
  distTo_[s] = 0.0;
  pq_.Insert(s, distTo_[s]);
  while (!pq_.isEmpty()) {
    int v = pq_.DelMin();
    Scan(G, v);
  }
}

void PrimMST::Scan(const EdgeWeightedGraph& G, int v) {
  marked_[v] = true;
  for (auto it = G.adj(v).begin(); it != G.adj(v).end(); ++it) {
    int w = (*it)->other(v);
    if (marked_[w]) continue;         // v-w is obsolete edge
    if ((*it)->weight() < distTo_[w]) {
      distTo_[w] = (*it)->weight();
      edgeTo_[w] = *it;
      if (pq_.Contains(w)) pq_.DecreaseKey(w, distTo_[w]);
      else                pq_.Insert(w, distTo_[w]);
    }
  }
}

double PrimMST::weight() const {
  double weight = 0.0;
  for (Edge* e : edges()) {
    if (!e) continue;
    weight += e->weight();
  }

  return weight;
}

bool PrimMST::Check(const EdgeWeightedGraph& G) const {
  // check weight
  double totalWeight = 0.0;
  for (Edge* e : edges()) {
    if (e) totalWeight += e->weight();
  }
  if (fabs(totalWeight - weight()) > FLOATING_POINT_EPSILON) {
    cerr << "Weight of edges does not equal weight(): " << totalWeight << " vs. " 
         << weight() << endl;
    return false;
  }

  // check that it is acyclic
  QuickUnionUF<int> uf(G.V());
  for (Edge* e : edges()) {
    if (!e) continue;
    int v = e->Either(), w = e->other(v);
    if (uf.Find(v) == uf.Find(w)) {
      cerr << "Not a forest" << endl;
      return false;
    }
    uf.UnionWith(v, w);
  }

  // check that it is a spanning forest
  for (Edge* e : G.Edges()) {
    if (!e) continue;
    int v = e->Either(), w = e->other(v);
    if (uf.Find(v) != uf.Find(w)) {
      cerr << "Not a spanning forest" << endl;
      return false;
    }
  }

  // check that it is a minimal spanning forest (cut optimality conditions)
  for (Edge* e : edges()) {
    if (!e) continue;
    // all edges in MST except e
    uf = QuickUnionUF<int>(G.V());
    for (Edge* f : edges()) {
      if (!f) continue;
      int x = f->Either(), y = f->other(x);
      if (f != e) uf.UnionWith(x, y);
    }

    // check that e is min weight edge in crossing cut
    for (Edge* f : G.Edges()) {
      int x = f->Either(), y = f->other(x);
      if (uf.Find(x) != uf.Find(y)) {
        if (f->weight() < e->weight()) {
          cerr << "Edge " << f->ToString() 
               << " violates cut optimality conditions" << endl;
          return false;
        }
      }
    }

  }

  return true;
}

}

/**
 * Unit tests the {@code PrimMST} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <fstream>
using std::fstream;
using namespace algs4;
int main(int args, char *argv[]) {
  EdgeWeightedGraph G(argv[1]);
  PrimMST mst(G);
  for (Edge* e : mst.edges()) {
    if (!e) continue;
    std::cout << e->ToString() << endl;
  }
  std::cout << mst.weight() << endl;

  return 0;
}
#endif
