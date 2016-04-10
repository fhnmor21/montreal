/**
  * basic data container interfaces
  *
  */

#ifndef UTILS_CONTAINERS_HPP
#define UTILS_CONTAINERS_HPP

#include <algorithm>

#include "utils/types.hpp"

namespace W2E
{

namespace Utils
{

///////////////////////////////////////////////////////////////////////////////
// Trie
///////////////////////////////////////////////////////////////////////////////

// template < typename Type >
// struct TrieInterface
// {
//   virtual ~TrieInterface() {}
//   TrieInterface();
//   explicit TrieInterface(const TrieInterface& other);
//   TrieInterface& operator=(const TrieInterface& other);
//   // TODO: Move constructor???
// };

///////////////////////////////////////////////////////////////////////////////
// Heap
///////////////////////////////////////////////////////////////////////////////

// template < typename Type >
// struct HeapInterface
// {
//   virtual ~HeapInterface() {}
//   HeapInterface();
//   explicit HeapInterface(const HeapInterface& other);
//   HeapInterface& operator=(const HeapInterface& other);
//   // TODO: Move constructor???
// };

} // end namespace Utils

} // end namespace W2E

#endif // UTILS_CONTAINERS_HPP
