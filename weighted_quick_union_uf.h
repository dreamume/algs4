#ifndef WEIGHTED_QUICK_UNION_UF_H
#define WEIGHTED_QUICK_UNION_UF_H

#include <vector>
#include <numeric>

class WeightedQuickUnionUF {
public:
    /**
     * Initializes an empty union-find data structure with
     * {@code n} elements {@code 0} through {@code n-1}. 
     * Initially, each elements is in its own set.
     *
     * @param  n the number of elements
     * @throws IllegalArgumentException if {@code n < 0}
     */
    WeightedQuickUnionUF(int n) noexcept : parent_(std::vector<int>(n)),
                                           size_(std::vector<int>(n, 1)), count_(n) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }
    WeightedQuickUnionUF() = delete;
    WeightedQuickUnionUF(const WeightedQuickUnionUF& other) = default;
    WeightedQuickUnionUF &operator=(const WeightedQuickUnionUF& other) = default;
    WeightedQuickUnionUF(WeightedQuickUnionUF&& other) = default;
    WeightedQuickUnionUF &operator=(WeightedQuickUnionUF&& other) = default;

    /**
     * Returns the number of sets.
     *
     * @return the number of sets (between {@code 1} and {@code n})
     */
    int count() const { return count_; }
    /**
     * Returns the canonical element of the set containing element {@code p}.
     *
     * @param  p an element
     * @return the canonical element of the set containing {@code p}
     * @throws IllegalArgumentException unless {@code 0 <= p < n}
     */
    int find(int p) const {
        validate(p);
        while (p != parent_[p]) p = parent_[p];
        return p;
    }

    /**
     * Returns true if the two elements are in the same set.
     * 
     * @param  p one element
     * @param  q the other element
     * @return {@code true} if {@code p} and {@code q} are in the same set;
     *         {@code false} otherwise
     * @throws IllegalArgumentException unless
     *         both {@code 0 <= p < n} and {@code 0 <= q < n}
     * @deprecated Replace with two calls to {@link #find(int)}.
     */
    bool connected(int p, int q) const {
        return find(p) == find(q);
    }

    /**
     * Merges the set containing element {@code p} with the 
     * the set containing element {@code q}.
     *
     * @param  p one element
     * @param  q the other element
     * @throws IllegalArgumentException unless
     *         both {@code 0 <= p < n} and {@code 0 <= q < n}
     */
    void unionWith(int p, int q);
private:
    // validate that p is a valid index
    void validate(int p) const;

private:
    std::vector<int> parent_;      /* parent_[i] = parent of i */
    std::vector<int> size_;        /* size_[i] = number of elements in subtree rooted at i */
    int count_;                   /* number of components */
};

#endif
