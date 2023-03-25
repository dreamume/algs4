/******************************************************************************
 *  Compilation:  clang++ -c directed_edge.cc -std=c++20
 *                clang++ -g -DDebug edge_weighted_digraph.cc directed_edge.o -std=c++20 -o edge_weighted_digraph
 *  Execution:    ./edge_weighted_digraph input.txt
 *  Dependencies: directed_edge.h
 *  Data files:   https://algs4.cs.princeton.edu/44st/tinyEWD.txt
 *                https://algs4.cs.princeton.edu/44st/mediumEWD.txt
 *                https://algs4.cs.princeton.edu/44st/largeEWD.txt
 *
 *  An edge-weighted digraph, implemented using adjacency lists.
 *
 ******************************************************************************/

/**
 *  The {@code EdgeWeightedDigraph} class represents a edge-weighted
 *  digraph of vertices named 0 through <em>V</em> - 1, where each
 *  directed edge is of type {@link DirectedEdge} and has a real-valued weight.
 *  It supports the following two primary operations: add a directed edge
 *  to the digraph and iterate over all of edges incident from a given vertex.
 *  It also provides
 *  methods for returning the number of vertices <em>V</em> and the number
 *  of edges <em>E</em>. Parallel edges and self-loops are permitted.
 *  <p>
 *  This implementation uses an adjacency-lists representation, which 
 *  is a vertex-indexed array of {@link Bag} objects.
 *  All operations take constant time (in the worst case) except
 *  iterating over the edges incident from a given vertex, which takes
 *  time proportional to the number of such edges.
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/44sp">Section 4.4</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#include "edge_weighted_digraph.h"

#include <unordered_set>
#include <exception>
#include <vector>
#include <fstream>

#include "directed_edge.h"

using std::vector;
using std::string;
using std::fstream;
using std::unordered_set;

/**
 * Initializes an empty edge-weighted digraph with {@code V} vertices and 0 edges.
 *
 * @param  V the number of vertices
 * @throws IllegalArgumentException if {@code V < 0}
 */
EdgeWeightedDigraph::EdgeWeightedDigraph(int V) noexcept: v_(V), e_(0), adj_(V), 
                                                          indegree_(V), gen_(rd_()) {
    assert(V > 0);
//        throw std::invalid_argument("Number of vertices in a Digraph must be nonnegative");
}

/**
 * Initializes a random edge-weighted digraph with {@code V} vertices and <em>E</em> edges.
 *
 * @param  V the number of vertices
 * @param  E the number of edges
 * @throws IllegalArgumentException if {@code V < 0}
 * @throws IllegalArgumentException if {@code E < 0}
 */
EdgeWeightedDigraph::EdgeWeightedDigraph(int V, int E) noexcept: EdgeWeightedDigraph(V) {
    assert (E < 0);
//        throw std::invalid_argument("Number of edges in a Digraph must be nonnegative");
    std::uniform_int_distribution<> dis(0, V - 1); 
    std::uniform_int_distribution<> dis100(0, 100 - 1); 
    for (int i = 0; i < E; i++) {
        int v = dis(gen_);
        int w = dis(gen_);
        double weight = 0.01 * dis100(gen_);
        DirectedEdge* e = new DirectedEdge(v, w, weight);
        addEdge(e);
    }
}

/**  
 * Initializes an edge-weighted digraph from the specified input stream.
 * The format is the number of vertices <em>V</em>,
 * followed by the number of edges <em>E</em>,
 * followed by <em>E</em> pairs of vertices and edge weights,
 * with each entry separated by whitespace.
 *
 * @param  in the input stream
 * @throws IllegalArgumentException if the endpoints of any edge are not in prescribed range
 * @throws IllegalArgumentException if the number of vertices or edges is negative
 */
