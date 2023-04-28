#ifndef QUICK_UNION_UF_H_
#define QUICK_UNION_UF_H_

#include <vector>
#include <numeric>
#include <exception>

namespace algs4 {
template<typename T>
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
  QuickUnionUF(int n) noexcept : parent_(std::vector<T>(n)), 
                                 //                                   rank_(std::vector<int>(n)), count_(n) {
                                 count_(n) {
    std::iota(parent_.begin(), parent_.end(), 0);
  }
  QuickUnionUF() = delete;
  QuickUnionUF(const QuickUnionUF& other) = default;
  QuickUnionUF &operator=(const QuickUnionUF& other) = default;
  QuickUnionUF(QuickUnionUF&& other) = default;
  QuickUnionUF &operator=(QuickUnionUF&& other) = default;

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
  T Find(T p) const {
    Validate(p);
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
  bool Connected(T p, T q) const {
    return Find(p) == Find(q);
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
  void UnionWith(T p, T q) {
    T root_p = Find(p);
    T root_q = Find(q);

    if (root_p == root_q) return;

    parent_[root_p] = root_q;
    // if (rank_[root_p] < rank_[root_q]) {
    //     parent_[root_p] = root_q;
    // } else {
    //     parent_[root_q] = root_p;
    //     if (rank_[root_p] == rank_[root_q]) ++rank_[root_p];
    // }
    --count_;
  }
private:
  // validate that p is a valid index
  void Validate(T p) const {
    if (p < 0 || p >= static_cast<T>(parent_.size()))
      throw std::invalid_argument("invalid id index!");
  }

private:
  std::vector<T> parent_;              /* parent_[i] = component identifier of i */
  //std::vector<T> rank_;
  int count_;                   /* number of components */
};
}

#endif  // QUICK_UNION_UF_H_
