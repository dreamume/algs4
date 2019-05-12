/******************************************************************************
 *  Compilation:  clang++ -c -O2 edge_weighted_digraph.cc -std=c++11
 *                clang++ -c -O2 directed_edge.cc -std=c++11
 *                clang++ -c -O2 digraph.cc -std=c++11
 *                clang++ -c -O2 directed_cycle.cc -std=c++11
 *                clang++ -c -O2 depth_first_order.cc -std=c++11
 *                clang++ -c -O2 edge_weighted_directed_cycle.cc -std=c++11
 *                clang++ -c -O2 topological_sort.cc -std=c++11
 *                clang+ -O2 acyclic_lp.cc edge_weighted_digraph.o directed_cycle.o directed_edge.o depth_first_order.o digraph.o edge_weighted_directed_cycle.o topological_sort.o -std=c++11 -o acyclic_lp
 *  Execution:    cpm input.txt
 *  Dependencies: edge_weighted_digraph.h directed_edge.h topological_sort.h
 *                digraph.h directed_cycle.h depth_first_order.h
 *                edge_weighted_directed_cycle.h acyclic_lp.h
 *  Data files:   https://algs4.cs.princeton.edu/44sp/jobsPC.txt
 *
 *  Critical path method.
 *
 *  % cpm jobsPC.txt
 *   job   start  finish
 *  --------------------
 *     0     0.0    41.0
 *     1    41.0    92.0
 *     2   123.0   173.0
 *     3    91.0   127.0
 *     4    70.0   108.0
 *     5     0.0    45.0
 *     6    70.0    91.0
 *     7    41.0    73.0
 *     8    91.0   123.0
 *     9    41.0    70.0
 *  Finish time:   173.0
 *
 ******************************************************************************/

/**
 *  The {@code CPM} class provides a client that solves the
 *  parallel precedence-constrained job scheduling problem
 *  via the <em>critical path method</em>. It reduces the problem
 *  to the longest-paths problem in edge-weighted DAGs.
 *  It builds an edge-weighted digraph (which must be a DAG)
 *  from the job-scheduling problem specification,
 *  finds the longest-paths tree, and computes the longest-paths
 *  lengths (which are precisely the start times for each job).
 *  <p>
 *  This implementation uses {@link AcyclicLP} to find a longest
 *  path in a DAG.
 *  The running time is proportional to <em>V</em> + <em>E</em>,
 *  where <em>V</em> is the number of jobs and <em>E</em> is the
 *  number of precedence constraints.
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/44sp">Section 4.4</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

  /**
   *  Reads the precedence constraints from standard input
   *  and prints a feasible schedule to standard output.
   *
   * @param args the command-line arguments
   */

#include <cstdio>
#include <fstream>
#include <string>
#include <iostream>

#include "edge_weighted_digraph.h"
#include "acyclic_lp.h"

using namespace std;

int main(int args, char* argv[]) {
  fstream in(argv[1]);

  if (!in.is_open()) {
    cout << "failed to open " << argv[1] << '\n';
	return 1;
  }

  // number of jobs
  string line;
  getline(in, line);
  int n = stoi(line);

  // source and sink
  int source = 2*n;
  int sink   = 2*n + 1;

  // build network
  EdgeWeightedDigraph G(2*n + 2);
  for (int i = 0; i < n; i++) {
	getline(in, line);
	size_t pos = 0;
	double duration = stod(line, &pos);
	G.addEdge(new DirectedEdge(source, i, 0.0));
	G.addEdge(new DirectedEdge(i+n, sink, 0.0));
	G.addEdge(new DirectedEdge(i, i+n, duration));

	// precedence constraints
	if (pos == string::npos) continue;
	++pos;
	line = line.substr(pos);
	int m = stoi(line, &pos);
	if (pos == string::npos) continue;
	for (int j = 0; j < m; j++) {
	  ++pos;
	  line = line.substr(pos);
	  int precedent = stoi(line, &pos);
	  G.addEdge(new DirectedEdge(n+i, precedent, 0.0));
	}
  }

  // compute longest path
  AcyclicLP lp(G, source);

  // print results
  printf(" job   start  finish\n");
  printf("--------------------\n");
  for (int i = 0; i < n; i++) {
	printf("%4d %7.1f %7.1f\n", i, lp.distTo(i), lp.distTo(i+n));
  }
  printf("Finish time: %7.1f\n", lp.distTo(sink));
}
