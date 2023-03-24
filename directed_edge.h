/******************************************************************************
 *  Compilation:  clang++ -g -DDebug directed_edge.cc -std=c++11 -o directed_edge
 *  Execution:    ./directed_edge
 *  Dependencies: 
 *
 *  Immutable weighted directed edge.
 *
 ******************************************************************************/

/**
 *  The {@code DirectedEdge} class represents a weighted edge in an 
 *  {@link EdgeWeightedDigraph}. Each edge consists of two integers
 *  (naming the two vertices) and a real-value weight. The data type
 *  provides methods for accessing the two endpoints of the directed edge and
 *  the weight.
 *  <p>
 *  For additional documentation, see <a href="https://algs4.cs.princeton.edu/44sp">Section 4.4</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#ifndef DIRECTED_EDGE_H
#define DIRECTED_EDGE_H

#include <string>

class DirectedEdge {
public:
    /**
     * Initializes a directed edge from vertex {@code v} to vertex {@code w} with
     * the given {@code weight}.
     * @param v the tail vertex
     * @param w the head vertex
     * @param weight the weight of the directed edge
     * @throws IllegalArgumentException if either {@code v} or {@code w}
     *    is a negative integer
     * @throws IllegalArgumentException if {@code weight} is {@code NaN}
     */
    DirectedEdge(int v, int w, double weight) noexcept;
    DirectedEdge() = delete;
    DirectedEdge(const DirectedEdge& other) = default;
    DirectedEdge &operator=(const DirectedEdge& other) = default;
    DirectedEdge(DirectedEdge&& other) = default;
    DirectedEdge &operator=(DirectedEdge&& other) = default;
    /**
     * Returns the tail vertex of the directed edge.
     * @return the tail vertex of the directed edge
     */
    int from() const { return _v; }
    /**
     * Returns the head vertex of the directed edge.
     * @return the head vertex of the directed edge
     */
    int to() const { return _w; }
    /**
     * Returns the weight of the directed edge.
     * @return the weight of the directed edge
     */
    double weight() const { return _weight; }
    /**
     * Returns a string representation of the directed edge.
     * @return a string representation of the directed edge
     */
    std::string toString() const;
private:
    int _v;
    int _w;
    double _weight;
};

#endif
