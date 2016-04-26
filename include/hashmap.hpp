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
#include "hash.hpp"
#include "pointers.hpp"

namespace Montreal
{
// forward declarations
struct Allocator;

///////////////////////////////////////////////////////////////////////////////
// HashMap
///////////////////////////////////////////////////////////////////////////////

// TODO: Implement
template < typename KeyType, typename EntryType >
struct HashMapInterface
{
  struct ElementType
  {
    KeyType key;
    EntryType value;
  };

  ElementType init_;
  usize length_;

  virtual ~HashMapInterface() {}
  HashMapInterface() = delete;
  explicit HashMapInterface(const ElementType& init);
  explicit HashMapInterface(const HashMapInterface& other);
  // TODO: Move constructor???
};

// default constructor
template < typename KeyType, typename EntryType >
HashMapInterface< KeyType, EntryType >::HashMapInterface(
    const typename HashMapInterface< KeyType, EntryType >::ElementType& init)
    : init_(init)
    , length_(0)
{
}

// copy constructor
template < typename KeyType, typename EntryType >
HashMapInterface< KeyType, EntryType >::HashMapInterface(
    const HashMapInterface< KeyType, EntryType >& other)
    : init_(other.init_)
    , length_(other.length_)
{
}

///////////////////////////////////////////////////////////////////////////////
// FixedHashMap : fixed size hash map containter
///////////////////////////////////////////////////////////////////////////////

template < typename KeyType, typename EntryType, usize Capacity >
struct FixedHashMap : public HashMapInterface< KeyType, EntryType >
{
  FixedArray< typename HashMapInterface< KeyType, EntryType >::ElementType, Capacity > data_;

  virtual ~FixedHashMap() {}
  FixedHashMap() = delete;
  explicit FixedHashMap(const typename HashMapInterface< KeyType, EntryType >::ElementType& init);
  explicit FixedHashMap(const FixedHashMap& other);
  FixedHashMap& operator=(const FixedHashMap& other);
  // TODO: Move constructor???
};

// constructor
template < typename KeyType, typename EntryType, usize Capacity >
FixedHashMap< KeyType, EntryType, Capacity >::FixedHashMap(
    const typename HashMapInterface< KeyType, EntryType >::ElementType& init)
    : HashMapInterface< KeyType, EntryType >(init)
{
}

// copy constructor
template < typename KeyType, typename EntryType, usize Capacity >
FixedHashMap< KeyType, EntryType, Capacity >::FixedHashMap(
    const FixedHashMap< KeyType, EntryType, Capacity >& other)
    : HashMapInterface< KeyType, EntryType >(other)
{
  // FIXME: copy entries and re-hash
}

// assignement operator
template < typename KeyType, typename EntryType, usize Capacity >
FixedHashMap< KeyType, EntryType, Capacity >& FixedHashMap< KeyType, EntryType, Capacity >::
operator=(const FixedHashMap< KeyType, EntryType, Capacity >& other)
{
  this->init_ = other.init_;
  this->data_ = {0, this->init_, nullptr};
  // FIXME: copy entries and re-hash
  this->length_ = other.length_;

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
// HashMap : dynamic hash map containter
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
template < typename KeyType, typename EntryType, typename Allocator >
struct HashMap : public HashMapInterface< KeyType, EntryType >
{

  HashMap() = delete;
  explicit HashMap(Allocator& alloc,
                   const typename HashMapInterface< KeyType, EntryType >::ElementType& init);
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
template < typename KeyType, typename EntryType >
inline usize len(HashMapInterface< KeyType, EntryType >& container)
{
  return container.length_;
}

// clear the container
// @param container
template < typename KeyType, typename EntryType >
inline void clear(HashMapInterface< KeyType, EntryType >& container)
{
  clear(container.data_);
  container.length_ = 0;
}

// count number of elements that have key
// @param container container to access
// @param key       key of element to access
// return pointer to the element at the container required position
template < typename KeyType, typename EntryType >
inline usize count(HashMapInterface< KeyType, EntryType >& container, const KeyType& key)
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
template < typename KeyType, typename EntryType >
inline EntryType* find(HashMapInterface< KeyType, EntryType >& container, const KeyType& key)
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
template < typename KeyType, typename EntryType >
inline bool emplace(HashMapInterface< KeyType, EntryType >& container,
                    const KeyType& key,
                    const EntryType& entry)
{
  const usize cap = container.data_.capacity();
  if(container.length_ < cap)
  {
    // TODO: insert the entry
    typename HashMapInterface< KeyType, EntryType >::ElementType el;
    el.key = key;
    el.value = entry;
    Hashes hash01 = hash(key);

    for(u8 i = 0; i < 3; i++)
    {
      u32 k = hash01.h[i] % cap;
      typename HashMapInterface< KeyType, EntryType >::ElementType& old =
          Deref(at(container.data_, k));
      if(old.key == container.init_.key)
      {
        // free slot add item and break loop

        return true;
      }
      else if(old.key == key)
      {
        // slot is taken by previous entry for this key
        // overwrite and break loop

        return true;
      }
      else if(old.key != key)
      {
        // slot is taken by entry with different key key
        // continue loop to try next hash

        continue;
      }
    }

    // if we got to this point without find a place for the item
    // we'll need to move things around!
    // TODO
  }
  return false;
}

// remove an element from container
// @param container container to access
// @param key       key of element to remove
// return pointer to the element at the container required position
template < typename KeyType, typename EntryType >
inline bool remove(HashMapInterface< KeyType, EntryType >& container, KeyType key)
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
