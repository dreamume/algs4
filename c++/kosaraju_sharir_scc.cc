/******************************************************************************
 *  Compilation:  clang++ -c -O2 digraph.cc -std=c++20
 *                clang++ -c -O2 depth_first_order.cc -std=c++20
 *                clang++ -DDebug -O2 kosaraju_sharir_scc.cc digraph.o depth_first_order.o -std=c++20 -o kosaraju_sharir_scc
 *  Execution:    ./kosaraju_sharir_scc filename.txt
 *  Dependencies: digraph.cc depth_first_order.cc
 *  Data files:   https://algs4.cs.princeton.edu/42digraph/tinyDG.txt
 *                https://algs4.cs.princeton.edu/42digraph/mediumDG.txt
 *                https://algs4.cs.princeton.edu/42digraph/largeDG.txt
 *
 *  Compute the strongly-connected components of a digraph using the
 *  Kosaraju-Sharir algorithm.
 *
 *  Runs in O(E + V) time.
 *
 *  % java KosarajuSharirSCC tinyDG.txt
 *  5 strong components
 *  1
 *  0 2 3 4 5
 *  9 10 11 12
 *  6 8
 *  7
 *
 *  % java KosarajuSharirSCC mediumDG.txt
 *  10 strong components
 *  21
 *  2 5 6 8 9 11 12 13 15 16 18 19 22 23 25 26 28 29 30 31 32 33 34 35 37 38 39 40 42 43 44 46 47 48 49
 *  14
 *  3 4 17 20 24 27 36
 *  41
 *  7
 *  45
 *  1
 *  0
 *  10
 *
 *  % java -Xss50m KosarajuSharirSCC mediumDG.txt
 *  25 strong components
 *  7 11 32 36 61 84 95 116 121 128 230   ...
 *  28 73 80 104 115 143 149 164 184 185  ...
 *  38 40 200 201 207 218 286 387 418 422 ...
 *  12 14 56 78 87 103 216 269 271 272    ...
 *  42 48 112 135 160 217 243 246 273 346 ...
 *  46 76 96 97 224 237 297 303 308 309   ...
 *  9 15 21 22 27 90 167 214 220 225 227  ...
 *  74 99 133 146 161 166 202 205 245 262 ...
 *  43 83 94 120 125 183 195 206 244 254  ...
 *  1 13 54 91 92 93 106 140 156 194 208  ...
 *  10 39 67 69 131 144 145 154 168 258   ...
 *  6 52 66 113 118 122 139 147 212 213   ...
 *  8 127 150 182 203 204 249 367 400 432 ...
 *  63 65 101 107 108 136 169 170 171 173 ...
 *  55 71 102 155 159 198 228 252 325 419 ...
 *  4 25 34 58 70 152 172 196 199 210 226 ...
 *  2 44 50 88 109 138 141 178 197 211    ...
 *  57 89 129 162 174 179 188 209 238 276 ...
 *  33 41 49 119 126 132 148 181 215 221  ...
 *  3 18 23 26 35 64 105 124 157 186 251  ...
 *  5 16 17 20 31 47 81 98 158 180 187    ...
 *  24 29 51 59 75 82 100 114 117 134 151 ...
 *  30 45 53 60 72 85 111 130 137 142 163 ...
 *  19 37 62 77 79 110 153 352 353 361    ...
 *  0 68 86 123 165 176 193 239 289 336   ...
 *
 ******************************************************************************/

#include "kosaraju_sharir_scc.h"

#include <exception>
#include <string>
#include <queue>

#include "transitive_closure.h"

using std::to_string;
using std::vector;
using std::queue;

KosarajuSharirSCC::KosarajuSharirSCC(const Digraph& G) noexcept : marked_(G.V()), 
                                                                  id_(G.V()) {
  // compute reverse postorder of reverse graph
  DepthFirstOrder dfs(*G.Reverse());

  // run DFS on G, using reverse postorder to guide calculation
  for (int v : dfs.reversePost()) {
    if (!marked_[v]) {
      Dfs(G, v);
      count_++;
    }
  }
}

bool KosarajuSharirSCC::StronglyConnected(int v, int w) const {
  ValidateVertex(v);
  ValidateVertex(w);
  return id_[v] == id_[w];
}

int KosarajuSharirSCC::id(int v) const {
  ValidateVertex(v);
  return id_[v];
}

void KosarajuSharirSCC::Dfs(Digraph G, int v) {
  marked_[v] = true;
  id_[v] = count_;
  for (int w : G.Adj(v)) {
    if (!marked_[w]) Dfs(G, w);
  }
}

bool KosarajuSharirSCC::Check(const Digraph& G) const {
  TransitiveClosure tc(G);
  for (int v = 0; v < G.V(); v++) {
    for (int w = 0; w < G.V(); w++) {
      if (StronglyConnected(v, w) != (tc.Reachable(v, w) && tc.Reachable(w, v)))
        return false;
    }
  }

  return true;
}

void KosarajuSharirSCC::ValidateVertex(int v) const {
  int V = marked_.size();
  if (v < 0 || v >= V)
    throw std::invalid_argument("vertex " + to_string(v) + 
                                " is not between 0 and " + to_string(V-1));
}

/**
 * Unit tests the {@code KosarajuSharirSCC} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <iostream>
#include <cstdlib>
int main(int argc, char *argv[]) {
  std::fstream in(argv[1]);
  if (!in.is_open()) {
    std::cout << "failed to open " << argv[1] << '\n';
    return 1;
  }

  Digraph G(in);
  KosarajuSharirSCC scc(G);

  // number of connected components
  int m = scc.count();
  std::cout << m << " strong components" << std::endl;

  // compute list of vertices in each strong component
  vector<queue<int>> components(m);
  for (int v = 0; v < G.V(); v++) {
    components[scc.id(v)].push(v);
  }

  // print results
  for (int i = 0; i < m; i++) {
    while (!components[i].empty()) {
      std::cout << components[i].front() << " ";
      components[i].pop();
    }
    std::cout << std::endl;
  }

}
#endif
