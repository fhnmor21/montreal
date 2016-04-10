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
// Tree
///////////////////////////////////////////////////////////////////////////////

template < typename Type >
struct TreeInterface // : Array ???should this inherite from Array???
{
  enum NodeColour
  {
    Red,
    Black,
  };

  struct ElementType
  {
    Type data;
    NodeColour colour;
    ElementType* parent;
    ElementType* left;
    ElementType* right;
  };

  cSize length_;
  ElementType* root_;
  ElementType* free_;

  virtual ~TreeInterface(){};
  TreeInterface();
  explicit TreeInterface(const TreeInterface& other);
  TreeInterface& operator=(const TreeInterface& other);
  // TODO: Move constructor???
};

// TODO: Implement constructors

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
// HashMap
///////////////////////////////////////////////////////////////////////////////

// TODO: Implement
template < typename Type >
struct HashMapInterface
{
  virtual ~HashMapInterface() {}
  HashMapInterface();
  explicit HashMapInterface(const HashMapInterface& other);
  HashMapInterface& operator=(const HashMapInterface& other);
  // TODO: Move constructor???
};

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
