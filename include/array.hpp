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

#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <algorithm>

#include "basic_types.hpp"
#include "functions.hpp"
#include "memory.hpp"
#include "modulus.hpp"

namespace Montreal
{

///////////////////////////////////////////////////////////////////////////////
// Array
///////////////////////////////////////////////////////////////////////////////

template < typename Type >
struct ArrayInterface
{
  using ElementType = Type;

  virtual usize capacity() = 0;
  virtual usize initialLen() = 0;

  usize firstPos_;
  usize lastPos_;
  usize length_;
  Type* array_;
  Type init_;

  virtual ~ArrayInterface() {}
  ArrayInterface();
  explicit ArrayInterface(const ArrayInterface& other);
  ArrayInterface& operator=(const ArrayInterface& other);
};

// default constructor
template < typename Type >
ArrayInterface< Type >::ArrayInterface()
    : firstPos_{0}
    , lastPos_{0}
    , length_{0}
    , array_{nullptr}
    , init_{}
{
}

// copy constructor
template < typename Type >
ArrayInterface< Type >::ArrayInterface(const ArrayInterface< Type >& other)
    : firstPos_{other.firstPos}
    , lastPos_{other.lastPos_}
    , length_{other.length_}
    , array_{nullptr}
    , init_{}
{
}

// assignement operator
template < typename Type >
ArrayInterface< Type >& ArrayInterface< Type >::operator=(const ArrayInterface< Type >& other)
{
  this->firstPos_ = other.firstPos;
  this->lastPos_ = other.lastPos_;
  this->length_ = other.length_;
  this->array_ = nullptr;
  this->init_ = other.init_;
}

///////////////////////////////////////////////////////////////////////////////
// FixedArray : array with fixed size
///////////////////////////////////////////////////////////////////////////////

template < typename Type, usize Capacity >
struct FixedArray : public ArrayInterface< Type >
{
  using ElementType = Type;
  GLOBAL const usize capacity_;

  usize initialLen_;
  Type buffer_[Capacity];

  virtual usize capacity() override { return capacity_; };
  virtual usize initialLen() override { return initialLen_; };

