/******************************************************************************
 *  Compilation:  clang++ -c -O2 digraph.cc -std=c++20
 *                clang++ -DDebug -O2 depth_first_directed_paths.cc  digraph.o -std=c++20 -o depth_first_directed_paths
 *  Execution:    ./depth_first_directed_paths tinyDG.txt s
 *  Data files:   https://algs4.cs.princeton.edu/42digraph/tinyDG.txt
 *                https://algs4.cs.princeton.edu/42digraph/mediumDG.txt
 *                https://algs4.cs.princeton.edu/42digraph/largeDG.txt
 *
 *  Determine reachability in a digraph from a given vertex using
 *  depth-first search.
 *  Runs in O(E + V) time.
 *
 *  % ./depth_first_directed_paths tinyDG.txt 3
 *  3 to 0:  3-5-4-2-0
 *  3 to 1:  3-5-4-2-0-1
 *  3 to 2:  3-5-4-2
 *  3 to 3:  3
 *  3 to 4:  3-5-4
 *  3 to 5:  3-5
 *  3 to 6:  not connected
 *  3 to 7:  not connected
 *  3 to 8:  not connected
 *  3 to 9:  not connected
 *  3 to 10:  not connected
 *  3 to 11:  not connected
 *  3 to 12:  not connected
 *
 ******************************************************************************/

/**
 *  The {@code DepthFirstDirectedPaths} class represents a data type for
 *  finding directed paths from a source vertex <em>s</em> to every
 *  other vertex in the digraph.
 *  <p>
 *  This implementation uses depth-first search.
 *  The constructor takes &Theta;(<em>V</em> + <em>E</em>) time in the
 *  worst case, where <em>V</em> is the number of vertices and <em>E</em>
 *  is the number of edges.
 *  Each instance method takes &Theta;(1) time.
 *  It uses &Theta;(<em>V</em>) extra space (not including the digraph).
 *  <p>
 *  See {@link DepthFirstDirectedPaths} for a nonrecursive implementation.
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/42digraph">Section 4.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#include "depth_first_directed_paths.h"

#include <exception>
#include <string>

using std::stack;

void DepthFirstDirectedPaths::dfs(const Digraph& G, int v) {
    marked_[v] = true;
    for (int w : G.adj(v)) {
        if (!marked_[w]) {
            edgeTo_[w] = v;
            dfs(G, w);
        }
    }
}

stack<int> DepthFirstDirectedPaths::pathTo(int v) const {
    validateVertex(v);
    stack<int> path;
    if (!hasPathTo(v)) return path;

    for (int x = v; x != s_; x = edgeTo_[x])
        path.push(x);
    path.push(s_);

    return path;
}

void DepthFirstDirectedPaths::validateVertex(int v) const {
    int V = marked_.size();
    if (v < 0 || v >= V)
        throw std::invalid_argument("vertex " + std::to_string(v) + " is not between 0 and " + std::to_string(V-1));
}

/**
 * Unit tests the {@code DepthFirstDirectedPaths} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <fstream>
#include <iostream>
#include <cstdlib>
int main(int argc, char *argv[]) {
    std::fstream in(argv[1]);
    if (!in.is_open()) {
        std::cout << "failed to open " << argv[1] << '\n';
        return 1;
    }

    Digraph G(in);
    // StdOut.println(G);

    int s = strtol(argv[2], nullptr, 10);
    DepthFirstDirectedPaths dfs(G, s);

    for (int v = 0; v < G.V(); ++v) {
        if (dfs.hasPathTo(v)) {
            std::cout << s << " to " << v << ":  ";
            stack<int> data = dfs.pathTo(v);
            while (!data.empty()) {
                if (data.top() == s) std::cout << data.top();
                else std::cout << "-" << data.top();
                data.pop();
            }
            std::cout << std::endl;
        } else {
            std::cout << s << " to " << v << ":  not connected" << std::endl;
        }
    }

    return 0;
}
#endif
