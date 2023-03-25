/******************************************************************************
 *  Compilation:  clang++ -c -O2 directed_edge.cc -std=c++20
 *                clang++ -c -O2 edge_weighted_digraph.cc -std=c++20
 *                clang++ -O2 -DDebug edge_weighted_directed_cycle.cc directed_edge.o edge_weighted_digraph.o -std=c++20 -o edge_weighted_directed_cycle
 *  Execution:    ./edge_weighted_directed_cycle V E F
 *  Dependencies: edge_weighted_digraph.h directed_edge.h
 *
 *  Finds a directed cycle in an edge-weighted digraph.
 *  Runs in O(E + V) time.
 *
 *
 ******************************************************************************/

/**
 *  The {@code EdgeWeightedDirectedCycle} class represents a data type for 
 *  determining whether an edge-weighted digraph has a directed cycle.
 *  The <em>hasCycle</em> operation determines whether the edge-weighted
 *  digraph has a directed cycle and, if so, the <em>cycle</em> operation
 *  returns one.
 *  <p>
 *  This implementation uses depth-first search.
 *  The constructor takes time proportional to <em>V</em> + <em>E</em>
 *  (in the worst case),
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the <em>hasCycle</em> operation takes constant time;
 *  the <em>cycle</em> operation takes time proportional
 *  to the length of the cycle.
 *  <p>
 *  See {@link Topological} to compute a topological order if the edge-weighted
 *  digraph is acyclic.
 *  <p>
 *  For additional documentation,   
 *  see <a href="https://algs4.cs.princeton.edu/44sp">Section 4.4</a> of   
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne. 
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#include "edge_weighted_directed_cycle.h"

#include <cstdio>
#include <cassert>

#include "directed_edge.h"

using std::vector;
using std::stack;

/**
 * Determines whether the edge-weighted digraph {@code G} has a directed cycle and,
 * if so, finds such a cycle.
 * @param G the edge-weighted digraph
 */
EdgeWeightedDirectedCycle::EdgeWeightedDirectedCycle(const EdgeWeightedDigraph& G) noexcept {
    marked_.resize(G.V());
    on_stack_.resize(G.V());
    edge_to_.resize(G.V());
    for (int v = 0; v < G.V(); v++)
        if (!marked_[v]) dfs(G, v);

    // check that digraph has a cycle
    //  assert(check());
}

// check that algorithm computes either the topological order or finds a directed cycle
void EdgeWeightedDirectedCycle::dfs(const EdgeWeightedDigraph& G, int v) {
    on_stack_[v] = true;
    marked_[v] = true;
    for (DirectedEdge* e : G.adj(v)) {
        int w = e->to();

        if (!cycle_.empty()) {
		  // short circuit if directed cycle found
		  return;
		} else if (!marked_[w]) {
		  // found new vertex, so recur
            edge_to_[w] = e;
            dfs(G, w);
        } else if (on_stack_[w]) {
		  // trace back directed cycle
            DirectedEdge* f = e;
            while (f->from() != w) {
                cycle_.push(f);
                f = edge_to_[f->from()];
            }
            cycle_.push(f);

            return;
        }
    }

    on_stack_[v] = false;
}

// certify that digraph is either acyclic or has a directed cycle
bool EdgeWeightedDirectedCycle::check() const {
    // edge-weighted digraph is cyclic
    if (hasCycle()) {
        // verify cycle
        DirectedEdge* first = nullptr, *last = nullptr;
        stack<DirectedEdge *> the_cycle(cycle());
        while (!the_cycle.empty()) {
            if (!first) first = the_cycle.top();
            if (last) {
                if (last->to() != the_cycle.top()->from()) {
                    printf("cycle edges %s and %s not incident\n", 
                           last->toString().c_str(), the_cycle.top()->toString().c_str());
                    return false;
                }
            }
            last = the_cycle.top();
        }

        if (last->to() != first->from()) {
            printf("cycle edges %s and %s not incident\n", last->toString().c_str(),
                   first->toString().c_str());
            return false;
        }
    }

    return true;
}

/**
 * Unit tests the {@code EdgeWeightedDirectedCycle} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <random>
#include <algorithm>

int main(int argc, char *argv[]) {
    // create random DAG with V vertices and E edges; then add F random edges
    int V = std::stoi(argv[1]);
    int E = std::stoi(argv[2]);
    int F = std::stoi(argv[3]);
    EdgeWeightedDigraph G(V);
    vector<int> vertices(V, 0);
    for (int i = 0; i < V; i++)
        vertices[i] = i;
    std::random_shuffle(vertices.begin(), vertices.end() );

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, V - 1); 
    std::uniform_real_distribution<> dis_real(0, 1.0);

    for (int i = 0; i < E; i++) {
        int v, w;
        do {
            v = dis(gen);
            w = dis(gen);
        } while (v >= w);
        double weight = dis_real(gen);
        G.addEdge(new DirectedEdge(v, w, weight));
    }

    // add F extra edges
    for (int i = 0; i < F; i++) {
        int v = dis(gen);
        int w = dis(gen);
        double weight = dis_real(gen);
        G.addEdge(new DirectedEdge(v, w, weight));
    }

    printf("%s\n", G.toString().c_str());

    // find a directed cycle
    EdgeWeightedDirectedCycle finder(G);
    if (finder.hasCycle()) {
        printf("Cycle: ");
        stack<DirectedEdge *> the_cycle(finder.cycle());
        while (!the_cycle.empty()) {
            printf("%s ", the_cycle.top()->toString().c_str());
        }
        printf("\n");
    }

    // or give topologial sort
    else {
        printf("No directed cycle\n");
    }
}
#endif

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
