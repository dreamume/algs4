/******************************************************************************
 *  Compilation:  clang++ -DDebug -O2 edge.cc -std=c++20 -o edge
 *  Execution:    ./edge
 *  Dependencies: 
 *
 *  Immutable weighted edge.
 *
 ******************************************************************************/

#include "edge.h"

#include <cassert>
#include <exception>

using std::string;
using std::to_string;

namespace algs4 {

[[nodiscard]] std::strong_ordering Edge::operator<=>(const Edge& other) const {
  //  return std::strong_order(weight_, other.weight());
  //return compare_strong_order_fallback(weight_, other.weight());
  auto cmp2 = weight_ <=> other.weight(); // might be partial_ordering for double
// map partial_ordering to strong_ordering:
  assert(cmp2 != std::partial_ordering::unordered); // RUNTIME ERROR if unordered 
  return cmp2 == 0 ? std::strong_ordering::equal
    : cmp2 > 0 ? std::strong_ordering::greater
    : std::strong_ordering::less;
}

int Edge::other(int vertex) const {
  if      (vertex == v_) return w_;
  else if (vertex == w_) return v_;
  else throw std::invalid_argument("Illegal endpoint");
}

string Edge::ToString() const {
  return to_string(v_) + '-' + to_string(w_) + ' ' + to_string(weight_);
}
}

/**
 * Unit tests the {@code Edge} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <iostream>
using namespace algs4;
int main(int argc, char *argv[]) {
  Edge e(12, 34, 5.67);
  Edge f(13, 35, 5.68);

  std::cout << e.ToString() << " compare " << f.ToString() << " is " << 
    (e < f) << std::endl;

  return 0;
}
#endif
