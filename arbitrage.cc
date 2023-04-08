/******************************************************************************
 *  Compilation:  clang++ -c -O2 directed_edge.cc -std=c++11
 *                clang++ -c -O2 edge_weighted_digraph.cc -std=c++11
 *                clang++ -c -O2 edge_weighted_directed_cycle.cc -std=c++11
 *                clang++ -c -O2 bellman_ford_sp.cc -std=c++11
 *                clang++ -O2 -DDebug arbitrage.cc bellman_ford_sp.o edge_weighted_directed_cycle.o edge_weighted_digraph.o directed_edge.o -std=c++11 -o arbitrage
 *  Execution:    arbitrage input.txt
 *  Dependencies: edge_weighted_digraph.h directed_edge.h
 *                bellman_ford_sp.h edge_weighted_directed_cycle.h
 *  Data file:    https://algs4.cs.princeton.edu/44sp/rates.txt
 *
 *  Arbitrage detection.
 *
 *  % more rates.txt
 *  5
 *  USD 1      0.741  0.657  1.061  1.005
 *  EUR 1.349  1      0.888  1.433  1.366
 *  GBP 1.521  1.126  1      1.614  1.538
 *  CHF 0.942  0.698  0.619  1      0.953
 *  CAD 0.995  0.732  0.650  1.049  1
 *
 *  % arbitrage rates.txt
 *  1000.00000 USD =  741.00000 EUR
 *   741.00000 EUR = 1012.20600 CAD
 *  1012.20600 CAD = 1007.14497 USD
 *
 ******************************************************************************/

/**
 *  The {@code Arbitrage} class provides a client that finds an arbitrage
 *  opportunity in a currency exchange table by constructing a
 *  complete-digraph representation of the exchange table and then finding
 *  a negative cycle in the digraph.
 *  <p>
 *  This implementation uses the Bellman-Ford algorithm to find a
 *  negative cycle in the complete digraph.
 *  The running time is proportional to <em>V</em><sup>3</sup> in the
 *  worst case, where <em>V</em> is the number of currencies.
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/44sp">Section 4.4</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

/**
 *  Reads the currency exchange table from standard input and
 *  prints an arbitrage opportunity to standard output (if one exists).
 *
 * @param args the command-line arguments
 */
#include <random>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stack>
#include <fstream>
#include <cmath>

#include "directed_edge.h"
#include "edge_weighted_digraph.h"
#include "bellman_ford_sp.h"

using std::string;
using std::vector;
using std::stack;
using std::fstream;
using namespace algs4;

int main(int argc, char *argv[]) {
  // V currencies
  fstream in(argv[1]);
  std::string line;
  std::getline(in, line);
  int V = std::stoi(line);
  vector<string> name(V, string());

  // create complete network
  EdgeWeightedDigraph G(V);
  for (int v = 0; v < V; v++) {
	std::getline(in, line);
	string::size_type pos = line.find(" ");
	if (pos == string::npos) continue;
	name[v] = line.substr(0, pos);
	const char* str = line.c_str() + pos;
	for (int w = 0; w < V; w++) {
	  char* end;
	  double rate = std::strtod(str, &end);
	  DirectedEdge* e = new DirectedEdge(v, w, -std::log(rate));
	  G.addEdge(e);
	  str = end;
	}
  }

  // find negative cycle
  BellmanFordSP spt(G, 0);
  if (spt.hasNegativeCycle()) {
	double stake = 1000.0;
	stack<DirectedEdge *> edges(spt.negativeCycle());
	while (!edges.empty()) {
	  printf("%10.5f %s ", stake, name[edges.top()->from()].c_str());
	  stake *= std::exp(-edges.top()->weight());
	  printf("= %10.5f %s\n", stake, name[edges.top()->to()].c_str());
	  edges.pop();
	}
  } else {
	printf("No arbitrage opportunity\n");
  }

  return 0;
}

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
