/**
The MIT License (MIT)

Copyright (c) 2016 Flavio Moreira

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include "basic_types.hpp"

namespace Montreal
{

// computes the imediate largest power of two
// ref: http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogObvious
// @param bytes number to find the nearest pow2 from
// @return the nearest larger power of two
usize roundToPow2(const usize& bytes)
{
  usize rounded = bytes;
  rounded--;
  rounded |= rounded >> 1;
  rounded |= rounded >> 2;
  rounded |= rounded >> 4;
  rounded |= rounded >> 8;
  rounded |= rounded >> 16;
  rounded++;
  return rounded;
}

usize roundToAlign(const usize& bytes, const usize alignment = 8)
{
  usize rounded = bytes + alignment - (bytes % alignment);
  return rounded;
}

} // end namespace Montreal

#endif // FUNCTIONS_HPP
