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

#include "directed_edge.h"

#include <cassert>

#ifdef Debug
#include <cstdio>
#endif
#include <exception>
#include <limits>

using std::string;

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
DirectedEdge::DirectedEdge(int v, int w, double weight) noexcept: _v(v), _w(w), _weight(weight) {
    assert (v >= 0);
//        throw std::invalid_argument("Vertex names must be nonnegative integers");
    assert (w >= 0);
//        throw std::invalid_argument("Vertex names must be nonnegative integers");
    assert (weight != std::numeric_limits<double>::quiet_NaN());
//        throw std::invalid_argument("Weight is NaN");
}

/**
 * Returns a string representation of the directed edge.
 * @return a string representation of the directed edge
 */
string DirectedEdge::toString() const {
    return std::to_string(_v) + "->" + std::to_string(_w) + " " + 
        std::to_string(_weight);
}

/**
 * Unit tests the {@code DirectedEdge} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
int main(int argc, char* argv[]) {
    DirectedEdge e(12, 34, 5.67);
    printf("%s\n", e.toString().c_str());
}
#endif
