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

/**
  * composable memory allocators with diverse strategies
  *

  EXAMPLE composite allocator:
  using StkAllocator = StackAllocator< 262144, 64 >; // 256kB
  using FLAllocator0 = Freelist< StkAllocator, 0, 64, 4096 >;
  using FLAllocator1 = Freelist< FLAllocator0, 65, 128, 2048 >;
  using FLAllocator2 = Freelist< FLAllocator1, 129, 256, 1024 >;
  using FLAllocator3 = Freelist< FLAllocator2, 257, 512, 512 >;
  using FLAllocator4 = Freelist< FLAllocator3, 513, 1024, 256 >;
  using FLAllocator5 = Freelist< FLAllocator4, 1025, 2048, 128 >;
  using FLAllocator6 = Freelist< FLAllocator5, 2049, 4096, 64 >;
  using FLAllocator7 = Freelist< FLAllocator6, 4097, 8192, 32 >;
  using PrimaryAlloc = Selector< 8192, FLAllocator7, MAllocator< 1 > >;
  using CompAllocator = FallbackAllocator< PrimaryAlloc, MAllocator< 2 > >;


  FIXME: The use of free lists may lead to fragmentation.
  Needs a way to have shorter freelists and to recover part of the memory.
  Perhaps make the free list check if the chunk being deallocated is
  contiguous to some other and if so merge them and buble them up.
  Ideally all free space should be kept as the largest possible
  chunk sizes!
*/

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include "functions.hpp"
#include <algorithm>

namespace Montreal
{

// allocator based on ideas from Alexandrescu:
// https://github.com/CppCon/CppCon2015/tree/master/Presentations/allocator%20Is%20to%20Allocation%20what%20vector%20Is%20to%20Vexation

// memory block
struct Blk
{
  void* ptr;
  usize size;
};

///////////////////////////////////////////////////////////////////////////////
// Fallback: try primary alloc, if alloc fails try secondary
///////////////////////////////////////////////////////////////////////////////

template < class Primary, class Fallback >
class FallbackAllocator : private Primary, private Fallback
{
public:
  Blk allocate(usize n);
  void deallocate(Blk b);
  bool owns(Blk b);
};

// allocate chunk of certain size into memory block
// @param n size of memory chunk
// @return allocated memory block
template < class Primary, class Fallback >
Blk FallbackAllocator< Primary, Fallback >::allocate(usize n)
{
  Blk r = Primary::allocate(n);
  if(!r.ptr)
  {
    r = Fallback::allocate(n);
  }
  return r;
}

// deallocate chunk described by block
// @param b memory block
template < class Primary, class Fallback >
void FallbackAllocator< Primary, Fallback >::deallocate(Blk b)
{
  if(Primary::owns(b))
  {
    Primary::deallocate(b);
  }
  else
  {
    Fallback::deallocate(b);
  }
}

// check if the chunk is owned by this allocator
// @param b memory block
// @return true -> owns | false -> does not own
template < class Primary, class Fallback >
bool FallbackAllocator< Primary, Fallback >::owns(Blk b)
{
  return Primary::owns(b) || Fallback::owns(b);
}

///////////////////////////////////////////////////////////////////////////////
// Selector: Sizes ≤ threshold goes to SmallAllocator, else to LargeAllocator
///////////////////////////////////////////////////////////////////////////////

template < usize threshold, class SmallAllocator, class LargeAllocator >
class Selector : private SmallAllocator, private LargeAllocator
{
public:
  Blk allocate(usize n);
  void deallocate(Blk b);
  bool owns(Blk b);
};

// allocate chunk of certain size into memory block
// @param n size of memory chunk
// @return allocated memory block
template < usize threshold, class SmallAllocator, class LargeAllocator >
Blk Selector< threshold, SmallAllocator, LargeAllocator >::allocate(usize n)
{
  Blk r;
  if(threshold > n)
  {
    r = SmallAllocator::allocate(n);
    return r;
  }
  r = LargeAllocator::allocate(n);
  return r;
}

// deallocate chunk described by block
// @param b memory block
template < usize threshold, class SmallAllocator, class LargeAllocator >
void Selector< threshold, SmallAllocator, LargeAllocator >::deallocate(Blk b)
{
  if(SmallAllocator::owns(b))
  {
    SmallAllocator::deallocate(b);
  }
  else
  {
    LargeAllocator::deallocate(b);
  }
}

// check if the chunk is owned by this allocator
// @param b memory block
// @return true -> owns | false -> does not own
template < usize threshold, class SmallAllocator, class LargeAllocator >
bool Selector< threshold, SmallAllocator, LargeAllocator >::owns(Blk b)
{
  return SmallAllocator::owns(b) || LargeAllocator::owns(b);
}

///////////////////////////////////////////////////////////////////////////////
// Freelist: Keeps list of previous allocations of any given size
///////////////////////////////////////////////////////////////////////////////

template < class Parent, usize minSize, usize maxSize, usize maxBlocks >
class Freelist : private Parent
{
public:
  Freelist()
      : Parent()
      , root_{nullptr}
  {
  }