  FixedArray() = delete;
  explicit FixedArray(const Type& init);
  FixedArray(const FixedArray& other);
  FixedArray& operator=(const FixedArray& other);
  // TODO: Move constructor???
  virtual ~FixedArray() {}
};

// GLOBAL
template < typename Type, usize Capacity >
const usize FixedArray< Type, Capacity >::capacity_{Capacity};

// constructor - initilized
template < typename Type, usize Capacity >
FixedArray< Type, Capacity >::FixedArray(const Type& init)
    : ArrayInterface< Type >()
    , initialLen_{Capacity}
    , buffer_{}
{
  // ArrayInterface initializes firstPos_=0, lastPos_=0, length_=0, array_=nullptr
  this->lastPos_ = Capacity;
  this->length_ = Capacity;
  this->array_ = this->buffer_;
  std::fill(this->array_, (this->array_ + this->capacity_), init);
}

// copy constructor
template < typename Type, usize Capacity >
FixedArray< Type, Capacity >::FixedArray(const FixedArray< Type, Capacity >& other)
    : ArrayInterface< Type >(other)
    , initialLen_{other.initialLen_}
    , buffer_{}
{
  // ArrayInterface copies firstPos_=other, lastPos_=other, length_=other, array_=nullptr
  this->array_ = this->buffer_;
  std::copy(other.array_, (other.array_ + this->capacity_), this->array_);
}

// assignement operator
template < typename Type, usize Capacity >
FixedArray< Type, Capacity >& FixedArray< Type, Capacity >::
operator=(const FixedArray< Type, Capacity >& other)
{
  std::copy(
      other.array_, (other.array_ + std::min(other.capacity_, this->capacity_)), this->array_);
  this->firstPos_ = other.firstPos_;
  this->lastPos_ = other.lastPot_;
  this->length_ = std::min(other.length_, this->length_);
  this->initialLen_ = std::min(other.initialLen_, this->initialLen_);
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
// Array : dynamic array with random accessor
///////////////////////////////////////////////////////////////////////////////

template < typename Type, typename Allocator >
struct Array : public ArrayInterface< Type >
{
  using ElementType = Type;
  using AllocatorType = Allocator;

  Allocator& alloc_;
  usize capacity_;
  usize initialLen_;
  Blk memBlock_;
  Array() = delete;

  // these are used by the base class
  virtual usize capacity() override { return capacity_; };
  virtual usize initialLen() override { return initialLen_; };

  // destructors and constructors
  Array(Allocator& alloc, const Type& init, const usize Capacity);
  explicit Array(const Array& other);
  Array& operator=(const Array& other);
  // TODO: Move constructor???
  virtual ~Array();
};

// default virtual destructor
template < typename Type, typename Allocator >
Array< Type, Allocator >::~Array()
{
  if(this->memBlock_.ptr)
  {
    this->alloc_.deallocate(this->memBlock_);
  }
}

// constructor
template < typename Type, typename Allocator >
Array< Type, Allocator >::Array(Allocator& alloc, const Type& init, const usize Capacity)
    : ArrayInterface< Type >()
    , alloc_{alloc}
    , capacity_{Capacity}
    , initialLen_{Capacity}
    , memBlock_{nullptr, 0}
{
  this->init_ = init;
  this->array_ = allocateType< Type, Allocator >(this->alloc_, this->memBlock_, this->capacity_);
  this->firstPos_ = 0;
  this->lastPos_ = Capacity;
  this->length_ = Capacity;

  std::fill(this->array_, (this->array_ + this->capacity_), this->init_);
}

// copy constructor
template < typename Type, typename Allocator >
Array< Type, Allocator >::Array(const Array& other)
    : ArrayInterface< Type >(other)
    , alloc_{other.alloc}
    , capacity_{other.capacity_}
    , initialLen_{other.capacity_}
    , memBlock_{nullptr, 0}
{
  this->firstPos_ = other.firstPos_;
  this->lastPos_ = other.capacity_;
  this->length_ = other.capacity_;
  this->init_ = other.init_;
  this->array_ = allocateType(this->alloc_, this->memBlock_, this->capacity_);
  std::copy(other.array_, (other.array_ + this->capacity_), this->array_);
}

// assignement operator
template < typename Type, typename Allocator >
Array< Type, Allocator >& Array< Type, Allocator >::operator=(const Array& other)
{
  if(this->memBlock_.ptr)
  {
    this->alloc_.deallocate(this->memBlock);
  }

  this->alloc_ = other.alloc;
  this->firstPos_ = other.firstPos_;
  this->lastPos_ = other.lastPos_;
  this->length_ = other.length_;
  this->capacity_ = other.capacity_;
  this->initialLen_ = other.capacity_;
  this->init_ = other.init_;
  this->memBlock_ = {nullptr, 0};

  this->array_ = allocateType(this->alloc_, this->memBlock_, this->capacity_);
  std::copy(other.array_, (other.array_ + this->capacity_), this->array_);

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
// BitMap : Fixed size Array of bits with random accessor
///////////////////////////////////////////////////////////////////////////////

template < usize Capacity >
using FixedBitMap = FixedArray< u8, (Capacity >> 3) >;

///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

// Array acessor functions
// ----------------------------------------------------------------------------

// random accessor
// @param container container to access
// @param pos       position to access
// return pointer to the element at the container required position
template < typename Type >
inline Type* at(ArrayInterface< Type >& container, usize pos)
{
  m64 logicPos(container.capacity(), pos);
  if(logicPos < container.length_)
  {
    logicPos += container.firstPos_;
    return (container.array_ + logicPos.toInt());
  }
  return nullptr;
}

// length of the container
// @param   container
// @return  size
template < typename Type >
inline usize len(ArrayInterface< Type >& container)
{
  return container.length_;
}

// clear the container
// @param container
template < typename Type >
inline void clear(ArrayInterface< Type >& container)
{
  std::fill(container.array_, (container.array_ + container.capacity()), container.init);
  container.firstPos_ = 0;
  container.lastPos_ = container.initialLen();
  container.length_ = container.initialLen();
}

// FixedBitMap: special accessor
// ----------------------------------------------------------------------------

// byte accessor
// @param container container to access
// @param pos       byte position to access
// return pointer to the element at the container required position
template < usize Capacity >
inline u8 byte(FixedBitMap< Capacity >& container, usize pos)
{
  return at(container, pos);
}

// bit accessor
// @param container container to access
// @param pos       bit position to access
// return pointer to the element at the container required position
template < usize Capacity >
inline u8 bit(FixedBitMap< Capacity >& container, usize pos)
{
  const u8 bitMask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
  usize byte_ = pos >> 3;

  if(byte_ < container.length_)
  {
    usize bit_ = pos % 8;
    return (bitMask[bit_] & byte(container, byte_));
  }
  return false;
}

// bits length of the container
// @param   container
// @return  size
template < usize Capacity >
inline usize bits(FixedBitMap< Capacity >& container)
{
  return (8 * container.length_);
}

} // end namespace Montreal

#endif // ARRAY_HPP
