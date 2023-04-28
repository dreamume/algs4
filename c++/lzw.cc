/******************************************************************************
 *  Compilation:  clang++ -DDebug -O2 lzw.cc -std=c++20 -o lzw
 *  Execution:    ./lzw -c input.txt output.lzw
 *  Execution:    ./lzw -d output.lzw output.txt
 *  Dependencies: 
 *  Data files:   https://algs4.cs.princeton.edu/55compression/abraLZW.txt
 *                https://algs4.cs.princeton.edu/55compression/ababLZW.txt
 *
 *  Compress or expand binary input from standard input using LZW.
 *
 ******************************************************************************/

#include "lzw.h"

#include <string>
#include <vector>
#include <utility>
#include <exception>
#include <array>
#include <exception>
#include <iostream>
#include <memory>

using std::string;
using std::vector;
using std::to_string;
using std::cin;
using std::array;
using std::cerr;
using std::pair;
using std::endl;
using std::unique_ptr;
using std::make_pair;

namespace algs4 {

using CodeType = std::uint16_t;

constexpr CodeType kDMS {std::numeric_limits<CodeType>::max()};

class EncoderDictionary {
  struct Node {
    explicit Node(char c): first(kDMS), c(c), left(kDMS), right(kDMS) {}
    
    CodeType first;
    char c;
    CodeType left;
    CodeType right; 
  };

public:
  EncoderDictionary() {
    CodeType k{0};

    for (long int c = std::numeric_limits<char>::min(); 
         c <= std::numeric_limits<char>::max(); ++c)
      initials_[static_cast<unsigned char>(c)] = k++;

    vn_.reserve(kDMS);
    reset();
  }
  EncoderDictionary(const EncoderDictionary& other) = default;
  EncoderDictionary &operator=(const EncoderDictionary& other) = default;
  EncoderDictionary(EncoderDictionary&& other) = default;
  EncoderDictionary &operator=(EncoderDictionary&& other) = default;

  void reset() {
    vn_.clear();
    
    for (long int c = std::numeric_limits<char>::min(); 
         c <= std::numeric_limits<char>::max(); ++c)
      vn_.push_back(Node(c));
  }

  CodeType search_and_insert(CodeType i, char c) {
    if (vn_.size() == kDMS) reset();
    if (i == kDMS) return search_initials(c);

    const CodeType vn_size = vn_.size();
    CodeType ci{vn_[i].first};

    if (ci != kDMS) {
      while (true) {
        if (c < vn_[ci].c) {
          if (vn_[ci].left == kDMS) {
            vn_[ci].left = vn_size;
            break;
          } else {
            ci = vn_[ci].left;
          }
        } else if (c > vn_[ci].c) {
          if (vn_[ci].right == kDMS) {
            vn_[ci].right = vn_size;
            break;
          } else {
            ci = vn_[ci].right;
          }
        } else {
          return ci;
        }
      }
    } else {
      vn_[i].first = vn_size;
    }

    vn_.push_back(Node(c));
    return kDMS;
  }

