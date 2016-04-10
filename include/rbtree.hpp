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

#ifndef RBTREE_HPP
#define RBTREE_HPP

#include "basic_types.hpp"

namespace Montreal
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

  usize length_;
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
// FixedTree : Fixed size tree containter (red-black)
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
template < typename Type, usize Capacity >
struct FixedTree : public TreeInterface< Type >
{
  FixedTree() = delete;
  explicit FixedTree(const Type& init);
  FixedTree(const FixedTree& other);
  FixedTree& operator=(const FixedTree& other);
  // TODO: Move constructor???
  virtual ~FixedTree() {}
};

///////////////////////////////////////////////////////////////////////////////
// Tree : dynamic balanced tree containter (red-black)
///////////////////////////////////////////////////////////////////////////////

// TODO: implement
template < typename Type, typename Allocator >
struct Tree : public TreeInterface< Type >
{
  Tree() = delete;
  Tree(Allocator& alloc, const Type& init);
  explicit Tree(const Tree& other);
  Tree& operator=(const Tree& other);
  // TODO: Move constructor???
  virtual ~Tree() {}
};

} // end namespace Montreal

#endif // RBTREE_HPP
