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

#ifndef DEQ_HPP
#define DEQ_HPP

#include "array.hpp"

namespace Montreal
{

///////////////////////////////////////////////////////////////////////////////
// DEQ
///////////////////////////////////////////////////////////////////////////////

template < typename Type >
struct DEQInterface : ArrayInterface< Type >
{
  bool canOverwrite_;

  virtual ~DEQInterface() {}
  DEQInterface();
  explicit DEQInterface(const DEQInterface& other);
  DEQInterface& operator=(const DEQInterface& other);
};

// default constructor
template < typename Type >
DEQInterface< Type >::DEQInterface()
    : ArrayInterface< Type >()
    , canOverwrite_{false}
{
}

// copy constructor
template < typename Type >
DEQInterface< Type >::DEQInterface(const DEQInterface< Type >& other)
    : ArrayInterface< Type >(other)
    , canOverwrite_{other.canOverwrite_}
{
}

// assgignement operator
template < typename Type >
DEQInterface< Type >& DEQInterface< Type >::operator=(const DEQInterface< Type >& other)
{
  this->canOverwrite_ = other.canOverwrite_;
}

///////////////////////////////////////////////////////////////////////////////
// FixedDEQ : Fixed size double ended queue with random accessor
///////////////////////////////////////////////////////////////////////////////

template < typename Type, usize Capacity, bool canOverwrite = false >
struct FixedDEQ : public DEQInterface< Type >
{
  using ElementType = Type;
  GLOBAL const usize capacity_;

  usize initialLen_;
  Type buffer_[Capacity];

  virtual usize capacity() override { return capacity_; };
  virtual usize initialLen() override { return initialLen_; };

