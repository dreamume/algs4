#ifndef LZW_H_
#define LZW_H_

#include <fstream>
#include <istream>
#include <ostream>

namespace algs4 {

void compress(std::istream &is, std::ostream &os);
void decompress(std::istream &is, std::ostream &os);

}

#endif  // LZW_H_
