/******************************************************************************
 *  Compilation:  clang++ -c -O2 edge.cc -std=c++20
 *                clang++ -DDebug -O2 edge.o edge_weighted_graph.cc -std=c++20 -o edge_weighted_graph
 *  Execution:    ./edge_weighted_graph filename.txt
 *  Dependencies: edge.cc
 *  Data files:   https://algs4.cs.princeton.edu/43mst/tinyEWG.txt
 *                https://algs4.cs.princeton.edu/43mst/mediumEWG.txt
 *                https://algs4.cs.princeton.edu/43mst/largeEWG.txt
 *
 *  An edge-weighted undirected graph, implemented using adjacency lists.
 *  Parallel edges and self-loops are permitted.
 *
 *  % ./edge_weighted_graph tinyEWG.txt
 *  8 16
 *  0: 6-0 0.58000  0-2 0.26000  0-4 0.38000  0-7 0.16000
 *  1: 1-3 0.29000  1-2 0.36000  1-7 0.19000  1-5 0.32000
 *  2: 6-2 0.40000  2-7 0.34000  1-2 0.36000  0-2 0.26000  2-3 0.17000
 *  3: 3-6 0.52000  1-3 0.29000  2-3 0.17000
 *  4: 6-4 0.93000  0-4 0.38000  4-7 0.37000  4-5 0.35000
 *  5: 1-5 0.32000  5-7 0.28000  4-5 0.35000
 *  6: 6-4 0.93000  6-0 0.58000  3-6 0.52000  6-2 0.40000
 *  7: 2-7 0.34000  1-7 0.19000  0-7 0.16000  5-7 0.28000  4-7 0.37000
 *
 ******************************************************************************/

#include "edge_weighted_graph.h"

#include <cstdlib>
#include <fstream>
#include <exception>

#include "edge.h"

using std::string;
using std::to_string;
using std::stoi;
using std::stod;
using std::list;
using std::fstream;

EdgeWeightedGraph::EdgeWeightedGraph(int V, int E) noexcept : EdgeWeightedGraph(V) {
  for (int i = 0; i < E; i++) {
    int v = random() % V;
    int w = random() % V;
    double weight = 0.01 * (random() % 101);
    Edge* e = new Edge(v, w, weight);
    AddEdge(e);
  }
}

EdgeWeightedGraph::EdgeWeightedGraph(const string& filename) {
  fstream in(filename);

  while (!in.eof()) {
    string line;
    std::getline(in, line);
    if (line == "") throw std::out_of_range("input file content is invalid");

    V_ = stoi(line);
    adj_.resize(V_);

    std::getline(in, line);
    if (line == "") throw std::out_of_range("input file content is invalid");
    int E = stoi(line);
    if (E < 0) throw std::invalid_argument("Number of edges must be non-negative");

    for (int i = 0; i < E; i++) {
      std::getline(in, line);
      if (line == "") throw std::out_of_range("input file content is invalid");
      string::size_type pos = line.find(' ', 0);
      int v = stoi(line.substr(0, pos));
      string::size_type next_pos = line.find(' ', pos + 1);
      int w = stoi(line.substr(pos + 1, next_pos - pos));
      ValidateVertex(v);
      ValidateVertex(w);
      double weight = stod(line.substr(next_pos + 1));
      Edge* e = new Edge(v, w, weight);
      AddEdge(e);
    }
    break;
  }
}

EdgeWeightedGraph::EdgeWeightedGraph(const EdgeWeightedGraph& G) noexcept : 
  EdgeWeightedGraph(G.V()) {
  E_ = G.E();
  for (int v = 0; v < G.V(); v++) {
    for_each (G.adj_[v].cbegin(), G.adj_[v].cend(), [&](auto& item) {
      adj_[v].push_back(item);
        });
  }
}

void EdgeWeightedGraph::ValidateVertex(int v) const {
  if (v < 0 || v >= V_)
    throw std::invalid_argument("vertex " + to_string(v) + 
                                " is not between 0 and " + to_string(V_-1));
}

void EdgeWeightedGraph::AddEdge(Edge* e) {
  int v = e->Either();
  int w = e->other(v);
  ValidateVertex(v);
  ValidateVertex(w);
  adj_[v].push_back(e);
  adj_[w].push_back(e);
  E_++;
}

list<Edge *> EdgeWeightedGraph::Edges() const {
  list<Edge *> res;
  for (int v = 0; v < V_; v++) {
    int selfLoops = 0;
    for (Edge* e : adj(v)) {
      if (e->other(v) > v) {
        res.push_back(e);
      }
      // add only one copy of each self loop (self loops will be consecutive)
      else if (e->other(v) == v) {
        if (selfLoops % 2 == 0) res.push_back(e);
        selfLoops++;
      }
    }
  }

  return res;
}

string EdgeWeightedGraph::ToString() const {
  string res = to_string(V_) + " " + to_string(E_) + NEWLINE;

  for (int v = 0; v < V_; v++) {
    res += to_string(v) + ": ";
    for (Edge* e : adj_[v])
      res += e->ToString() + "  ";
    res += NEWLINE;
  }

  return res;
}

/**
 * Unit tests the {@code EdgeWeightedGraph} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <iostream>
int main(int argc, char *argv[]) {
  EdgeWeightedGraph G(argv[1]);
  std::cout << G.ToString() << std::endl;

  return 0;
}
#endif
