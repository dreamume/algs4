/******************************************************************************
 *  Compilation: clang++ -g -DDebug quick_find_uf.cc -std=c++11 -o quick_find_uf
 *  Execution:  quick_find_uf < input.txt
 *  Dependencies: 
 *  Data files:   https://algs4.cs.princeton.edu/15uf/tinyUF.txt
 *                https://algs4.cs.princeton.edu/15uf/mediumUF.txt
 *                https://algs4.cs.princeton.edu/15uf/largeUF.txt
 *
 *  Quick-find algorithm.
 *
 ******************************************************************************/

/**
 *  The {@code QuickFindUF} class represents a <em>union–find data type</em>
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
 *  itself changes during a call to <em>unionWith</em>&mdash;it cannot
 *  change during a call to either <em>find</em> or <em>count</em>.
 *  <p>
 *  This implementation uses <em>quick find</em>.
 *  The constructor takes &Theta;(<em>n</em>) time, where <em>n</em>
 *  is the number of sites.
 *  The <em>find</em>, <em>connected</em>, and <em>count</em>
 *  operations take &Theta;(1) time; the <em>unionWith</em> operation
 *  takes &Theta;(<em>n</em>) time.
 *  <p>
 *  For alternative implementations of the same API, see
 *  {@link UF}, {@link QuickUnionUF}, and {@link WeightedQuickUnionUF}.
 *  For additional documentation, see
 *  <a href="https://algs4.cs.princeton.edu/15uf">Section 1.5</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#include "quick_find_uf.h"

#include <exception>

#ifdef Debug
#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <fstream>

using std::fstream;

#endif

void QuickFindUF::validate(int p) {
  if (p < 0 || p >= _id.size())
	throw std::invalid_argument("invalid id index!");
}

void QuickFindUF::unionWith(int p, int q) {
  validate(p);
  validate(q);
  int pID = _id[p];   // needed for correctness
  int qID = _id[q];   // to reduce the number of array accesses

  // p and q are already in the same component
  if (pID == qID) return;

  for (int i = 0; i < _id.size(); i++)
    if (_id[i] == pID) _id[i] = qID;
  _count--;
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

  QuickFindUF uf(n);
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
    //printf("%d %d", p, q);
  } //while (!std::cin.fail() || !std::cin.eof());
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
