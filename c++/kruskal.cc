/******************************************************************************
 *  Compilation:  clang++ -c -O2 edge.cc -std=c++20
 *                clang++ -c -O2 edge_weighted_graph.cc -std=c++20
 *                clang++ -DDebug -O2 edge.o edge_weighted_graph.o kruskal.cc -std=c++20 -o kruskal
 *  Execution:    ./kruskal filename.txt
 *  Dependencies: edge.cc edge_weighted_graph.cc
 *  Data files:   https://algs4.cs.princeton.edu/43mst/tinyEWG.txt
 *                https://algs4.cs.princeton.edu/43mst/mediumEWG.txt
 *                https://algs4.cs.princeton.edu/43mst/largeEWG.txt
 *
 *  Compute a minimum spanning forest using Kruskal's algorithm.
 *
 *  %  ./kruskal tinyEWG.txt
 *  0-7 0.16000
 *  2-3 0.17000
 *  1-7 0.19000
 *  0-2 0.26000
 *  5-7 0.28000
 *  4-5 0.35000
 *  6-2 0.40000
 *  1.81000
 *
 *  % ./kruskal mediumEWG.txt
 *  168-231 0.00268
 *  151-208 0.00391
 *  7-157   0.00516
 *  122-205 0.00647
 *  8-152   0.00702
 *  156-219 0.00745
 *  28-198  0.00775
 *  38-126  0.00845
 *  10-123  0.00886
 *  ...
 *  10.46351
 *
 ******************************************************************************/

#include "kruskal.h"

#include <vector>
#include <cassert>
#include <cmath>
#include <iostream>

#include "edge.h"
#include "quick_union_uf.h"

using std::vector;
using std::queue;
using std::sort;
using std::list;
using std::cerr;
using std::endl;

namespace algs4 {
KruskalMST::KruskalMST(const EdgeWeightedGraph& G) noexcept {
  // create array of edges, sorted by weight
  vector<Edge *> edges(G.E());
  int t{0};
  for (Edge* e: G.Edges())
    edges[t++] = e;
  sort(edges.begin(), edges.end(), [](const Edge* left, const Edge* right) {
    return left->weight() < right->weight();
  });

  // run greedy algorithm
  QuickUnionUF<int> uf(G.V());
  for (int i = 0; i < G.E() && edges_.size() < G.V() - 1; ++i) {
    Edge* e = edges[i];
    int v = e->Either();
    int w = e->other(v);

    // v-w does not create a cycle
    if (uf.Find(v) != uf.Find(w)) {
      uf.UnionWith(v, w);     // merge v and w components
      edges_.push(e);     // add edge e to mst
      weight_ += e->weight();
    }
  }

  // check optimality conditions
  assert(Check(G));
}

bool KruskalMST::Check(const EdgeWeightedGraph& G) const {
  // check total weight
  double total = 0.0;
  queue<Edge *> all_edges{edges()};
  while (!all_edges.empty()) {
    total += all_edges.front()->weight();
    all_edges.pop();
  }
  if (fabs(total - weight()) > FLOATING_POINT_EPSILON) {
    cerr << "Weight of edges does not equal weight(): " << total << " vs. " 
         << weight() << endl;
    return false;
  }

  // check that it is acyclic
  QuickUnionUF<int> uf(G.V());
  queue<Edge *> all_edges2{edges()};
  while (!all_edges2.empty()) {
    int v = all_edges2.front()->Either(), w = all_edges2.front()->other(v);
    if (uf.Find(v) == uf.Find(w)) {
      cerr << "Not a forest" << endl;
      return false;
    }
    uf.UnionWith(v, w);
    all_edges2.pop();
  }

  // check that it is a spanning forest
  for (Edge* f : G.Edges()) {
    int v = f->Either(), w = f->other(v);
    if (uf.Find(v) != uf.Find(w)) {
      cerr << "Not a spanning forest" << endl;
      return false;
    }
  }

  // check that it is a minimal spanning forest (cut optimality conditions)
  queue<Edge *> all_edges3{edges()};
  while (!all_edges3.empty()) {

    // all edges in MST except e
    uf = QuickUnionUF<int>(G.V());
    queue<Edge *> all_edges4{edges_};
    while (!all_edges4.empty()) {
      int x = all_edges4.front()->Either(), y = all_edges4.front()->other(x);
      if (all_edges4.front() != all_edges3.front()) uf.UnionWith(x, y);
      all_edges4.pop();
    }

    // check that e is min weight edge in crossing cut
    for (Edge* f : G.Edges()) {
      if (!f) continue;
      int x = f->Either(), y = f->other(x);
      if (uf.Find(x) != uf.Find(y)) {
        if (f->weight() < all_edges3.front()->weight()) {
          cerr << "Edge " << f->ToString() 
               << " violates cut optimality conditions" << endl;
          return false;
        }
      }
    }

    all_edges3.pop();
  }

  return true;
}
}

/**
 * Unit tests the {@code KruskalMST} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <fstream>
using std::fstream;
using namespace algs4;
int main(int args, char *argv[]) {
  EdgeWeightedGraph G(argv[1]);
  KruskalMST mst(G);
  queue<Edge *> all_edges{mst.edges()};
  while (!all_edges.empty()) {
    std::cout << all_edges.front()->ToString() << std::endl;
    all_edges.pop();
  }
  std::cout << mst.weight() << std::endl;

  return 0;
}
#endif
