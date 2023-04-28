/******************************************************************************
 *  Compilation:  clang++ -c -O2 edge.cc -std=c++20
 *                clang++ -DDebug -O2 edge.o lazy_prim_mst.cc -std=c++20 -o lazy_prim_mst
 *  Execution:    ./lazy_prim_mst filename.txt
 *  Dependencies: edge.cc
 *  Data files:   https://algs4.cs.princeton.edu/43mst/tinyEWG.txt
 *                https://algs4.cs.princeton.edu/43mst/mediumEWG.txt
 *                https://algs4.cs.princeton.edu/43mst/largeEWG.txt
 *
 *  Compute a minimum spanning forest using a lazy version of Prim's
 *  algorithm.
 *
 *  %  ./lazy_prim_mst tinyEWG.txt
 *  0-7 0.16000
 *  1-7 0.19000
 *  0-2 0.26000
 *  2-3 0.17000
 *  5-7 0.28000
 *  4-5 0.35000
 *  6-2 0.40000
 *  1.81000
 *
 *  % ./lazy_prim_mst mediumEWG.txt
 *  0-225   0.02383
 *  49-225  0.03314
 *  44-49   0.02107
 *  44-204  0.01774
 *  49-97   0.03121
 *  202-204 0.04207
 *  176-202 0.04299
 *  176-191 0.02089
 *  68-176  0.04396
 *  58-68   0.04795
 *  10.46351
 *
 *  % ./lazy_prim_mst largeEWG.txt
 *  ...
 *  647.66307
 *
 ******************************************************************************/

#include "lazy_prim_mst.h"

#include <cassert>
#include <iostream>

#include "quick_union_uf.h" 

using std::queue;
using std::vector;
using std::cerr;
using std::endl;

namespace algs4 {
LazyPrimMST::LazyPrimMST(const EdgeWeightedGraph& G) noexcept : marked_(G.V()) {
  for (int v = 0; v < G.V(); v++)     // run Prim from all vertices to
    if (!marked_[v]) Prim(G, v);     // get a minimum spanning forest

  // check optimality conditions
  assert(Check(G));
}

    // run Prim's algorithm
void LazyPrimMST::Prim(const EdgeWeightedGraph& G, int s) {
  Scan(G, s);
  while (!pq_.IsEmpty()) {                        // better to stop when mst has V-1 edges
    Edge* e = pq_.DelMin();                      // smallest edge on pq
    int v = e->Either(), w = e->other(v);        // two endpoints
    assert(marked_[v] || marked_[w]);
    if (marked_[v] && marked_[w]) continue;      // lazy, both v and w already scanned
    edges_.push(e);                            // add e to MST
    weight_ += e->weight();
    if (!marked_[v]) Scan(G, v);               // v becomes part of tree
    if (!marked_[w]) Scan(G, w);               // w becomes part of tree
  }
}

void LazyPrimMST::Scan(const EdgeWeightedGraph& G, int v) {
  assert(!marked_[v]);
  marked_[v] = true;
  for (Edge* e : G.adj(v))
    if (!marked_[e->other(v)]) pq_.Insert(e);
}

bool LazyPrimMST::Check(const EdgeWeightedGraph& G) {
  // check weight
  double totalWeight = 0.0;
  queue<Edge *> theEdges{edges()};
  while (!theEdges.empty()) {
    totalWeight += theEdges.front()->weight();
    theEdges.pop();
  }
  if (fabs(totalWeight - weight()) > FLOATING_POINT_EPSILON) {
    cerr << "Weight of edges does not equal weight(): " << totalWeight << " vs. " 
         << weight() << endl;
    return false;
  }

  // check that it is acyclic
  QuickUnionUF<int> uf(G.V());
  queue<Edge *> theEdges2{edges()};
  while (!theEdges2.empty()) {
    int v = theEdges2.front()->Either(), w = theEdges2.front()->other(v);
    if (uf.Find(v) == uf.Find(w)) {
      cerr << "Not a forest" << endl;
      return false;
    }
    uf.UnionWith(v, w);
    theEdges2.pop();
  }

  // check that it is a spanning forest
  for (Edge* e : G.Edges()) {
    int v = e->Either(), w = e->other(v);
    if (uf.Find(v) != uf.Find(w)) {
      cerr << "Not a spanning forest" << endl;
      return false;
    }
  }

  // check that it is a minimal spanning forest (cut optimality conditions)
  queue<Edge *> theEdges3{edges()};
  while (!theEdges3.empty()) {
    Edge* e = theEdges3.front();
    theEdges3.pop();

    // all edges in MST except e
    uf = QuickUnionUF<int>(G.V());
    queue<Edge *> theEdges4{edges()};
    while (!theEdges4.empty()) {
      Edge* f = theEdges4.front();
      theEdges4.pop();
      int x = f->Either(), y = f->other(x);
      if (f != e) uf.UnionWith(x, y);
    }

    // check that e is min weight edge in crossing cut
    queue<Edge *> theEdges5{edges()};
    while (!theEdges5.empty()) {
      Edge* f = theEdges5.front();
      theEdges5.pop();
      int x = f->Either(), y = f->other(x);
      if (uf.Find(x) != uf.Find(y)) {
        if (f->weight() < e->weight()) {
          cerr << "Edge " << f << " violates cut optimality conditions" << endl;
          return false;
        }
      }
    }

  }

  return true;
}
}

/**
 * Unit tests the {@code LazyPrimMST} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <fstream>
using std::fstream;
using namespace algs4;
int main(int args, char *argv[]) {
  EdgeWeightedGraph G(argv[1]);
  LazyPrimMST mst(G);
  queue<Edge *> current{mst.edges()};
  while (!current.empty()) {
    std::cout << current.front()->ToString() << endl;
    current.pop();
  }

  std::cout << mst.weight() << endl;

  return 0;
}
#endif
