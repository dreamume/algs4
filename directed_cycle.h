#include <vector>
#include <stack>

#include "digraph.h"

class DirectedCycle {
public:
    /**
     * Determines whether the digraph {@code G} has a directed cycle and, if so,
     * finds such a cycle.
     * @param G the digraph
     */
    DirectedCycle(const Digraph& G) noexcept;
    DirectedCycle() = default;
    DirectedCycle(const DirectedCycle& other) = default;
    DirectedCycle &operator=(const DirectedCycle& other) = default;
    DirectedCycle(DirectedCycle&& other) = default;
    DirectedCycle &operator=(DirectedCycle&& other) = default;
    /**
     * Does the digraph have a directed cycle?
     * @return {@code true} if the digraph has a directed cycle, {@code false} otherwise
     */
    bool hasCycle() const { return !cycle_.empty(); }
    /**
     * Returns a directed cycle if the digraph has a directed cycle, and {@code null} otherwise.
     * @return a directed cycle (as an iterable) if the digraph has a directed cycle,
     *    and {@code null} otherwise
     */
    std::stack<int> cycle() const { return cycle_; }

private:
    // check that algorithm computes either the topological order or finds a directed cycle
    void dfs(const Digraph& G, int v);
    // certify that digraph has a directed cycle if it reports one
    bool check() const;

private:
    std::vector<bool> marked_;        // marked[v] = has vertex v been marked?
    std::vector<int> edge_to_;            // edgeTo[v] = previous vertex on path to v
    std::vector<bool> on_stack_;       // onStack[v] = is vertex on the stack?
    std::stack<int> cycle_;    // directed cycle (or null if no such cycle)
};
