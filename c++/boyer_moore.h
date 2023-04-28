#ifndef BYOER_MOORE_H_
#define BYOER_MOORE_H_

#include <vector>
#include <string>

namespace algs4 {
/**
 *  The {@code BoyerMoore} class finds the first occurrence of a pattern string
 *  in a text string.
 *  <p>
 *  This implementation uses the Boyer-Moore algorithm (with the bad-character
 *  rule, but not the strong good suffix rule).
 *  <p>
 *  For additional documentation,
 *  see <a href="https://algs4.cs.princeton.edu/53substring">Section 5.3</a> of
 *  <i>Algorithms, 4th Edition</i> by Robert Sedgewick and Kevin Wayne.
 */
class BoyerMoore {
public:
    /**
     * Preprocesses the pattern string.
     *
     * @param pat the pattern string
     */
  BoyerMoore(const std::string& pat) noexcept;

    /**
     * Preprocesses the pattern string.
     *
     * @param pattern the pattern string
     * @param R the alphabet size
     */
  BoyerMoore() = delete;
  BoyerMoore(const BoyerMoore& other) = default;
  BoyerMoore &operator=(const BoyerMoore& other) = default;
  BoyerMoore(BoyerMoore&& other) = default;
  BoyerMoore &operator=(BoyerMoore&& other) = default;

    /**
     * Returns the index of the first occurrence of the pattern string
     * in the text string.
     *
     * @param  txt the text string
     * @return the index of the first occurrence of the pattern string
     *         in the text string; n if no such match
     */
  int search(const std::string& txt) const;

private:
  int r_{256};     // the radix
  std::vector<int> right_;     // the bad-character skip array
  std::string pattern_{};  // store the pattern as a character array
};
}

#endif  // BYOER_MOORE_H_