  Blk allocate(usize n);
  void deallocate(Blk b);
  bool owns(Blk b);

private:
  Freelist(Freelist& other) = delete;
  Freelist& operator=(Freelist& other) = delete;
  struct Node
  {
    Node* next;
  };

  Node* root_;
  usize countDown_{maxBlocks};
};

// allocate chunk of certain size into memory block
// @param n size of memory chunk
// @return allocated memory block
template < class Parent, usize minSize, usize maxSize, usize maxBlocks >
Blk Freelist< Parent, minSize, maxSize, maxBlocks >::allocate(usize n)
{
  if(n >= minSize && n <= maxSize && (countDown_ < maxBlocks) && root_)
  {
    Blk b = {static_cast< void* >(this->root_), n};
    root_ = static_cast< Node* >(root_->next);
    ++countDown_;
    return b;
  }
  return Parent::allocate(n);
}

// deallocate chunk described by block
// @param b memory block
template < class Parent, usize minSize, usize maxSize, usize maxBlocks >
void Freelist< Parent, minSize, maxSize, maxBlocks >::deallocate(Blk b)
{
  if(b.size < minSize || b.size > maxSize || countDown_ == 0)
  {
    Parent::deallocate(b);
  }
  else
  {
    auto p = static_cast< Node* >(b.ptr);
    p->next = root_;
    root_ = p;
    --countDown_;
  }
}

// check if the chunk is owned by this allocator
// @param b memory block
// @return true -> owns | false -> does not own
template < class Parent, usize minSize, usize maxSize, usize maxBlocks >
bool Freelist< Parent, minSize, maxSize, maxBlocks >::owns(Blk b)
{
  return (b.size >= minSize && b.size < maxSize) || Parent::owns(b);
}

///////////////////////////////////////////////////////////////////////////////
// MAllocator: simple wraper around malloc to keep the interface consistent
///////////////////////////////////////////////////////////////////////////////

// if Mallocator is used more than once in an allocator composite, the id of each
// component MAllocator has to be different to avoid ambiguity @ template resolution
template < u8 id >
class MAllocator
{
public:
  MAllocator() {}
  Blk allocate(usize n);
  void deallocate(Blk b);
  bool owns(Blk b);

private:
  MAllocator(MAllocator& other) = delete;
  MAllocator& operator=(const MAllocator& other) = delete;
};

// allocate chunk of certain size into memory block
// @param n size of memory chunk
// @return allocated memory block
template < u8 id >
Blk MAllocator< id >::allocate(usize n)
{
  Blk r;
  r.ptr = std::malloc(n * sizeof(char));
  r.size = n;

  return r;
}

// deallocate chunk described by block
// @param b memory block
template < u8 id >
void MAllocator< id >::deallocate(Blk b)
{
  std::free(b.ptr);
}

// check if the chunk is owned by this allocator
// @param b memory block
// @return true -> owns | false -> does not own
template < u8 id >
bool MAllocator< id >::owns(Blk b)
{
  // no way to control this
  // other allocators should use this as fall back or provider
  return false;
}

///////////////////////////////////////////////////////////////////////////////
// Stack: Use static array (compile time allocation)
// and stack semantics to allocate memory
///////////////////////////////////////////////////////////////////////////////

template < usize size, usize minBlock >
class StackAllocator
{
public:
  StackAllocator()
      : data_{}
      , pointer_{data_}
  {
  }
  Blk allocate(usize n);
  void deallocate(Blk b);
  bool owns(Blk b);

private:
  StackAllocator(StackAllocator& other) = delete;
  StackAllocator& operator=(const StackAllocator& other) = delete;

