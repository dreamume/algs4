/******************************************************************************
 *  Compilation:  clang++ -DDebug -O2 digraph.cc -std=c++20 -o digraph
 *  Execution:    ./digraph filename.txt
 *  Dependencies: 
 *  Data files:   https://algs4.cs.princeton.edu/42digraph/tinyDG.txt
 *                https://algs4.cs.princeton.edu/42digraph/mediumDG.txt
 *                https://algs4.cs.princeton.edu/42digraph/largeDG.txt  
 *
 *  A graph, implemented using an array of lists.
 *  Parallel edges and self-loops are permitted.
 *
 *  % java Digraph tinyDG.txt
 *  13 vertices, 22 edges
 *  0: 5 1 
 *  1: 
 *  2: 0 3 
 *  3: 5 2 
 *  4: 3 2 
 *  5: 4 
 *  6: 9 4 8 0 
 *  7: 6 9
 *  8: 6 
 *  9: 11 10 
 *  10: 12 
 *  11: 4 12 
 *  12: 9 
 *  
 ******************************************************************************/

/**
 *  The {@code Digraph} class represents a directed graph of vertices
 *  named 0 through <em>V</em> - 1.
 *  It supports the following two primary operations: add an edge to the digraph,
 *  iterate over all of the vertices adjacent from a given vertex.
 *  Parallel edges and self-loops are permitted.
 *  <p>
 *  This implementation uses an adjacency-lists representation, which 
 *  is a vertex-indexed array of {@link Bag} objects.
 *  All operations take constant time (in the worst case) except
 *  iterating over the vertices adjacent from a given vertex, which takes
 *  time proportional to the number of such vertices.
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/42digraph">Section 4.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#include "digraph.h"

#include <exception>

using std::vector;
using std::string;
using std::fstream;

/**
 * Initializes an empty digraph with <em>V</em> vertices.
 *
 * @param  V the number of vertices
 * @throws IllegalArgumentException if {@code V < 0}
 */
Digraph::Digraph(int V): v_(V) {
    if (V < 0) 
        throw std::invalid_argument("Number of vertices in a Digraph must be nonnegative");
    indegree_.resize(V);
    adj_.resize(V);
}

/**  
 * Initializes a digraph from the specified input stream.
 * The format is the number of vertices <em>V</em>,
 * followed by the number of edges <em>E</em>,
 * followed by <em>E</em> pairs of vertices, with each entry separated by whitespace.
 *
 * @param  in the input stream
 * @throws IllegalArgumentException if the endpoints of any edge are not in prescribed range
 * @throws IllegalArgumentException if the number of vertices or edges is negative
 * @throws IllegalArgumentException if the input stream is in the wrong format
 */
Digraph::Digraph(fstream& in) {
    try {
        string line;
        std::getline(in, line);
        v_ = stoi(line);

        if (v_ < 0) 
            throw std::invalid_argument("number of vertices in a Digraph must be nonnegative");
        indegree_.resize(v_);
        adj_.resize(v_);
        std::getline(in, line);
        int E = stoi(line);
        if (E < 0) 
            throw std::invalid_argument("number of edges in a Digraph must be nonnegative");
        for (int i = 0; i < E; i++) {
            std::getline(in, line);
            const char* str = line.c_str();
            char *end;
            int v = std::strtol(str, &end, 10);
            str = end;
            int w = std::strtol(str, &end, 10);
            addEdge(v, w); 
        }
    }
    catch (...) {
        throw;
    }
}

/**
 * Initializes a new digraph that is a deep copy of the specified digraph.
 *
 * @param  G the digraph to copy
 */
Digraph::Digraph(const Digraph& G) noexcept : Digraph(G.V()) {
    e_ = G.E();
    indegree_.resize(v_);
    for (int i = 0; i < v_; ++i) indegree_[i] = G.indegree(i);
    for (int v = 0; v < G.V(); v++) adj_[v] = G.adj(v);
}

// throw an IllegalArgumentException unless {@code 0 <= v < V}
void Digraph::validateVertex(int v) const {
    if (v < 0 || v >= v_)
        throw std::invalid_argument("vertex " + std::to_string(v) + 
                                    " is not between 0 and " + std::to_string(v_-1));
}

/**
 * Adds the directed edge v→w to this digraph.
 *
 * @param  v the tail vertex
 * @param  w the head vertex
 * @throws IllegalArgumentException unless both {@code 0 <= v < V} and {@code 0 <= w < V}
 */
void Digraph::addEdge(int v, int w) {
    validateVertex(v);
    validateVertex(w);
    adj_[v].push_back(w);
    ++indegree_[w];
    ++e_;
}

/**
 * Returns the vertices adjacent from vertex {@code v} in this digraph.
 *
 * @param  v the vertex
 * @return the vertices adjacent from vertex {@code v} in this digraph, as an iterable
 * @throws IllegalArgumentException unless {@code 0 <= v < V}
 */
const vector<int>& Digraph::adj(int v) const {
    validateVertex(v);
    return adj_[v];
}

/**
 * Returns the number of directed edges incident from vertex {@code v}.
 * This is known as the <em>outdegree</em> of vertex {@code v}.
 *
 * @param  v the vertex
 * @return the outdegree of vertex {@code v}               
 * @throws IllegalArgumentException unless {@code 0 <= v < V}
 */
int Digraph::outdegree(int v) const {
    validateVertex(v);
    return adj_[v].size();
}

/**
 * Returns the number of directed edges incident to vertex {@code v}.
 * This is known as the <em>indegree</em> of vertex {@code v}.
 *
 * @param  v the vertex
 * @return the indegree of vertex {@code v}               
 * @throws IllegalArgumentException unless {@code 0 <= v < V}
 */
int Digraph::indegree(int v) const {
    validateVertex(v);
    return indegree_[v];
}

/**
 * Returns the reverse of the digraph.
 *
 * @return the reverse of the digraph
 */
Digraph* Digraph::reverse() const {
    Digraph* reverse = new Digraph(v_);
    for (int v = 0; v < v_; v++)
        for (int w : adj(v))
            reverse->addEdge(w, v);

    return reverse;
}

/**
 * Returns a string representation of the graph.
 *
 * @return the number of vertices <em>V</em>, followed by the number of edges <em>E</em>,  
 *         followed by the <em>V</em> adjacency lists
 */
string Digraph::toString() const {
    string s = 
        std::to_string(v_) + " vertices, " + std::to_string(e_) + " edges \n";
    for (int v = 0; v < v_; v++) {
        s += std::to_string(v) + ": ";
        for (int w : adj_[v]) s += std::to_string(w) + " ";
        s += "\n";
    }

    return s;
}

/**
 * Unit tests the {@code Digraph} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <iostream>
int main(int args, char *argv[]) {
    fstream in(argv[1]);
    if (!in.is_open()) {
        std::cout << "failed to open " << argv[1] << '\n';
        return 1;
    }

    Digraph G(in);
    printf("%s\n", G.toString().c_str());
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
