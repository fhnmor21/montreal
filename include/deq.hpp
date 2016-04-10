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
  explicit DEQInterface(DEQInterface& other);
  DEQInterface& operator=(DEQInterface& other);
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
DEQInterface< Type >::DEQInterface(DEQInterface< Type >& other)
    : ArrayInterface< Type >(other)
    , canOverwrite_{other.canOverwrite_}
{
}

// assgignement operator
template < typename Type >
DEQInterface< Type >& DEQInterface< Type >::operator=(DEQInterface< Type >& other)
{
  this->canOverwrite_ = other.canOverwrite_;
}

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
