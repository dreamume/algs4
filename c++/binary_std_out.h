#ifndef BINARY_STD_OUT_H_
#define BINARY_STD_OUT_H_

/**
 *  <i>Binary standard output</i>. This class provides methods for converting
 *  primitive type variables ({@code boolean}, {@code byte}, {@code char},
 *  {@code int}, {@code long}, {@code float}, and {@code double})
 *  to sequences of bits and writing them to standard output.
 *  Uses big-endian (most-significant byte first).
 *  <p>
 *  The client must {@code flush()} the output stream when finished writing bits.
 *  <p>
 *  The client should not intermix calls to {@code BinaryStdOut} with calls
 *  to {@code StdOut} or {@code System.out}; otherwise unexpected behavior
 *  will result.
 *
 *  @author Robert Sedgewick
 *  @author Kevin Wayne
 */

#include <string>

namespace algs4 {
class BinaryStdOut {
public:
    // don't instantiate
  BinaryStdOut() = delete;
  BinaryStdOut(const BinaryStdOut& other) = delete;
  BinaryStdOut &operator=(const BinaryStdOut& other) = delete;
  BinaryStdOut(BinaryStdOut&& other) = delete;
  BinaryStdOut &operator=(BinaryStdOut&& other) = delete;

   /**
     * Flushes standard output, padding 0s if number of bits written so far
     * is not a multiple of 8.
     */
  static void Flush();

   /**
     * Flushes and closes standard output. Once standard output is closed, you can no
     * longer write bits to it.
     */
  static void Close();


   /**
     * Writes the specified bit to standard output.
     * @param x the {@code boolean} to write.
     */
  static void Write(bool x) { WriteBit(x); }

   /**
     * Writes the 8-bit byte to standard output.
     * @param x the {@code byte} to write.
     */
  static void Write(char x) { WriteByte((int)x); }

   /**
     * Writes the 32-bit int to standard output.
     * @param x the {@code int} to write.
     */
  static void Write(int x);

   /**
     * Writes the <em>r</em>-bit int to standard output.
     * @param x the {@code int} to write.
     * @param r the number of relevant bits in the char.
     * @throws IllegalArgumentException if {@code r} is not between 1 and 32.
     * @throws IllegalArgumentException if {@code x} is not between 0 and 2<sup>r</sup> - 1.
     */
  static void Write(int x, int r);

   /**
     * Writes the 64-bit double to standard output.
     * @param x the {@code double} to write.
     */
  static void Write(double x);

   /**
     * Writes the 64-bit long to standard output.
     * @param x the {@code long} to write.
     */
  static void Write(long long x);

   /**
     * Writes the 32-bit float to standard output.
     * @param x the {@code float} to write.
     */
  static void Write(float x);

   /**
     * Writes the 16-bit int to standard output.
     * @param x the {@code short} to write.
     */
  static void Write(short x);

   /**
     * Writes the 8-bit char to standard output.
     * @param x the {@code char} to write.
     * @throws IllegalArgumentException if {@code x} is not between 0 and 255.
     */
  //  static void Write(char x);

   /**
     * Writes the <em>r</em>-bit char to standard output.
     * @param x the {@code char} to write.
     * @param r the number of relevant bits in the char.
     * @throws IllegalArgumentException if {@code r} is not between 1 and 16.
     * @throws IllegalArgumentException if {@code x} is not between 0 and 2<sup>r</sup> - 1.
     */
  static void Write(short x, int r);

   /**
     * Writes the string of 8-bit characters to standard output.
     * @param s the {@code String} to write.
     * @throws IllegalArgumentException if any character in the string is not
     * between 0 and 255.
     */
  static void Write(const std::string& s);

   /**
     * Writes the string of <em>r</em>-bit characters to standard output.
     * @param s the {@code String} to write.
     * @param r the number of relevant bits in each character.
     * @throws IllegalArgumentException if r is not between 1 and 16.
     * @throws IllegalArgumentException if any character in the string is not
     * between 0 and 2<sup>r</sup> - 1.
     */
  static void Write(const std::string& s, int r);

private:
    // initialize BinaryStdOut
  static void Initialize();

   /**
     * Writes the specified bit to standard output.
     */
  static void WriteBit(bool bit);

   /**
     * Writes the 8-bit byte to standard output.
     */
  static void WriteByte(int x);

    // write out any remaining bits in buffer to standard output, padding with 0s
  static void ClearBuffer();

};
}

#endif  /* BINARY_STD_OUT_H_ */
