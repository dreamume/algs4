/******************************************************************************
 *  Compilation:  clang++ -DDebug -O2 edge.cc -std=c++20 -o edge
 *  Execution:    ./edge
 *  Dependencies: 
 *
 *  Immutable weighted edge.
 *
 ******************************************************************************/

#include "edge.h"

#include <exception>

using std::string;
using std::to_string;

bool operator<(const Edge& left, const Edge& right) {
  return left.weight() < right.weight();
}

int Edge::other(int vertex) const {
  if      (vertex == v_) return w_;
  else if (vertex == w_) return v_;
  else throw std::invalid_argument("Illegal endpoint");
}

string Edge::ToString() const {
  return to_string(v_) + '-' + to_string(w_) + ' ' + to_string(weight_);
}

/**
 * Unit tests the {@code Edge} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <iostream>
int main(int argc, char *argv[]) {
    Edge e(12, 34, 5.67);
    Edge f(13, 35, 5.68);

    std::cout << e.ToString() << " compare " << f.ToString() << " is " << 
      (e < f) << std::endl;

    return 0;
}
#endif
