/******************************************************************************
 *  Compilation:  clang++ -c -O2 digraph.cc -std=c++20
 *                clang++ -c -O2 directed_edge.cc -std=c++20
 *                clang++ -c -O2 edge_weighted_digraph.cc -std=c++20
 *                clang++ -c -O2 depth_first_order.cc -std=c++20
 *                clang++ -c -O2 directed_cycle.cc -std=c++20
 *                clang++ -c -O2 edge_weighted_directed_cycle.cc -std=c++20
 *                clang++ -c -O2 symbol_digraph.cc -std=c++20
 *                clang++ -DDebug -O2 topological_sort.cc digraph.o directed_edge.o edge_weighted_digraph.o depth_first_order.o directed_cycle.o edge_weighted_directed_cycle.o symbol_digraph.o -std=c++20 -o topological_sort
 *  Execution:    ./topological filename.txt delimiter
 *  Dependencies: digraph.h depth_first_order.h directed_cycle.h
 *                depth_first_order.h edge_weighted_directed_cycle.h
 *                symbol_digraph.h
 *  Data files:   https://algs4.cs.princeton.edu/42digraph/jobs.txt
 *
 *  Compute topological ordering of a DAG or edge-weighted DAG.
 *  Runs in O(E + V) time.
 *
 *  % java Topological jobs.txt "/"
 *  Calculus
 *  Linear Algebra
 *  Introduction to CS
 *  Advanced Programming
 *  Algorithms
 *  Theoretical CS
 *  Artificial Intelligence
 *  Robotics
 *  Machine Learning
 *  Neural Networks
 *  Databases
 *  Scientific Computing
 *  Computational Biology
 *
 ******************************************************************************/

/**
 *  The {@code Topological} class represents a data type for 
 *  determining a topological order of a directed acyclic graph (DAG).
 *  Recall, a digraph has a topological order if and only if it is a DAG.
 *  The <em>hasOrder</em> operation determines whether the digraph has
 *  a topological order, and if so, the <em>order</em> operation
 *  returns one.
 *  <p>
 *  This implementation uses depth-first search.
 *  The constructor takes time proportional to <em>V</em> + <em>E</em>
 *  (in the worst case),
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the <em>hasOrder</em> and <em>rank</em> operations takes constant time;
 *  the <em>order</em> operation takes time proportional to <em>V</em>.
 *  <p>
 *  See {@link DirectedCycle}, {@link DirectedCycleX}, and
 *  {@link EdgeWeightedDirectedCycle} to compute a
 *  directed cycle if the digraph is not a DAG.
 *  See {@link TopologicalX} for a nonrecursive queue-based algorithm
 *  to compute a topological order of a DAG.
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/42digraph">Section 4.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#include "topological_sort.h"

#include <exception>

#include "directed_cycle.h"
#include "depth_first_order.h"
#include "symbol_digraph.h"
#include "edge_weighted_directed_cycle.h"

using std::vector;
using std::string;

/**
 * Determines whether the digraph {@code G} has a topological order and, if so,
 * finds such a topological order.
 * @param G the digraph
 */
Topological::Topological(const Digraph& G) noexcept {
    DirectedCycle finder(G);
    if (!finder.hasCycle()) {
        DepthFirstOrder dfs(G);
        order_ = dfs.reversePost();
        rank_.resize(G.V());
        int i = 0;
        for (int v : order_)
            rank_[v] = i++;
    }
}

/**
 * Determines whether the edge-weighted digraph {@code G} has a topological
 * order and, if so, finds such an order.
 * @param G the edge-weighted digraph
 */
Topological::Topological(const EdgeWeightedDigraph& G) noexcept {
    EdgeWeightedDirectedCycle finder(G);
    if (!finder.hasCycle()) {
        DepthFirstOrder dfs(G);
        order_ = dfs.reversePost();
    }
}

/**
 * The the rank of vertex {@code v} in the topological order;
 * -1 if the digraph is not a DAG
 *
 * @param v the vertex
 * @return the position of vertex {@code v} in a topological order
 *    of the digraph; -1 if the digraph is not a DAG
 * @throws IllegalArgumentException unless {@code 0 <= v < V}
 */
int Topological::rank(int v) const {
    validateVertex(v);
    if (hasOrder()) return rank_[v];
    else            return -1;
}

// throw an IllegalArgumentException unless {@code 0 <= v < V}
void Topological::validateVertex(int v) const {
    int V = rank_.size();
    if (v < 0 || v >= V)
        throw std::invalid_argument("vertex " + std::to_string(v) + 
                                    " is not between 0 and " + std::to_string(V-1));
}

/**
 * Unit tests the {@code Topological} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
int main(int argc, char *argv[]) {
    string filename  = argv[1];
    string delimiter{" "};
    if (argc > 2) delimiter = argv[2];
    SymbolDigraph sg(argv[1], delimiter);
    Topological topological(*sg.digraph());
    for (int v : topological.order()) {
        printf("%s\n", sg.nameOf(v).c_str());
    }
}
#endif
