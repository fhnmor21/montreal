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

#ifndef LIST_HPP
#define LIST_HPP

#include <algorithm>

#include "basic_types.hpp"

namespace Montreal
{
///////////////////////////////////////////////////////////////////////////////
// List
///////////////////////////////////////////////////////////////////////////////

template < typename Type >
struct ListInterface
{
  struct ElementType
  {
    Type data;
    ElementType* next;
    ElementType* prev;
  };

  usize length_;
  ElementType* head_;
  ElementType* tail_;
  ElementType* free_;

  virtual ~ListInterface(){};
  ListInterface();
  explicit ListInterface(ListInterface& other);
  ListInterface& operator=(const ListInterface& other);
};

// default constructor
template < typename Type >
ListInterface< Type >::ListInterface()
    : length_{0}
    , head_{nullptr}
    , tail_{nullptr}
    , free_{nullptr}
{
}

// copy constructor
template < typename Type >
ListInterface< Type >::ListInterface(ListInterface& other)
    : length_{0}
    , head_{nullptr}
    , tail_{nullptr}
    , free_{nullptr}
{
}

// assignement operator
template < typename Type >
ListInterface< Type >& ListInterface< Type >::operator=(const ListInterface< Type >& other)
{
  this->length_ = 0;
  typename ListInterface< Type >::ElementType* otherElement = &(other.head_);
  for(usize i = 0; i < other.length_; i++, this->length_++)
  {
    if(this->free_ && otherElement)
    {
      // update this
      typename ListInterface< Type >::ElementType* newElement = this->free_;
      this->free_ = this->free_->next;
      newElement->data = otherElement->data;
      newElement->next = nullptr;
      newElement->prev = this->tail_;
      this->tail_->next = newElement;
      this->tail_ = newElement;
    }
    // move other to next
    otherElement = otherElement->next;
  }
}

///////////////////////////////////////////////////////////////////////////////
// FixedList : Fixed size double linked list
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
template < typename Type, usize Capacity >
struct FixedList : public ListInterface< Type >
{

