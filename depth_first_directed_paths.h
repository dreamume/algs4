#ifndef DEPTH_FIRST_DIRECTED_PATHS
#define DEPTH_FIRST_DIRECTED_PATHS

#include <vector>
#include <stack>

#include "digraph.h"

class DepthFirstDirectedPaths {
public:
    /**
     * Computes a directed path from {@code s} to every other vertex in digraph {@code G}.
     * @param  G the digraph
     * @param  s the source vertex
     * @throws IllegalArgumentException unless {@code 0 <= s < V}
     */
    DepthFirstDirectedPaths(const Digraph& G, int s) noexcept : marked_(G.V()), edgeTo_(G.V()), s_(s) {
        //validateVertex(s);
        dfs(G, s);
    }
    DepthFirstDirectedPaths() = delete;
    DepthFirstDirectedPaths(const DepthFirstDirectedPaths& other) = default;
    DepthFirstDirectedPaths &operator=(const DepthFirstDirectedPaths& other) = default;
    DepthFirstDirectedPaths(DepthFirstDirectedPaths&& other) = default;
    DepthFirstDirectedPaths &operator=(DepthFirstDirectedPaths&& other) = default;

    /**
     * Is there a directed path from the source vertex {@code s} to vertex {@code v}?
     * @param  v the vertex
     * @return {@code true} if there is a directed path from the source
     *         vertex {@code s} to vertex {@code v}, {@code false} otherwise
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    bool hasPathTo(int v) const {
        validateVertex(v);
        return marked_[v];
    }

    /**
     * Returns a directed path from the source vertex {@code s} to vertex {@code v}, or
     * {@code null} if no such path.
     * @param  v the vertex
     * @return the sequence of vertices on a directed path from the source vertex
     *         {@code s} to vertex {@code v}, as an Iterable
     * @throws IllegalArgumentException unless {@code 0 <= v < V}
     */
    std::stack<int> pathTo(int v) const;

private:
    void dfs(const Digraph& G, int v);


    // throw an IllegalArgumentException unless {@code 0 <= v < V}
    void validateVertex(int v) const;

private:
    std::vector<bool> marked_;  // marked_[v] = true iff v is reachable from s
    std::vector<int> edgeTo_;      // edgeTo_[v] = last edge on path from s to v
    int s_;       // source vertex
};

#endif
