/**
  * fixed size containers : allocated at compile time
  *
  */

#ifndef UTILS_FIX_CONTAINERS_HPP
#define UTILS_FIX_CONTAINERS_HPP

#include "utils/containers.hpp"

namespace W2E
{

namespace Utils
{

///////////////////////////////////////////////////////////////////////////////
// FixedTrie : Fixed size trie containter
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
// template < typename Type >
// struct FixedTrie : public TrieInterface
// {
//   FixedTrie() = delete;
//   explicit FixedTrie(const Type& init);
//   FixedTrie(const FixedTrie& other);
//   FixedTrie& operator=(const FixedTrie& other);
//   // TODO: Move constructor???
//   virtual ~FixedTrie() {}
// };

///////////////////////////////////////////////////////////////////////////////
// FixedHeap : fixed size heap containter
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
// template < typename Type >
// struct FixedHeap: public HeapInterface
// {
//   FixedHeap() = delete;
//   explicit FixedHeap(Allocator& alloc, const Type& init);
//   explicit FixedHeap(const FixedHeap& other);
//   FixedHeap& operator=(const FixedHeap& other);
//   // TODO: Move constructor???
// };

///////////////////////////////////////////////////////////////////////////////
// Accessor functions
///////////////////////////////////////////////////////////////////////////////

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_FIX_CONTAINERS_HPP
