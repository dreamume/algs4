#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <string>
#include <fstream>

class Graph {
public:    
    /**
     * Initializes an empty graph with {@code V} vertices and 0 edges.
     * param V the number of vertices
     *
     * @param  V number of vertices
     * @throws IllegalArgumentException if {@code V < 0}
     */
    Graph(int V) noexcept : v_(V), adj_(V) {}
    Graph() = delete;
    Graph &operator=(const Graph& other) = default;
    Graph(Graph&& other) = default;
    Graph &operator=(Graph&& other) = default;
    /**
     * Initializes a new graph that is a deep copy of {@code G}.
     *
     * @param  G the graph to copy
     */
    Graph(const Graph& other) noexcept;
    /**  
     * Initializes a graph from the specified input stream.
     * The format is the number of vertices <em>V</em>,
     * followed by the number of edges <em>E</em>,
     * followed by <em>E</em> pairs of vertices, with each entry separated by whitespace.
     *
     * @param  in the input stream
     * @throws IllegalArgumentException if the endpoints of any edge are not in prescribed range
     * @throws IllegalArgumentException if the number of vertices or edges is negative
     * @throws IllegalArgumentException if the input stream is in the wrong format
     */
    Graph(std::fstream& in);
    /**
     * Returns the number of vertices in this graph.
     *
     * @return the number of vertices in this graph
     */
    int V() const { return v_; }

    /**
     * Returns the number of edges in this graph.
     *
     * @return the number of edges in this graph
     */
    int E() const { return e_; }
    /**
     * Adds the undirected edge v-w to this graph.
     *
     * @param  v one vertex in the edge
     * @param  w the other vertex in the edge
     * @throws IllegalArgumentException unless both {@code 0 <= v < V} and {@code 0 <= w < V}
     */
    void addEdge(int v, int w);
    /**
     * Returns the vertices adjacent to vertex {@code v}.
     *
     * @param  v the vertex
     * @return the vertices adjacent to vertex {@code v}, as an iterable
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    const std::vector<int>& adj(int v) const;
    /**
     * Returns the degree of vertex {@code v}.
     *
     * @param  v the vertex
     * @return the degree of vertex {@code v}
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    int degree(int v) const;
    /**
     * Returns a string representation of this graph.
     *
     * @return the number of vertices <em>V</em>, followed by the number of edges <em>E</em>,
     *         followed by the <em>V</em> adjacency lists
     */
    std::string toString() const;
private:
    // throw an IllegalArgumentException unless {@code 0 <= v < V}
    void validateVertex(int v) const;
private:
    int v_;
    int e_{0};
    std::vector<std::vector<int>> adj_;
};

#endif  // GRAPH_H_
