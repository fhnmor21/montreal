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

#ifndef HASH_HPP
#define HASH_HPP

#include "basic_types.hpp"
#include "hashes/City.h"
#include "hashes/MurmurHash2.h"
#include "hashes/Spooky.h"

namespace Montreal
{

struct Hashes
{
  union {
    struct
    {
      u64 murmur;
      u64 city;
      u64 spooky;
    };
    u64 h[3];
  };
};

template < typename KeyType >
Hashes hash(const KeyType& key, u64 seed = 0x9747b28c)
{
  const usize len(sizeof(KeyType));

  Hashes rVal = {{{0, 0, 0}}};

  union {
    KeyType key_;
    u8 buf_[len];
  } buffer;

  buffer.key_ = key;

  rVal.city = CityHash64WithSeed(buffer.buf_, len, seed);
  rVal.murmur = MurmurHash64A(buffer.buf_, len, seed);
  rVal.spooky = SpookyHash::Hash64(buffer.buf_, len, seed);
}

} // end namespace Montreal

#endif // HASH_HPP
