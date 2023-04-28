/******************************************************************************
 *  Compilation: clang++ -g -DDebug weighted_quick_union_uf.cc -W -Wall -Wfatal-errors -std=c++20 -o weighted_quick_union_uf
 *  Execution:  weighted_quick_union_uf input.txt
 *  Dependencies: 
 *  Data files:   https://algs4.cs.princeton.edu/15uf/tinyUF.txt
 *                https://algs4.cs.princeton.edu/15uf/mediumUF.txt
 *                https://algs4.cs.princeton.edu/15uf/largeUF.txt
 *
 *  Weighted quick-union (without path compression).
 *
 ******************************************************************************/

/**
 *  The {@code WeightedQuickUnionUF} class represents a <em>union–find data type</em>
 *  (also known as the <em>disjoint-sets data type</em>).
 *  It supports the classic <em>union</em> and <em>find</em> operations,
 *  along with a <em>count</em> operation that returns the total number
 *  of sets.
 *  <p>
 *  The union-find data type models a collection of sets containing
 *  <em>n</em> elements, with each element in exactly one set.
 *  The elements are named 0 through <em>n</em>–1.
 *  Initially, there are <em>n</em> sets, with each element in its
 *  own set. The <em>cannonical elemement</em> of a set
 *  (also known as the <em>root</em>, <em>identifier</em>,
 *  <em>leader</em>, or <em>set representative</em>)
 *  is one distinguished element in the set. Here is a summary of
 *  the operations:
 *  <ul>
 *  <li><em>find</em>(<em>p</em>) returns the canonical element
 *      of the set containing <em>p</em>. The <em>find</em> operation
 *      returns the same value for two elements if and only if
 *      they are in the same set.
 *  <li><em>union</em>(<em>p</em>, <em>q</em>) merges the set
 *      containing element <em>p</em> with the set containing
 *      element <em>q</em>. That is, if <em>p</em> and <em>q</em>
 *      are in different sets, replace these two sets
 *      with a new set that is the union of the two.
 *  <li><em>count</em>() returns the number of sets.
 *  </ul>
 *  <p>
 *  The canonical element of a set can change only when the set
 *  itself changes during a call to <em>union</em>&mdash;it cannot
 *  change during a call to either <em>find</em> or <em>count</em>.
 *  <p>
 *  This implementation uses <em>weighted quick union by size</em>
 *  (without path compression).
 *  The constructor takes &Theta;(<em>n</em>), where <em>n</em>
 *  is the number of elements.
 *  The <em>union</em> and <em>find</em>
 *  operations  take &Theta;(log <em>n</em>) time in the worst
 *  case. The <em>count</em> operation takes &Theta;(1) time.
 *  <p>
 *  For alternative implementations of the same API, see
 *  {@link UF}, {@link QuickFindUF}, and {@link QuickUnionUF}.
 *  For additional documentation, see
 *  <a href="https://algs4.cs.princeton.edu/15uf">Section 1.5</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#include "weighted_quick_union_uf.h"

#include <exception>

#ifdef Debug
#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <fstream>

using std::fstream;

#endif

template <typename T>
void WeightedQuickUnionUF<T>::validate(T p) const {
    if (p < 0 || p >= static_cast<T>(parent_.size()))
        throw std::invalid_argument("invalid id index!");
}

template <typename T>
void WeightedQuickUnionUF<T>::unionWith(T p, T q) {
  T root_p = find(p);
  T root_q = find(q);

  if (root_p == root_q) return;

  // make smaller root point to larger one
  if (size_[root_p] < size_[root_q]) {
    parent_[root_p] = root_q;
    size_[root_q] += size_[root_p];
  } else {
    parent_[root_q] = root_p;
    size_[root_p] += size_[root_q];
  }
  --count_;
}

/**
 * Reads an integer {@code n} and a sequence of pairs of integers
 * (between {@code 0} and {@code n-1}) from standard input, where each integer
 * in the pair represents some element;
 * if the elements are in different sets, merge the two sets
 * and print the pair to standard output.
 * 
 * @param args the command-line arguments
 */
#ifdef Debug
int main(int args, char *argv[]) {
    fstream in(argv[1]);
    if (!in.is_open()) {
        std::cout << "failed to open " << argv[1] << '\n';
        return 1;
    }

    std::string line;
    std::getline(in, line);
    int n = stoi(line);

    WeightedQuickUnionUF<int> uf(n);
    while (!in.eof()) {
        std::getline(in, line);
        std::stringstream ss(line);
        std::istream_iterator<std::string> begin(ss);
        std::istream_iterator<std::string> end;
        std::vector<std::string> vstrings(begin, end);
        if (vstrings.size() < 2) break;
        int p = stoi(vstrings[0]);
        int q = stoi(vstrings[1]);
    
        if (uf.find(p) == uf.find(q)) continue;
        uf.unionWith(p, q);
    }
    printf("%d components\n", uf.count());
}
#endif

/******************************************************************************
 *  Copyright 2002-2020, Robert Sedgewick and Kevin Wayne.
 *
 *  This file is part of algs4.jar, which accompanies the textbook
 *
 *      Algorithms, 4th edition by Robert Sedgewick and Kevin Wayne,
 *      Addison-Wesley Professional, 2011, ISBN 0-321-57351-X.
 *      http://algs4.cs.princeton.edu
 *
 *
 *  algs4.jar is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  algs4.jar is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with algs4.jar.  If not, see http://www.gnu.org/licenses.
 ******************************************************************************/
