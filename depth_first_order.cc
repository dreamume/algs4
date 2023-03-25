/******************************************************************************
 *  Compilation:  clang++ -c -O2 digraph.cc -std=c++20
 *                clang++ -c -O2 directed_edge.cc -std=c++20
 *                clang++ -c -O2 edge_weighted_digraph.cc -std=c++20
 *                clang++ -O2 -DDebug depth_first_order.cc digraph.o edge_weighted_digraph.o directed_edge.o -std=c++20 -o depth_first_order
 *  Execution:    ./depth_first_order digraph.txt
 *  Dependencies: digraph.h
 *                edge_weighted_digraph.h directed_edge.h
 *  Data files:   https://algs4.cs.princeton.edu/42digraph/tinyDAG.txt
 *                https://algs4.cs.princeton.edu/42digraph/tinyDG.txt
 *
 *  Compute preorder and postorder for a digraph or edge-weighted digraph.
 *  Runs in O(E + V) time.
 *
 *  % java DepthFirstOrder tinyDAG.txt
 *     v  pre post
 *  --------------
 *     0    0    8
 *     1    3    2
 *     2    9   10
 *     3   10    9
 *     4    2    0
 *     5    1    1
 *     6    4    7
 *     7   11   11
 *     8   12   12
 *     9    5    6
 *    10    8    5
 *    11    6    4
 *    12    7    3
 *  Preorder:  0 5 4 1 6 9 11 12 10 2 3 7 8 
 *  Postorder: 4 5 1 12 11 10 9 6 0 3 2 7 8 
 *  Reverse postorder: 8 7 2 3 0 6 9 10 11 12 1 5 4 
 *
 ******************************************************************************/

/**
 *  The {@code DepthFirstOrder} class represents a data type for 
 *  determining depth-first search ordering of the vertices in a digraph
 *  or edge-weighted digraph, including preorder, postorder, and reverse postorder.
 *  <p>
 *  This implementation uses depth-first search.
 *  The constructor takes time proportional to <em>V</em> + <em>E</em>
 *  (in the worst case),
 *  where <em>V</em> is the number of vertices and <em>E</em> is the number of edges.
 *  Afterwards, the <em>preorder</em>, <em>postorder</em>, and <em>reverse postorder</em>
 *  operation takes take time proportional to <em>V</em>.
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/42digraph">Section 4.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#include "depth_first_order.h"

#include <cstdio>
#include <cassert>
#include <stack>
#include <algorithm>

#include "directed_edge.h"

using std::queue;
using std::vector;
using std::stack;

/**
 * Determines a depth-first order for the digraph {@code G}.
 * @param G the digraph
 */
DepthFirstOrder::DepthFirstOrder(const Digraph& G) noexcept {
    pre_.resize(G.V());
    post_.resize(G.V());
    marked_.resize(G.V());
    for (int v = 0; v < G.V(); v++)
        if (!marked_[v]) dfs(G, v);

    assert(check());
}

/**
 * Determines a depth-first order for the edge-weighted digraph {@code G}.
 * @param G the edge-weighted digraph
 */
DepthFirstOrder::DepthFirstOrder(const EdgeWeightedDigraph& G) noexcept {
    pre_.resize(G.V());
    post_.resize(G.V());
    marked_.resize(G.V());
    for (int v = 0; v < G.V(); v++)
        if (!marked_[v]) dfs(G, v);
}

// run DFS in digraph G from vertex v and compute preorder/postorder
void DepthFirstOrder::dfs(const Digraph& G, int v) {
    marked_[v] = true;
    pre_[v] = pre_counter_++;
    preorder_.push(v);
    for (int w : G.adj(v))
        if (!marked_[w]) dfs(G, w);
    postorder_.push(v);
    post_[v] = post_counter_++;
}

// run DFS in edge-weighted digraph G from vertex v and compute preorder/postorder
void DepthFirstOrder::dfs(const EdgeWeightedDigraph& G, int v) {
    marked_[v] = true;
    pre_[v] = pre_counter_++;
    preorder_.push(v);
    for (DirectedEdge* e : G.adj(v)) {
        int w = e->to();
        if (!marked_[w]) dfs(G, w);
    }
    postorder_.push(v);
    post_[v] = post_counter_++;
}

/**
 * Returns the preorder number of vertex {@code v}.
 * @param  v the vertex
 * @return the preorder number of vertex {@code v}
 * @throws IllegalArgumentException unless {@code 0 <= v < V}
 */
int DepthFirstOrder::pre(int v) const {
    validateVertex(v);
    return pre_[v];
}

/**
 * Returns the postorder number of vertex {@code v}.
 * @param  v the vertex
 * @return the postorder number of vertex {@code v}
 * @throws IllegalArgumentException unless {@code 0 <= v < V}
 */
int DepthFirstOrder::post(int v) const {
    validateVertex(v);
    return post_[v];
}

/**
 * Returns the vertices in reverse postorder.
 * @return the vertices in reverse postorder, as an iterable of vertices
 */
vector<int> DepthFirstOrder::reversePost() const {
    // Stack<Integer> reverse = new Stack<Integer>();
    // for (int v : postorder_)
    // 	reverse.push(v);
    // return reverse;
    vector<int> reverse;
    queue<int> post(postorder_);
    while (!post.empty()) {
        reverse.push_back(post.front());
        post.pop();
    }
    std::reverse(reverse.begin(), reverse.end());

    return reverse;
}

// check that pre() and post() are consistent with pre(v) and post(v)
bool DepthFirstOrder::check() const {
    // check that post(v) is consistent with post()
    int r = 0;
    queue<int> postorder(post());
    while (!postorder.empty()) {
        if (post(postorder.front()) != r) {
            printf("post(v) and post() inconsistent\n");
            return false;
        }
        postorder.pop();
        r++;
    }

    // check that pre(v) is consistent with pre()
    r = 0;
    queue<int> preorder(pre());
    while (!preorder.empty()) {
        if (pre(preorder.front()) != r) {
            printf("pre(v) and pre() inconsistent\n");
            return false;
        }
        preorder.pop();
        r++;
    }

    return true;
}

// throw an IllegalArgumentException unless {@code 0 <= v < V}
void DepthFirstOrder::validateVertex(int v) const {
    int V = marked_.size();
    if (v < 0 || v >= V)
        throw std::invalid_argument("vertex " + std::to_string(v) + 
                                    " is not between 0 and " + std::to_string(V-1));
}

/**
 * Unit tests the {@code DepthFirstOrder} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <fstream>
#include <iostream>
int main(int argc, char *argv[]) {
    std::fstream in(argv[1]);
    if (!in.is_open()) {
        std::cout << "failed to open " << argv[1] << '\n';
        return 1;
    }

    Digraph G(in);
    DepthFirstOrder dfs(G);
    printf("   v  pre post\n");
    printf("--------------\n");
    for (int v = 0; v < G.V(); v++) {
        printf("%4d %4d %4d\n", v, dfs.pre(v), dfs.post(v));
    }

    printf("Preorder:  \n");
    queue<int> preorder(dfs.pre());
    while (!preorder.empty()) {
        printf("%d ", preorder.front());
        preorder.pop();
    }
    printf("\n");

    printf("Postorder: ");
    queue<int> postorder(dfs.post());
    while (!postorder.empty()) {
        printf("%d ", postorder.front());
        postorder.pop();
    }
    printf("\n");

    printf("Reverse postorder: ");
    for (int v : dfs.reversePost()) {
        printf("%d ", v);
    }
    printf("\n");

    return 0;
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
