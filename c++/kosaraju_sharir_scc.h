#ifndef KOSARAJU_SHARIR_SCC_H_
#define KOSARAJU_SHARIR_SCC_H_

/**
 *  The {@code KosarajuSharirSCC} class represents a data type for
 *  determining the strong components in a digraph.
 *  The <em>id_</em> operation determines in which strong component
 *  a given vertex lies; the <em>areStronglyConnected</em> operation
 *  determines whether two vertices are in the same strong component;
 *  and the <em>count_</em> operation determines the number of strong
 *  components.
 *  <p>
 *  The <em>component identifier</em> of a component is one of the
 *  vertices in the strong component: two vertices have the same component
 *  identifier if and only if they are in the same strong component.
 *  <p>
 *  This implementation uses the Kosaraju-Sharir algorithm.
 *  The constructor takes &Theta;(<em>V</em> + <em>E</em>) time,
 *  where <em>V</em> is the number of vertices and <em>E</em>
 *  is the number of edges.
 *  Each instance method takes &Theta;(1) time.
 *  It uses &Theta;(<em>V</em>) extra space (not including the digraph).
 *  For alternative implementations of the same API, see
 *  {@link TarjanSCC} and {@link GabowSCC}.
 *  <p>
 *  For additional documentation, see
 *  <a href="https://algs4.cs.princeton.edu/42digraph">Section 4.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#include <vector>

#include "digraph.h"
#include "depth_first_order.h"

class KosarajuSharirSCC {
public:
  /**
   * Computes the strong components of the digraph {@code G}.
   * @param G the digraph
   */
  KosarajuSharirSCC(const Digraph& G) noexcept;
  KosarajuSharirSCC() = delete;
  KosarajuSharirSCC(const KosarajuSharirSCC& other) = default;
  KosarajuSharirSCC &operator=(const KosarajuSharirSCC& other) = default;
  KosarajuSharirSCC(KosarajuSharirSCC&& other) = default;
  KosarajuSharirSCC &operator=(KosarajuSharirSCC&& other) = default;

  /**
   * Returns the number of strong components.
   * @return the number of strong components
   */
  int count() const { return count_; }

  /**
   * Are vertices {@code v} and {@code w} in the same strong component?
   * @param  v one vertex
   * @param  w the other vertex
   * @return {@code true} if vertices {@code v} and {@code w} are in the same
   *         strong component, and {@code false} otherwise
   * @throws IllegalArgumentException unless {@code 0 <= v < V}
   * @throws IllegalArgumentException unless {@code 0 <= w < V}
   */
  bool StronglyConnected(int v, int w) const;

  /**
   * Returns the component id_ of the strong component containing vertex {@code v}.
   * @param  v the vertex
   * @return the component id_ of the strong component containing vertex {@code v}
   * @throws IllegalArgumentException unless {@code 0 <= s < V}
   */
  int id(int v) const;
private:
  // DFS on graph G
  void Dfs(Digraph G, int v);

  // does the id_[] array contain the strongly connected components?
  bool Check(const Digraph& G) const;

  // throw an IllegalArgumentException unless {@code 0 <= v < V}
  void ValidateVertex(int v) const;

private:
  std::vector<bool> marked_;     // marked_[v] = has vertex v been visited?
  std::vector<int> id_;             // id_[v] = id_ of strong component containing v
  int count_{0};            // number of strongly-connected components
};

#endif  /* KOSARAJU_SHARIR_SCC_H_ */
