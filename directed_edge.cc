/******************************************************************************
 *  Compilation:  clang++ -g -DDebug directed_edge.cc -std=c++20 -o directed_edge
 *  Execution:    ./directed_edge
 *  Dependencies: 
 *
 *  Immutable weighted directed edge.
 *
 ******************************************************************************/

#include "directed_edge.h"

#include <cassert>

#ifdef Debug
#include <cstdio>
#endif
#include <exception>
#include <limits>

using std::string;

namespace algs4 {
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
DirectedEdge::DirectedEdge(int v, int w, double weight) noexcept: v_(v), w_(w), weight_(weight) {
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
string DirectedEdge::ToString() const {
  return std::to_string(v_) + "->" + std::to_string(w_) + " " + 
    std::to_string(weight_);
}
}

/**
 * Unit tests the {@code DirectedEdge} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
int main(int argc, char* argv[]) {
  DirectedEdge e(12, 34, 5.67);
  printf("%s\n", e.ToString().c_str());
}
#endif