  FixedDEQ() = delete;
  explicit FixedDEQ(const Type& init);
  FixedDEQ(const FixedDEQ& other);
  FixedDEQ& operator=(const FixedDEQ& other);
  // TODO: Move constructor???
  virtual ~FixedDEQ() {}
};

// GLOBAL
template < typename Type, usize Capacity, bool canOverwrite >
const usize FixedDEQ< Type, Capacity, canOverwrite >::capacity_{Capacity};

// constructor - initialized
template < typename Type, usize Capacity, bool canOverwrite >
FixedDEQ< Type, Capacity, canOverwrite >::FixedDEQ(const Type& init)
    : DEQInterface< Type >()
    , initialLen_{0}
    , buffer_{}
{
  // DEQInterface inits firstPos_=0, lastPos_=0, length_=0, array_=nullptr, canOverwrite_=false
  this->canOverwrite_ = canOverwrite;
  this->array_ = this->buffer_;
  std::fill(this->array_, (this->array_ + this->capacity_), init);
}

// copy constructor
template < typename Type, usize Capacity, bool canOverwrite >
FixedDEQ< Type, Capacity, canOverwrite >::FixedDEQ(
    const FixedDEQ< Type, Capacity, canOverwrite >& other)
    : DEQInterface< Type >(other)
    , initialLen_{other.initialLen_}
    , buffer_{}
{
  // DEQInterface copies firstPos_=other, lastPos_=other, length_=other, array_=nullptr,
  // canOverwrite_=false
  this->canOveWrite_ = canOverwrite;
  this->array_ = this->buffer_;
  std::copy(other.array_, (other.array_ + this->capacity_), this->array_);
}

// assignement operator
template < typename Type, usize Capacity, bool canOverwrite >
FixedDEQ< Type, Capacity, canOverwrite >& FixedDEQ< Type, Capacity, canOverwrite >::
operator=(const FixedDEQ< Type, Capacity, canOverwrite >& other)
{
  std::copy(
      other.array_, (other.array_ + std::min(other.capacity_, this->capacity_)), this->array_);
  this->firstPos_ = other.firstPos_;
  this->lastPos_ = other.lastPot_;
  this->canOveWrite_ = other.canOverwrite_;
  this->length_ = std::min(other.length_, this->length_);
  this->initialLen_ = std::min(other.initialLen_, this->initialLen_);
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
// DEQ : dynamic double ended queue with random accessor
///////////////////////////////////////////////////////////////////////////////

template < typename Type, typename Allocator, bool canOverwrite = false >
struct DEQ : public DEQInterface< Type >
{
  using ElementType = Type;
  using AllocatorType = Allocator;

  Allocator& alloc_;
  usize capacity_;
  usize initialLen_;
  Type init_;
  Blk memBlock_;
  DEQ() = delete;

  // these are used by the base class
  virtual usize capacity() override { return capacity_; };
  virtual usize initialLen() override { return initialLen_; };

  // destructors and constructors
  DEQ(Allocator& alloc, const Type& init, const usize Capacity);
  explicit DEQ(const DEQ& other);
  DEQ& operator=(const DEQ& other);
  // TODO: Move constructor???
  virtual ~DEQ();
};

// virtual destructor
template < typename Type, typename Allocator, bool canOverwrite >
DEQ< Type, Allocator, canOverwrite >::~DEQ()
{
  if(this->memBlock_.ptr)
  {
    this->alloc_.deallocate(this->memBlock_);
  }
}

// constructor
template < typename Type, typename Allocator, bool canOverwrite >
DEQ< Type, Allocator, canOverwrite >::DEQ(Allocator& alloc, const Type& init, const usize Capacity)
    : DEQInterface< Type >()
    , alloc_{alloc}
    , capacity_{Capacity}
    , initialLen_{0}
    , init_{init}
    , memBlock_{nullptr, 0}
{
  // DEQInterface inits firstPos_=0, lastPos_=0, length_=0, array_=nullptr, canOverwrite_=false
  this->canOverwrite_ = canOverwrite;
  this->array_ = allocateType< Type, Allocator >(this->alloc_, this->memBlock_, this->capacity_);

  std::fill(this->array_, (this->array_ + this->capacity_), this->init_);
}

// copy constructor
template < typename Type, typename Allocator, bool canOverwrite >
DEQ< Type, Allocator, canOverwrite >::DEQ(const DEQ< Type, Allocator, canOverwrite >& other)
    : DEQInterface< Type >(other)
    , alloc_{other.alloc}
    , capacity_{other.capacity_}
    , initialLen_{0}
    , init_{other.init_}
    , memBlock_{nullptr, 0}
{
  // DEQInterface copies firstPos_=other, lastPos_=other, length_=other, array_=nullptr,
  // canOverwrite_=false
  this->canOverwrite_ = other.canOverwrite_;
  this->array_ = allocateType(this->alloc_, this->memBlock_, this->capacity_);
  std::copy(other.array_, (other.array_ + this->capacity_), this->array_);
}

// assignement operator
template < typename Type, typename Allocator, bool canOverwrite >
DEQ< Type, Allocator, canOverwrite >& DEQ< Type, Allocator, canOverwrite >::
operator=(const DEQ< Type, Allocator, canOverwrite >& other)
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
  this->initialLen_ = other.initialLen_;
  this->canOverwrite_ = other.canOverwrite_;
  this->init_ = other.init_;
  this->memBlock_ = {nullptr, 0};

  this->array_ = allocateType(this->alloc_, this->memBlock_, this->capacity_);
  std::copy(other.array_, (other.array_ + this->capacity_), this->array_);

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
// RingQ : fixed size, acessible back and front with overwriting
///////////////////////////////////////////////////////////////////////////////

template < typename Type, usize Capacity >
using FixedRingQ = FixedDEQ< Type, Capacity, true >;

///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

// DEQ accessor functions
// ----------------------------------------------------------------------------

// push the element to the back of the container (copy the content)
// NOTE: the element has to be copyable!!!
// @param container
// @param el  element to be copied at the container
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode pushBack(DEQInterface< Type >& container, const Type& el)
{
  if((container.length_ < container.capacity()) || container.canOverwrite_)
  {
    m64 mm(container.capacity(), 0);
    if(container.length_ > 0)
    {
      mm = container.lastPos_;
      container.lastPos_ = (++mm).toInt();
      if(container.lastPos_ == container.firstPos_)
      {
        mm = container.firstPos_;
        container.firstPos_ = (++mm).toInt();
      }
    }
    container.array_[container.lastPos_] = el;
    const usize currLen = container.length_ + 1;
    container.length_ = std::min(currLen, container.capacity());
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

// push the element to the front of the container (copy the content)
// NOTE: the element has to be copyable!!!
// @param container
// @param el  element to be copied at the container
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode pushFront(DEQInterface< Type >& container, const Type& el)
{
  m64 mm(container.capacity(), 0);
  if((container.length_ < container.capacity()) || container.canOverwrite_)
  {
    if(container.length_ > 0)
    {
      mm = container.firstPos_;
      container.firstPos_ = (--mm).toInt();
      if(container.lastPos_ == container.firstPos_)
      {
        mm = container.lastPos_;
        container.lastPos_ = (--mm).toInt();
      }
    }
    container.array_[container.firstPos_] = el;
    const usize currLen = container.length_ + 1;
    container.length_ = std::min(currLen, container.capacity());
    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

// pop the element from the back of the container.
// NOTE: the element will be destroyed - pointers to this element will be lost
// @param container
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode popBack(DEQInterface< Type >& container)
{
  m64 mm(container.capacity(), 0);
  if(container.length_ > 0)
  {
    mm = container.length_;
    container.length_ = (--mm).toInt();
    mm = container.lastPos_;
    container.lastPos_ = (--mm).toInt();

    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

// pop the element from the front of the container.
// NOTE: the element will be destroyed - pointers to this element will be lost
// @param container
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode popFront(DEQInterface< Type >& container)
{
  m64 mm(container.capacity(), 0);
  if(container.length_ > 0)
  {
    mm = container.length_;
    container.length_ = (--mm).toInt();

    mm = container.firstPos_;
    container.firstPos_ = (++mm).toInt();

    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

// get the element from the front of the container.
// @param container
// return pointer to the element at the front of the container
template < typename Type >
inline Type* front(DEQInterface< Type >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.firstPos_]);
  }
  return nullptr;
}

// get the element from the back of the container.
// @param container
// return pointer to the element at the front of the container
template < typename Type >
inline Type* back(DEQInterface< Type >& container)
{
  if(container.length_ > 0)
  {
    return &(container.array_[container.lastPos_]);
  }
  return nullptr;
}

} // end namespace Montreal

#endif // DEQ_HPP
