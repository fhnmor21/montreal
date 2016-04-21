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

#include "array.hpp"
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
  struct ElementType
  {
    u64 key;
    Type value;
    ElementType* next;
  };

  Type init_;
  usize length_;

  virtual ~HashMapInterface() {}
  HashMapInterface() = delete;
  explicit HashMapInterface(const Type& init);
  explicit HashMapInterface(const HashMapInterface& other);
  // TODO: Move constructor???
};

// default constructor
template < typename Type >
HashMapInterface< Type >::HashMapInterface(const Type& init)
    : init_(init)
    , length_(0)
{
}

// copy constructor
template < typename Type >
HashMapInterface< Type >::HashMapInterface(const HashMapInterface< Type >& other)
    : init_(other.init_)
    , length_(other.length_)
{
}

///////////////////////////////////////////////////////////////////////////////
// FixedHashMap : fixed size hash map containter
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
template < typename Type, usize Capacity >
struct FixedHashMap : public HashMapInterface< Type >
{
  FixedArray< typename HashMapInterface< Type >::ElementType, Capacity > data_;

  virtual ~FixedHashMap() {}
  FixedHashMap() = delete;
  explicit FixedHashMap(const Type& init);
  explicit FixedHashMap(const FixedHashMap& other);
  FixedHashMap& operator=(const FixedHashMap& other);
  // TODO: Move constructor???
};

// constructor
template < typename Type, usize Capacity >
FixedHashMap< Type, Capacity >::FixedHashMap(const Type& init)
    : HashMapInterface< Type >(init)
    , data_({0, init, nullptr})
{
}

// copy constructor
template < typename Type, usize Capacity >
FixedHashMap< Type, Capacity >::FixedHashMap(const FixedHashMap< Type, Capacity >& other)
    : HashMapInterface< Type >(other)
    , data_({0, other.init_, nullptr})
{
  // TODO: copy entries and re-hash
}

// assignement operator
template < typename Type, usize Capacity >
FixedHashMap< Type, Capacity >& FixedHashMap< Type, Capacity >::
operator=(const FixedHashMap< Type, Capacity >& other)
{
  this->init_ = other.init_;
  this->data_ = {0, this->init_, nullptr};
  // TODO: copy entries and re-hash
  this->length_ = other.length_;

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
// HashMap : dynamic hash map containter
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
template < typename Type, typename Allocator >
struct HashMap : public HashMapInterface< Type >
{

  HashMap() = delete;
  explicit HashMap(Allocator& alloc, const Type& init);
  explicit HashMap(const HashMap& other);
  HashMap& operator=(const HashMap& other);
  // TODO: Move constructor???
};

///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

// HashMap acessor functions
// ----------------------------------------------------------------------------

// length of the container
// @param   container
// @return  size
template < typename Type >
inline usize len(HashMapInterface< Type >& container)
{
  return container.length_;
}

// clear the container
// @param container
template < typename Type >
inline void clear(HashMapInterface< Type >& container)
{
  clear(container.data_);
  container.length_ = 0;
}

// count number of elements that have key
// @param container container to access
// @param key       key of element to access
// return pointer to the element at the container required position
template < typename Type >
inline usize count(HashMapInterface< Type >& container, u64 key)
{
  if(container.length_)
  {
    // TODO: find all the element
    // return // some value;
  }
  return 0;
}

// random accessor
// @param container container to access
// @param key       key of element to access
// return pointer to the element at the container required position
template < typename Type >
inline Type* find(HashMapInterface< Type >& container, u64 key)
{
  if(container.length_)
  {
    // TODO: find the entry
    // return // some pointer;
  }
  return nullptr;
}

// add a new element to the key
// @param container container to access
// @param key       key of element to access
// @param entry     element content
// return pointer to the element at the container required position
template < typename Type >
inline bool emplace(HashMapInterface< Type >& container, u64 key, const Type& entry)
{
  if(container.length_)
  {
    // TODO: insert the entry
    // return // some value;
  }
  return true;
}

// remove an element from container
// @param container container to access
// @param key       key of element to remove
// return pointer to the element at the container required position
template < typename Type >
inline bool remove(HashMapInterface< Type >& container, u64 key)
{
  if(container.length_)
  {
    // TODO: remove the entry
    // return // some value;
  }
  return true;
}

} // end namespace Montreal

#endif // HASHMAP_HPP
