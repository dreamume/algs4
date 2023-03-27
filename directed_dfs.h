#ifndef DIRECTED_DFS
#define DIRECTED_DFS

#include "digraph.h"

#include <vector>

class DirectedDFS {
public:
    /**
     * Computes the vertices in digraph {@code G} that are
     * reachable from the source vertex {@code s}.
     * @param G the digraph
     * @param s the source vertex
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     */
    DirectedDFS(const Digraph& G, int s) noexcept;

    /**
     * Computes the vertices in digraph {@code G} that are
     * connected to any of the source vertices {@code sources}.
     * @param G the graph
     * @param sources the source vertices
     * @throws IllegalArgumentException if {@code sources} is {@code null}
     * @throws IllegalArgumentException if {@code sources} contains no vertices
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     *         for each vertex {@code s} in {@code sources}
     */
    DirectedDFS(const Digraph& G, const std::vector<int>& sources) noexcept;
    DirectedDFS() = delete;
    DirectedDFS(const DirectedDFS& other) = default;
    DirectedDFS &operator=(const DirectedDFS& other) = default;
    DirectedDFS(DirectedDFS&& other) = default;
    DirectedDFS &operator=(DirectedDFS&& other) = default;

    /**
     * Is there a directed path from the source vertex (or any
     * of the source vertices) and vertex {@code v}?
     * @param  v the vertex
     * @return {@code true} if there is a directed path, {@code false} otherwise
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    bool marked(int v) const {
        validateVertex(v);
        return marked_[v];
    }

    /**
     * Returns the number of vertices reachable from the source vertex
     * (or source vertices).
     * @return the number of vertices reachable from the source vertex
     *   (or source vertices)
     */
    int count() const {
        return count_;
    }

private:
    void dfs(const Digraph& G, int v);

    // throw an IllegalArgumentException unless {@code 0 <= v < V}
    void validateVertex(int v) const;

    // throw an IllegalArgumentException if vertices is null, has zero vertices,
    // or has a vertex not between 0 and V-1
    void validateVertices(const std::vector<int>& vertices) const;
private:
    std::vector<bool> marked_;  // marked_[v] = true iff v is reachable from source(s)
    int count_{0};         // number of vertices reachable from source(s)
};

#endif