  FixedList() = delete;
  explicit FixedList(const Type& init);
  FixedList(const FixedList& other);
  FixedList& operator=(const FixedList& other);
  // TODO: Move constructor???
  virtual ~FixedList() {}
};

///////////////////////////////////////////////////////////////////////////////
// List : dynamic double linked list
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
template < typename Type, usize Capacity >
struct List : public ListInterface< Type >
{
  List() = delete;
  explicit List(const Type& init);
  List(const List& other);
  List& operator=(const List& other);
  // TODO: Move constructor???
  virtual ~List() {}
};

///////////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////////

// List accessor functions
// ----------------------------------------------------------------------------

// random accessor
// @param container container to access
// @param pos       position to access
// return pointer to the element at the container required position
template < typename Type >
inline typename ListInterface< Type >::ElementType* at(ListInterface< Type >& container, usize pos)
{
  if(container.head_ && container.Length_ >= pos)
  {
    // search for element in the position
    // FIXME: if pos > (container.Length_/2) then better to search backwards
    typename ListInterface< Type >::ElementType* iterator = container.head_;
    usize cursor = 0;
    do
    {
      if(cursor == pos)
      {
        return iterator;
      }
      else
      {
        iterator = iterator->next;
        ++cursor;
      }
    } while(iterator != nullptr && cursor < pos);
  }
  return nullptr;
}

// length of the container
// @param   container
// @return  size
template < typename Type >
inline usize len(ListInterface< Type >& container)
{
  return container.length_;
}

// clear the container
// @param container
template < typename Type >
inline void clear(ListInterface< Type >& container)
{
  container.length_ = 0;
  if(container.head_)
  {
    if(!container.free_)
    {
      container.free_ = container.head_;
      container.head_ = nullptr;
    }
    else
    {
      container.free_->next = container.head_;
      container.head_->prev = container.free_;
      container.head_ = nullptr;
    }
  }
  container.tail_ = nullptr;
}

// push the element to the front of the container (copy the content)
// NOTE: the element has to be copyable!!!
// @param container
// @param el  element to be copied at the container
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode pushFront(ListInterface< Type >& container, const Type& element)
{
  if(container.free_)
  {
    typename ListInterface< Type >::ElementType* newElement = container.free_;
    container.free_ = container.free_->next;
    newElement->data = element;
    newElement->next = container.head_;
    newElement->prev = nullptr;
    container.head_->prev = newElement;
    container.head_ = newElement;

    ++container.length_;

    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

// push the element to the back of the container (copy the content)
// NOTE: the element has to be copyable!!!
// @param container
// @param el  element to be copied at the container
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode pushBack(ListInterface< Type >& container, const Type& element)
{
  if(container.free_)
  {
    typename ListInterface< Type >::ElementType* newElement = container.free_;
    container.free_ = container.free_->next;
    newElement->data = element;
    newElement->next = nullptr;
    newElement->prev = container.tail_;
    container.tail_->next = newElement;
    container.tail_ = newElement;

    ++container.length_;

    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

// pop the element from the front of the container.
// NOTE: the element will be destroyed - pointers to this element will be lost
// @param container
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode popFront(ListInterface< Type >& container)
{
  if(container.head_)
  {
    typename ListInterface< Type >::ElementType* iterator = container.head_;

    container.head_ = iterator->next;
    container.free_->prev = iterator;
    iterator->next = container.free_;
    iterator->prev = nullptr;
    container.free = iterator;

    --container.length_;

    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

// pop the element from the back of the container.
// NOTE: the element will be destroyed - pointers to this element will be lost
// @param container
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode popBack(ListInterface< Type >& container)
{
  if(container.tail_)
  {
    typename ListInterface< Type >::ElementType* iterator = container.tail_;

    container.tail_ = iterator->prev;
    container.free_->prev = iterator;
    iterator->next = container.free_;
    iterator->prev = nullptr;
    container.free = iterator;

    --container.length_;

    return NO_ERROR;
  }
  return UNKNOWN_ERROR;
}

// get the element from the front of the container.
// @param container
// return pointer to the element at the front of the container
template < typename Type >
inline typename ListInterface< Type >::ElementType* front(ListInterface< Type >& container)
{
  return container.head_;
}

// get the element from the back of the container.
// @param container
// return pointer to the element at the front of the container
template < typename Type >
inline typename ListInterface< Type >::ElementType* back(ListInterface< Type >& container)
{
  return container.tail_;
}

// insert one element from the container.
// NOTE: the element has to be copyable!!!
// @param container
// @param element element to be inserted (will be copied)
// @param postion to include the element
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode insert(ListInterface< Type >& container,
                        const typename ListInterface< Type >::ElementType& element,
                        usize pos)
{
  if(container.free_ && container.head_ && (container.Length_ >= pos))
  {
    // search for element in the position
    // FIXME: if pos > (container.Length_/2) then better to search backwards
    typename ListInterface< Type >::ElementType* iterator = container.head_;
    usize cursor = 0;
    while(cursor < pos && iterator)
    {
      iterator = iterator->next;
      ++cursor;
    }

    if(iterator)
    {
      typename ListInterface< Type >::ElementType* newElement = container.free_;
      container.free_ = container.free_->next;
      newElement->data = element;
      newElement->next = iterator;
      newElement->prev = iterator->prev;
      iterator->prev = newElement;
      newElement->prev->next = newElement;

      ++container.length_;

      return NO_ERROR;
    }
  }
  return UNKNOWN_ERROR;
}

// remove one element from the container.
// NOTE: the element to be removed has to be a pointer to the position
// @param container
// @param element pointer to the element to be removed
// return ErrorCode (FIXME: so far there are only 2 possible error code values)
template < typename Type >
inline ErrorCode remove(ListInterface< Type >& container,
                        const typename ListInterface< Type >::ElementType* element)
{
  if(container.head_)
  {
    typename ListInterface< Type >::ElementType* iterator = container.head_;
    if(iterator == element)
    {
      container.head_ = iterator->next;
      container.free_->prev = iterator;
      iterator->next = container.free_;
      iterator->prev = nullptr;
      container.free = iterator;
      --container.length_;

      return NO_ERROR;
    }
    else
    {
      do
      {
        iterator = iterator->next;
        if(iterator == element)
        {
          iterator->prev->next = iterator->next;
          iterator->next->prev = iterator->prev;
          container.free_->prev = iterator;
          iterator->next = container.free_;
          iterator->prev = nullptr;
          container.free = iterator;
          --container.length_;

          return NO_ERROR;
        }
      } while(iterator != container.tail);
    }
  }
  return UNKNOWN_ERROR;
}

} // end namespace Montreal

#endif // LIST_HPP
