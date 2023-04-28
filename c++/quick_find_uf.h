#ifndef QUICK_FIND_UF_H
#define QUICK_FIND_UF_H

#include <vector>
#include <exception>

class QuickFindUF {
 public:
    /**
     * Initializes an empty union-find data structure with
     * {@code n} elements {@code 0} through {@code n-1}. 
     * Initially, each elements is in its own set.
     *
     * @param  n the number of elements
     * @throws IllegalArgumentException if {@code n < 0}
     */
  QuickFindUF(int n): _id(std::vector<int>(n, 0)) {
    _count = n;
    for (int i = 0; i < n; i++) _id[i] = i;
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
    Validate(p);
    return _id[p];
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
    Validate(p);
    Validate(q);
    return _id[p] == _id[q];
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
  void unionWith(int p, int q) {
    Validate(p);
    Validate(q);
    int pID = _id[p];   // needed for correctness
    int qID = _id[q];   // to reduce the number of array accesses

    // p and q are already in the same component
    if (pID == qID) return;

    for (int i = 0; i < _id.size(); i++)
      if (_id[i] == pID) _id[i] = qID;
    _count--;
  }
 private:
  // Validate that p is a valid index
  void Validate(int p) const {
    if (p < 0 || p >= _id.size())
      throw std::invalid_argument("invalid id index!");
  }

 private:
  std::vector<int> _id;              /* _id[i] = component identifier of i */
  int _count;                   /* number of components */
};

#endif
