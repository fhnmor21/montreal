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

#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include "basic_types.hpp"

namespace Montreal
{
// forward declarations
struct Allocator;

///////////////////////////////////////////////////////////////////////////////
// HashMap
///////////////////////////////////////////////////////////////////////////////

// TODO: Implement
template < typename Type >
struct HashMapInterface
{
  virtual ~HashMapInterface() {}
  HashMapInterface();
  explicit HashMapInterface(const HashMapInterface& other);
  HashMapInterface& operator=(const HashMapInterface& other);
  // TODO: Move constructor???
};

///////////////////////////////////////////////////////////////////////////////
// FixedHashMap : fixed size hash map containter
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
template < typename Type, usize Capacity, bool allowMultiple >
struct FixedHashMap : public HashMapInterface< Type >
{
  const bool allowMultiple_{allowMultiple};

  FixedHashMap() = delete;
  explicit FixedHashMap(const Type& init);
  explicit FixedHashMap(const FixedHashMap& other);
  FixedHashMap& operator=(const FixedHashMap& other);
  // TODO: Move constructor???
};

///////////////////////////////////////////////////////////////////////////////
// HashMap : dynamic hash map containter
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
template < typename Type, typename Allocator, bool allowMultiple >
struct HashMap : public HashMapInterface< Type >
{
  const bool allowMultiple_{allowMultiple};

  HashMap() = delete;
  explicit HashMap(Allocator& alloc, const Type& init);
  explicit HashMap(const HashMap& other);
  HashMap& operator=(const HashMap& other);
  // TODO: Move constructor???
};

} // end namespace Montreal

#endif // HASHMAP_HPP