  char data_[size];
  char* pointer_;
};

// allocate chunk of certain size into memory block
// @param n size of memory chunk
// @return allocated memory block
template < usize size, usize minBlock >
Blk StackAllocator< size, minBlock >::allocate(usize n)
{
  auto nn = std::max(roundToPow2(n), minBlock);
  if(nn > (data_ + size) - pointer_)
  {
    return {nullptr, 0};
  }
  Blk result = {pointer_, n};
  pointer_ += nn;
  return result;
}

// deallocate chunk described by block
// @param b memory block
template < usize size, usize minBlock >
void StackAllocator< size, minBlock >::deallocate(Blk b)
{
  if(static_cast< char* >(b.ptr) + roundToPow2(b.size) == pointer_)
  {
    pointer_ = static_cast< char* >(b.ptr);
  }
}

// check if the chunk is owned by this allocator
// @param b memory block
// @return true -> owns | false -> does not own
template < usize size, usize minBlock >
bool StackAllocator< size, minBlock >::owns(Blk b)
{
  return b.ptr >= this->data_ && b.ptr < this->data_ + size;
}

///////////////////////////////////////////////////////////////////////////////
// BitMapAllocator: uses malloc to get a big chunck and manages its use
// in a memory pool using a bitmap
///////////////////////////////////////////////////////////////////////////////

// TODO: change to allow allocating a range of chunk sizes!
template < usize size, usize block >
class BitMapAllocator
{
public:
  BitMapAllocator()
      : data_{nullptr}
      , pointer_{nullptr}
      , freeChuncks_{0}
      , map_{}
  {
    this->freeChuncks_ = static_cast< u32 >(size / block);
    this->data_ = static_cast< char* >(std::calloc(size, sizeof(char)));
    this->pointer = this->data_;
    std::fill(this->map, this->map + this->freeChuncks_, 0);
  }
  ~BitMapAllocator()
  {
    assert(this->freeChuncks_ == static_cast< u32 >(size / block));
    std::free(static_cast< void* >(this->data_));
  }

  Blk allocate(usize n);
  void deallocate(Blk b);
  bool owns(Blk b);

private:
  BitMapAllocator(BitMapAllocator& other) = delete;
  BitMapAllocator& operator=(const BitMapAllocator& other) = delete;

  char* data_;
  char* pointer_;
  u32 freeChuncks_;
  u8 map_[static_cast< u32 >(size / block) >> 3];

  const u8 bitMask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
};

// allocate chunk of certain size into memory block
// @param n size of memory chunk
// @return allocated memory block
template < usize size, usize block >
Blk BitMapAllocator< size, block >::allocate(usize n)
{
  usize nn = roundToPow2(n);
  if(nn == block && this->freeChuncks_)
  {
    Blk r;
    // free block at the end of the chunk
    if(this->pointer_ < (this->data_ + size))
    {
      r.ptr = static_cast< void* >(this->pointer_);
      r.size = n;
      u32 offset = static_cast< u32 >((this->pointer_ - this->data_) / block);
      u8 bitset = offset % 8;
      offset = offset >> 3;
      this->map_[offset] |= bitMask[bitset];
      this->pointer_ += nn;
      --(this->freeChuncks_);
      return r;
    }
    else
    // needs to find a free block
    {
      int offset = 0;
      for(; offset < (static_cast< u32 >(size / block) >> 3); ++offset)
      {
        u8 bmap = this->map_[offset];
        if(bmap < 0xff)
        {
          u8 rev = ~bmap;
          u8 bitset = 0x07;
          while(bitset <= 0x00)
          {
            u8 mask = bitMask[bitset];
            if(mask & rev)
            {
              u32 delta = (offset << 3);
              delta += bitset;
              r.ptr = static_cast< void* >(this->data_ + delta);
              r.size = n;
              this->pointer_ += nn;
              this->map_[offset] |= mask;
              --(this->freeChuncks_);
              return r;
            }
            --bitset;
          }
        }
      }
    }
  }
  return {nullptr, 0};
}

// deallocate chunk described by block
// @param b memory block
template < usize size, usize block >
void BitMapAllocator< size, block >::deallocate(Blk b)
{
  u32 offset = static_cast< u32 >((static_cast< char* >(b.ptr) - this->data_) / block);
  u8 bitset = offset % 8;
  offset = offset >> 3;
  this->map_[offset] &= (~bitMask[bitset]);
}

// check if the chunk is owned by this allocator
// @param b memory block
// @return true -> owns | false -> does not own
template < usize size, usize block >
bool BitMapAllocator< size, block >::owns(Blk b)
{
  return static_cast< char* >(b.ptr) >= data_ && static_cast< char* >(b.ptr) < this->data_ + size;
}

///////////////////////////////////////////////////////////////////////////////
// ObjectPool pre-alloc a lot of objects and recycle them when no longer needed
///////////////////////////////////////////////////////////////////////////////

template < typename Type, usize preAllocateAmount >
class ObjectPool
    : public Freelist<
          FallbackAllocator< StackAllocator< preAllocateAmount * sizeof(Type), sizeof(Type) >,
                             MAllocator< 0 > >,
          sizeof(Type),
          sizeof(Type),
          preAllocateAmount >
{
};

///////////////////////////////////////////////////////////////////////////////
// helper function to allocate a ptr to a specific type
///////////////////////////////////////////////////////////////////////////////

template < typename Type, typename Allocator >
Type* allocateType(Allocator& alloc, Blk& b, const usize amount)
{
  if(b.ptr == nullptr)
  {
    b = alloc.allocate(amount * sizeof(Type));
    return static_cast< Type* >(b.ptr);
  }
  return nullptr;
}

} // end namespace Montreal

#endif // MEMORY_HPP
