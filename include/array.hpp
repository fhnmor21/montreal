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
  explicit ArrayInterface(ArrayInterface& other);
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
ArrayInterface< Type >::ArrayInterface(ArrayInterface& other)
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

} // end namespace Montreal

#endif // ARRAY_HPP
