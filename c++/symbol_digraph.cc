/******************************************************************************
 *  Compilation:  clang++ -c -O2 digraph.cc -std=c++20
 *                clang++ -DDebug -O2 symbol_digraph.cc digraph.o -std=c++20 -o symbol_digraph
 *  Execution:    ./symbol_digraph routes.txt
 *  Dependencies: digraph.h
 *  Data files:   https://algs4.cs.princeton.edu/42digraph/routes.txt
 *  
 *  %  ./symbol_digraph routes.txt
 *  JFK
 *     MCO
 *     ATL
 *     ORD
 *  ATL
 *     HOU
 *     MCO
 *  LAX
 *
 ******************************************************************************/

#include "symbol_digraph.h"

#include <fstream>
#include <exception>

#include "digraph.h"

using std::map;
using std::vector;
using std::string;
using std::fstream;

namespace algs4 {
/**
 *  The {@code SymbolDigraph} class represents a digraph, where the
 *  vertex names are arbitrary strings.
 *  By providing mappings between string vertex names and integers,
 *  it serves as a wrapper around the
 *  {@link Digraph} data type, which assumes the vertex names are integers
 *  between 0 and <em>V</em> - 1.
 *  It also supports initializing a symbol digraph from a file.
 *  <p>
 *  This implementation uses an {@link ST} to map from strings to integers,
 *  an array to map from integers to strings, and a {@link Digraph} to store
 *  the underlying graph.
 *  The <em>indexOf</em> and <em>contains</em> operations take time 
 *  proportional to log <em>V</em>, where <em>V</em> is the number of vertices.
 *  The <em>nameOf</em> operation takes constant time.
 *  <p>
 *  For additional documentation, see <a href="https://algs4.cs.princeton.edu/42digraph">Section 4.2</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

/**  
 * Initializes a digraph from a file using the specified delimiter.
 * Each line in the file contains
 * the name of a vertex, followed by a list of the names
 * of the vertices adjacent to that vertex, separated by the delimiter.
 * @param filename the name of the file
 * @param delimiter the delimiter between fields
 */
SymbolDigraph::SymbolDigraph(const string& filename, const string& delimiter) noexcept {
  // First pass builds the index by reading strings to associate
  // distinct strings with an index
  fstream in(filename);
  while (!in.eof()) {
    string line;
    std::getline(in, line);
    if (line == "") break;

    string::size_type pos = 0;
    while (pos != string::npos) {
      string::size_type new_pos = line.find(delimiter, pos);
      string vertice;
      if (new_pos != string::npos) {
        vertice = line.substr(pos, new_pos - pos);
        pos = new_pos + 1;
      } else {
        vertice = line.substr(pos);
        pos = new_pos;
      }
      auto it = vertex_.find(vertice);
      if (it == vertex_.end()) {
        vertex_[vertice] = vertex_.size();
        keys_.push_back(vertice);
      }
    }
  }

  // second pass builds the digraph by connecting first vertex on each
  // line to all others
  graph_ = new Digraph(vertex_.size());
  fstream in2(filename);
  while (!in2.eof()) {
    string line;
    std::getline(in2, line);

    string::size_type pos = line.find(delimiter);
    if (pos == string::npos) continue;
    int v = vertex_[line.substr(0, pos)];
    ++pos;
    while (true) {
      string::size_type new_pos = line.find(delimiter, pos);
      if (new_pos != string::npos) {
        graph_->AddEdge(v, vertex_[line.substr(pos, new_pos - pos)]);
        pos = new_pos + 1;
      } else {
        string w = line.substr(pos);
        graph_->AddEdge(v, vertex_[line.substr(pos)]);
        break;
      }
    }
  }
}

// throw an IllegalArgumentException unless {@code 0 <= v < V}
void SymbolDigraph::ValidateVertex(int v) const {
  int V = graph_->V();
  if (v < 0 || v >= V)
    throw std::invalid_argument("vertex " + std::to_string(v) + 
                                " is not between 0 and " + std::to_string(V-1));
}
}

/**
 * Unit tests the {@code SymbolDigraph} data type.
 *
 * @param args the command-line arguments
 */
#ifdef Debug
#include <iostream>
#include <cstdio>
#include <algorithm>
using namespace algs4;
int main(int argc, char *argv[]) {
  string delimiter{" "};
  if (argc > 2) delimiter = argv[2];
  SymbolDigraph sg(argv[1], delimiter);
  Digraph* graph = sg.digraph();
  string t;
  auto print = [&](const int& n) { std::cout << "    " << sg.Name(n) << std::endl; };
  while (std::cin >> t) {
    vector<int> array = graph->adj(sg.Index(t));
    std::for_each(array.cbegin(), array.cend(), print);
  }
}
#endif

/******************************************************************************
 *  Copyright 2002-2018, Robert Sedgewick and Kevin Wayne.
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