EdgeWeightedDigraph::EdgeWeightedDigraph(fstream& in): gen_(rd_()) {
    string line;
    getline(in, line);
    v_ = stoi(line);
    e_ = 0;

    if (v_ < 0) 
        throw std::invalid_argument("Number of vertices in a Digraph must be nonnegative");

    getline(in, line);
    int E = stoi(line);
    if (E < 0) throw std::invalid_argument("Number of edges must be nonnegative");

    indegree_.resize(v_);
    adj_.resize(v_);
		
    for (int i = 0; i < E; i++) {
        std::getline(in, line);
        const char* str = line.c_str();
        char *end;
        int v = std::strtol(str, &end, 10);
        str = end;
        int w = std::strtol(str, &end, 10);
        validateVertex(v);
        validateVertex(w);

        str = end;
        double weight = std::strtod(str, &end);
        addEdge(new DirectedEdge(v, w, weight));
    }
}

/**
 * Initializes a new edge-weighted digraph that is a deep copy of {@code G}.
 *
 * @param  G the edge-weighted digraph to copy
 */
EdgeWeightedDigraph::EdgeWeightedDigraph(const EdgeWeightedDigraph& G) noexcept : 
  EdgeWeightedDigraph(G.V()) {
    e_ = G.E();
    for (int v = 0; v < G.V(); v++)
        indegree_[v] = G.indegree(v);
    for (int v = 0; v < G.V(); v++)
        adj_[v] = G.adj(v);
}

  // throw an IllegalArgumentException unless {@code 0 <= v < V}
void EdgeWeightedDigraph::validateVertex(int v) const {
    if (v < 0 || v >= v_)
        throw std::invalid_argument("vertex " + std::to_string(v) + 
                                    " is not between 0 and " + std::to_string(v_-1));
}

/**
 * Adds the directed edge {@code e} to this edge-weighted digraph.
 *
 * @param  e the edge
 * @throws IllegalArgumentException unless endpoints of edge are between {@code 0}
 *         and {@code V-1}
 */
void EdgeWeightedDigraph::addEdge(DirectedEdge* e) {
    int v = e->from();
    int w = e->to();
    validateVertex(v);
    validateVertex(w);
    adj_[v].push_back(e);
    ++indegree_[w];
    ++e_;
}

/**
 * Returns the directed edges incident from vertex {@code v}.
 *
 * @param  v the vertex
 * @return the directed edges incident from vertex {@code v} as an Iterable
 * @throws IllegalArgumentException unless {@code 0 <= v < V}
 */
const vector<DirectedEdge *>& EdgeWeightedDigraph::adj(int v) const {
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
int EdgeWeightedDigraph::outdegree(int v) const {
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
int EdgeWeightedDigraph::indegree(int v) const {
    validateVertex(v);
    return indegree_[v];
}

/**
 * Returns all directed edges in this edge-weighted digraph.
 * To iterate over the edges in this edge-weighted digraph, use foreach notation:
 * {@code for (DirectedEdge e : G.edges())}.
 *
 * @return all edges in this edge-weighted digraph, as an iterable
 */
vector<DirectedEdge *> EdgeWeightedDigraph::edges() const {
    vector<DirectedEdge *> list;
    for (int v = 0; v < v_; v++)
        for (DirectedEdge* e : adj(v))
            list.push_back(e);

    return list;
} 

/**
 * Returns a string representation of this edge-weighted digraph.
 *
 * @return the number of vertices <em>V</em>, followed by the number of edges <em>E</em>,
 *         followed by the <em>V</em> adjacency lists of edges
 */
string EdgeWeightedDigraph::toString() const {
    string s = std::to_string(v_) + " " + std::to_string(e_) + "\n";
    for (int v = 0; v < v_; v++) {
        s += std::to_string(v) + ": ";
        for (DirectedEdge* e : adj_[v]) s += e->toString() + "  ";
        s += "\n";
    }

    return s;
}

/**
 * Unit tests the {@code EdgeWeightedDigraph} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
int main(int args, char *argv[]) {
    fstream in(argv[1]);
    EdgeWeightedDigraph G(in);
    printf("%s\n", G.toString().c_str());

    return 0;
}
#endif
