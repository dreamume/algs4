/******************************************************************************
 *  Compilation:  javac BinaryStdOut.java
 *  Execution:    java BinaryStdOut
 *  Dependencies: none
 *
 *  Write binary data to standard output, either one 1-bit boolean,
 *  one 8-bit char, one 32-bit int, one 64-bit double, one 32-bit float,
 *  or one 64-bit long at a time.
 *
 *  The bytes written are not aligned.
 *
 ******************************************************************************/

#include "binary_std_out.h"

#include <iostream>
#include <exception>
#include <cassert>

using std::cout;
using std::string;
using std::to_string;

namespace algs4 {

static int buffer;                // 8-bit buffer of bits to write
static int n;                     // number of bits remaining in buffer
static bool is_initialized;     // has BinaryStdOut been called for first time?

void BinaryStdOut::Initialize() {
  buffer = 0;
  n = 0;
  is_initialized = true;
}

void BinaryStdOut::WriteBit(bool bit) {
  if (!is_initialized) Initialize();

  // add bit to buffer
  buffer <<= 1;
  if (bit) buffer |= 1;

  // if buffer is full (8 bits), write out as a single byte
  ++n;
  if (n == 8) ClearBuffer();
}

void BinaryStdOut::WriteByte(int x) {
  if (!is_initialized) Initialize();

  //assert(x >= 0 && x < 256);

  // optimized if byte-aligned
  if (n == 0) {
    cout << (char)x;
    return;
  }

  // otherwise write one bit at a time
  for (int i = 0; i < 8; i++)
    WriteBit((x >> (8 - i - 1)) & 1);
}

void BinaryStdOut::ClearBuffer() {
  if (!is_initialized) Initialize();

  if (n == 0) return;
  if (n > 0) buffer <<= (8 - n);
  cout << (char)buffer;

  n = 0;
  buffer = 0;
}

void BinaryStdOut::Flush() {
  ClearBuffer();
  cout.flush();
}

void BinaryStdOut::Close() {
  Flush();
  is_initialized = false;
}

void BinaryStdOut::Write(int x) {
  WriteByte(x >> 24);
  WriteByte(x >> 16);
  WriteByte(x >>  8);
  WriteByte(x >>  0);
}

void BinaryStdOut::Write(int x, int r) {
  if (r == 32) {
    Write(x);
    return;
  }
  if (r < 1 || r > 32)        
    throw std::invalid_argument("Illegal value for r = " + to_string(r));
  if (x < 0 || x >= (1 << r)) 
    throw std::invalid_argument("Illegal " + to_string(r) + "-bit char = " + to_string(x));
  for (int i = 0; i < r; i++)
    WriteBit(x >> (r - i - 1));
}

void BinaryStdOut::Write(long long x) {
  WriteByte(x >> 56);
  WriteByte(x >> 48);
  WriteByte(x >> 40);
  WriteByte(x >> 32);
  WriteByte(x >> 24);
  WriteByte(x >> 16);
  WriteByte(x >>  8);
  WriteByte(x >>  0);
}

void BinaryStdOut::Write(double x) {
  union BinaryStdOutDoubleUnion {
    double dValue;
    long long iValue;
  };

  BinaryStdOutDoubleUnion tmp;
  tmp.dValue = x;
  Write(tmp.iValue);
}

void BinaryStdOut::Write(float x) {
  union BinaryStdOutFloatUnion {
    float fValue;
    int iValue;
  };

  BinaryStdOutFloatUnion tmp;
  tmp.fValue = x;
  Write(tmp.iValue);
}

void BinaryStdOut::Write(short x) {
  WriteByte(x >>  8);
  WriteByte(x);
}

// void BinaryStdOut::Write(char x) {
//   WriteByte(x);
// }

void BinaryStdOut::Write(short x, int r) {
  if (r == 8) {
    Write((char)x);
    return;
  }
  if (r < 1 || r > 16) 
    throw std::invalid_argument("Illegal value for r = " + to_string(r));
  if (x >= (1 << r))   
    throw std::invalid_argument("Illegal " + to_string(r) + 
                                "-bit char = " + to_string(x));
  for (int i = 0; i < r; i++)
    WriteBit(x >> (r - i - 1));
}

void BinaryStdOut::Write(const string& s) {
  for (const auto ch : s) Write(ch);
}

void BinaryStdOut::Write(const string& s, int r) {
  for (const auto ch : s)
    Write((int)ch, r);
}
}

/**
  * Tests the methods in this class.
  *
  * @param args the command-line arguments
  */
#ifdef Debug
using namespace algs4;
int main(int argc, char *argv[]) {
  int m = atoi(argv[1]);

  // write n integers to binary standard output
  for (int i = 0; i < m; i++)
    BinaryStdOut::Write(i);
  BinaryStdOut::Flush();

  return 0;
}
#endif
