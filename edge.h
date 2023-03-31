#ifndef EDGE_H_
#define EDGE_H_

#include <string>

/**
 *  The {@code Edge} class represents a weighted edge in an
 *  {@link EdgeWeightedGraph}. Each edge consists of two integers
 *  (naming the two vertices) and a real-value weight. The data type
 *  provides methods for accessing the two endpoints of the edge and
 *  the weight. The natural order for this data type is by
 *  ascending order of weight.
 *  <p>
 *  For additional documentation, see <a href="https://algs4.cs.princeton.edu/43mst">Section 4.3</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

class Edge {
public:
  /**
   * Initializes an edge between vertices {@code v} and {@code w} of
   * the given {@code weight}.
   *
   * @param  v one vertex
   * @param  w the other vertex
   * @param  weight the weight of this edge
   * @throws IllegalArgumentException if either {@code v} or {@code w}
   *         is a negative integer
   * @throws IllegalArgumentException if {@code weight} is {@code NaN}
   */
  Edge(int v, int w, double weight) noexcept : v_(v), w_(w), weight_(weight) {}
  Edge() = delete;
  Edge(const Edge& other) = default;
  Edge &operator=(const Edge& other) = default;
  Edge(Edge&& other) = default;
  Edge &operator=(Edge&& other) = default;

  /**
   * Returns the weight of this edge.
   *
   * @return the weight of this edge
   */
  double weight() const { return weight_; }

  /**
   * Returns either endpoint of this edge.
   *
   * @return either endpoint of this edge
   */
  int Either() const { return v_; }

  /**
   * Returns the endpoint of this edge that is different from the given vertex.
   *
   * @param  vertex one endpoint of this edge
   * @return the other endpoint of this edge
   * @throws IllegalArgumentException if the vertex is not one of the
   *         endpoints of this edge
   */
  int other(int vertex) const;

  /**
   * Returns a string representation of this edge.
   *
   * @return a string representation of this edge
   */
  std::string ToString() const;

private:
  int v_;
  int w_;
  double weight_;
};

bool operator<(const Edge& left, const Edge& right);

#endif  // EDGE_H_