  CodeType search_initials(char c) const {
    return initials_[static_cast<unsigned char>(c)];
  }

private:
  vector<Node> vn_;
  array<CodeType, 1u << CHAR_BIT> initials_;
};

void compress(std::istream &is, std::ostream &os) {
  EncoderDictionary ed;
  CodeType i{kDMS};
  char c;

  while (is.get(c)) {
    const CodeType tmp{i};
    if ((i = ed.search_and_insert(tmp, c)) == kDMS) {
      os.write(reinterpret_cast<const char *>(&tmp), sizeof(CodeType));
      i = ed.search_initials(c);
    }
  }

  if (i != kDMS)
    os.write(reinterpret_cast<const char *>(&i), sizeof(CodeType));
}

void decompress(std::istream &is, std::ostream &os) {
  vector<pair<CodeType, char>> dictionary;

  const auto reset_dictionary = [&dictionary] {
    dictionary.clear();
    dictionary.reserve(kDMS);
    
    for (long int c = std::numeric_limits<char>::min(); 
         c <= std::numeric_limits<char>::max(); ++c)
      dictionary.push_back(make_pair(kDMS, static_cast<char>(c)));
  };

  const auto rebuild_string = [&dictionary](CodeType k) -> const vector<char> * {
    static vector<char> s;
    s.clear();
    s.reserve(kDMS);

    while (k != kDMS) {
      s.push_back(dictionary[k].second);
      k = dictionary[k].first;
    }

    std::reverse(s.begin(), s.end());
    return &s;
  };

  reset_dictionary();

  CodeType i{kDMS};
  CodeType k;

  while (is.read(reinterpret_cast<char *>(&k), sizeof(CodeType))) {
    if (dictionary.size() == kDMS) reset_dictionary();
    if (k > dictionary.size()) 
      throw std::runtime_error("invalid compressed code");

    const std::vector<char> *s;

    if (k == dictionary.size()) {
      dictionary.push_back(make_pair(i, rebuild_string(i)->front()));
      s = rebuild_string(k);
    } else {
      s = rebuild_string(k);

      if (i != kDMS) dictionary.push_back(make_pair(i, s->front()));
    }

    os.write(&s->front(), s->size());
    i = k;
  }

  if (!is.eof() || is.gcount() != 0)
    throw std::runtime_error("corrupted compressed file");
}
}

/**
 * Sample client that calls {@code compress()} if the command-line
 * argument is "-" an {@code expand()} if it is "+".
 *
 * @param args the command-line arguments
 */
#ifdef Debug
void print_usage(const string&s = "", bool su = true) {
  if (!s.empty()) cerr << "\nERROR: " << s << endl;
  if (su) {
    cerr << endl << "Usage:" << endl;
    cerr << "\tprogram -flag input_file output_file" << endl << endl;
    cerr << "Where `flag' is either `c' for compressing, or `d' for decompressing, and"
         << endl;
    cerr << "`inpute_file' and `output_file' are distinct files." << endl << endl;
    cerr << "Examples:" << endl;
    cerr << "\tlzw -c license.txt license.lzw" << endl;
    cerr << "\tlzw -d license.lzw new_license.txt" << endl;
  }

  cerr << endl;
}

using namespace algs4;
int main(int argc, char *argv[]) {
  if (argc != 4) {
    print_usage("Wrong number of arguments.");
    return EXIT_FAILURE;
  }

  enum class Mode {
    Compress,
    Decompress
  };

  Mode m;

  if (string(argv[1]) == "-c") {
    m = Mode::Compress;
  } else if (string(argv[1]) == "-d") {
    m = Mode::Decompress;
  } else {
    print_usage(string("flag `") + argv[1] + "' is not recognized.");
    return EXIT_FAILURE;
  }

  const std::size_t buffer_size{1024 * 1024};

  const unique_ptr<char[]> input_buffer(new char[buffer_size]);
  const unique_ptr<char[]> output_buffer(new char[buffer_size]);

  std::ifstream input_file;
  std::ofstream output_file;
  
  input_file.rdbuf()->pubsetbuf(input_buffer.get(), buffer_size);
  input_file.open(argv[2], std::ios_base::binary);

  if (!input_file.is_open()) {
    print_usage(string("input_file `") + argv[2] + "' could not be opened.");
    return EXIT_FAILURE;
  }

  output_file.rdbuf()->pubsetbuf(output_buffer.get(), buffer_size);
  output_file.open(argv[3], std::ios_base::binary);

  if (!output_file.is_open()) {
    print_usage(string("output_file `") + argv[3] + "' could not be opened.");
    return EXIT_FAILURE;
  }

  try {
    input_file.exceptions(std::ios_base::badbit);
    output_file.exceptions(std::ios_base::badbit | std::ios_base::failbit);

    if (m == Mode::Compress) compress(input_file, output_file);
    else if (m == Mode::Decompress) decompress(input_file, output_file);
  } catch (const std::ios_base::failure &f) {
    print_usage(string("File input/output failure: ") + f.what() + '.', false);
    return EXIT_FAILURE;
  } catch (const std::exception &e) {
    print_usage(string("Caught exception: ") + e.what() + '.', false);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
#endif
