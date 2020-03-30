#ifndef QUICK_UNION_UF_H
#define QUICK_UNION_UF_H

#include <vector>

class QuickUnionUF {
 public:
    /**
     * Initializes an empty union-find data structure with
     * {@code n} elements {@code 0} through {@code n-1}. 
     * Initially, each elements is in its own set.
     *
     * @param  n the number of elements
     * @throws IllegalArgumentException if {@code n < 0}
     */
  QuickUnionUF(int n): _parent(std::vector<int>(n, 0)) {
    _count = n;
    for (int i = 0; i < n; i++) _parent[i] = i;
  }

    /**
     * Returns the number of sets.
     *
     * @return the number of sets (between {@code 1} and {@code n})
     */
  int count() { return _count; }
    /**
     * Returns the canonical element of the set containing element {@code p}.
     *
     * @param  p an element
     * @return the canonical element of the set containing {@code p}
     * @throws IllegalArgumentException unless {@code 0 <= p < n}
     */
  int find(int p) {
    validate(p);
    while (p != _parent[p]) p = _parent[p];
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
  bool connected(int p, int q) {
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
  void validate(int p);

 private:
  std::vector<int> _parent;              /* _parent[i] = component identifier of i */
  int _count;                   /* number of components */
};

#endif
