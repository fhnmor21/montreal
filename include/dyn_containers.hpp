/**
  * implementation of containers using dynamic allocation
  * NOTE: an allocator has to be passe to the containers that
  * provides the allocation interface as in utils/memory.hpp
  *
  */

#ifndef UTILS_DYN_CONTAINERS_HPP
#define UTILS_DYN_CONTAINERS_HPP

#include <algorithm>

#include "utils/containers.hpp"
#include "utils/memory.hpp"

namespace W2E
{

namespace Utils
{

///////////////////////////////////////////////////////////////////////////////
// Trie : dynamic trie containter
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
// template < typename Type >
// struct Trie : public TrieInterface
// {
//   Trie() = delete;
//   Trie(Allocator& alloc, const Type& init);
//   explicit Trie(const Trie& other);
//   Trie& operator=(const Trie& other);
//   // TODO: Move constructor???
//   virtual ~Trie() {}
// };

///////////////////////////////////////////////////////////////////////////////
// Heap : dynamic heap containter
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
// template < typename Type >
// struct Heap : public HeapInterface
// {
//   Heap() = delete;
//   explicit Heap(Allocator& alloc, const Type& init);
//   explicit Heap(const Heap& other);
//   Heap& operator=(const Heap& other);
//   // TODO: Move constructor???
// };

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_DYN_CONTAINERS_HPP
