/**
  * helper functions to abstrct the backend
  *
  */

#ifndef STRINGS_HPP
#define STRINGS_HPP

#include <algorithm>
#include <cstring>

#include "basic_types.hpp"
#include "functions.hpp"

namespace Montreal
{

///////////////////////////////////////////////////////////////////////////////
// String : Fixed size String pool
///////////////////////////////////////////////////////////////////////////////

// forward declarations
class StringWrapper;

// allocation result data structure
struct allocResult
{
  ErrorCode err;
  char* str;
  u16* refCount;
};

// interface
class StringPoolInterface
{
public:
  virtual ~StringPoolInterface() {}

private:
  friend class StringWrapper;
  virtual allocResult allocate(const u16& length) = 0;
  virtual ErrorCode deallocate(const u16& length, char* str, u16* refCount) = 0;
};

///////////////////////////////////////////////////////////////////////////////
// Dynamic allocated string pool
///////////////////////////////////////////////////////////////////////////////

// TODO: implemente using the new allocators!
template < usize Capacity >
class DynStringPool : public StringPoolInterface
{
public:
  virtual ~DynStringPool() {}
  DynStringPool();

private:
  virtual allocResult allocate(const u16& length) override;
  virtual ErrorCode deallocate(const u16& length, char* str, u16* refCount) override;
  DynStringPool(DynStringPool&) = delete;
  DynStringPool& operator=(DynStringPool&) = delete;
};

// TODO: implement DynStrinPool

///////////////////////////////////////////////////////////////////////////////
// Fixed size string pool - compile time allocation
///////////////////////////////////////////////////////////////////////////////

template < usize Capacity >
class FixedStringPool : public StringPoolInterface
{
public:
  virtual ~FixedStringPool() {}
  FixedStringPool();

private:
  virtual allocResult allocate(const u16& length) override;
  virtual ErrorCode deallocate(const u16& length, char* str, u16* refCount) override;
  FixedStringPool(FixedStringPool&) = delete;
  FixedStringPool& operator=(FixedStringPool&) = delete;

  // data
  GLOBAL const usize maxLength_{Capacity};
  GLOBAL const usize maxElements_{maxLength_ >> 3}; // smallest string is 2^3 chars

  GLOBAL char array_[maxLength_];
  GLOBAL usize arrayCursor_;

  GLOBAL u16 refCount_[maxElements_];
  GLOBAL usize refCountCursor_;
};

// GLOBAL vars initialization
template < usize Capacity >
char FixedStringPool< Capacity >::array_[maxLength_] = {0};
template < usize Capacity >
usize FixedStringPool< Capacity >::arrayCursor_ = 0;
template < usize Capacity >
u16 FixedStringPool< Capacity >::refCount_[maxElements_] = {0};
template < usize Capacity >
usize FixedStringPool< Capacity >::refCountCursor_ = 0;

// default constructor
template < usize Capacity >
FixedStringPool< Capacity >::FixedStringPool()
{
}

template < usize Capacity >
allocResult FixedStringPool< Capacity >::allocate(const u16& length)
{
  usize len = std::max(static_cast< usize >(8), roundToPow2(length));
  if((this->maxLength_ - this->arrayCursor_) > len && (this->maxElements_ > this->refCountCursor_))
  {
    // set the refCount
    u16 mapLen = len >> 3;
    u16* refCount = &(this->refCount_[this->refCountCursor_]);
    std::fill(refCount, (refCount + mapLen), 0);
    this->refCountCursor_ += mapLen;

    // grab the string pointer and clear it
    char* str = &(this->array_[this->arrayCursor_]);
    this->arrayCursor_ += len;
    std::fill(str, (str + len), 0);

    return allocResult{NO_ERROR, str, refCount};
  }
  else
  {
    // search the refCount array and reuse freed memory
    // FIXME: this has to use other kind of allocation later!!!
    u16 mapLen = len >> 3;
    u16* pattern = new u16[mapLen];
    std::fill(pattern, (pattern + mapLen), 0);
    u16* refCount = std::search(
        this->refCount_, (this->refCount_ + this->refCountCursor_), pattern, (pattern + mapLen));
    delete[] pattern;

    if(refCount < (this->refCount_ + this->refCountCursor_))
    {
      u16 arrayCur = (refCount - this->refCount_) << 3;
      char* str = this->array_ + arrayCur;
      return allocResult{NO_ERROR, str, refCount};
    }
  }
  return allocResult{UNKNOWN_ERROR, nullptr, nullptr};
}

template < usize Capacity >
ErrorCode FixedStringPool< Capacity >::deallocate(const u16& length, char* str, u16* refCount)
{
  usize len = std::max(static_cast< usize >(8), roundToPow2(length));
  u16 mapLen = len >> 3;

  // clear refCount
  std::fill(refCount, (refCount + mapLen), 0);

  // clear string pool segment
  std::fill(str, (str + len), 0);

  return NO_ERROR;
}

///////////////////////////////////////////////////////////////////////////////
// String Wrapper container
///////////////////////////////////////////////////////////////////////////////

class StringWrapper
{
public:
  ~StringWrapper();
  explicit StringWrapper(StringPoolInterface* sp);
  explicit StringWrapper(StringPoolInterface* sp, const char* initStr);
  StringWrapper(StringWrapper& otherStr);
  // TODO: move constructor???
  StringWrapper& operator=(StringWrapper& otherStr);
  StringWrapper& operator=(const char* initStr); // copy on write
  const char operator[](u16 pos) const;
  char operator[](u16 pos); // copy on write
  const char* cStr();

private:
  // data
  StringPoolInterface* strPool_;
  u16 length_;
  u16* refCount_;
  char* str_;
};

StringWrapper::~StringWrapper()
{
  this->strPool_->deallocate(this->length_, this->str_, this->refCount_);
}

StringWrapper::StringWrapper(StringPoolInterface* sp, const char* initStr)
    : strPool_{sp}
    , length_{0}
    , refCount_{nullptr}
    , str_{nullptr}
{
  u16 inLen = std::strlen(initStr);
  u16 len = std::max(static_cast< usize >(8), roundToPow2(inLen));
  auto ret = this->strPool_->allocate(len);
  if(ret.err == NO_ERROR)
  {
    this->str_ = ret.str;
    this->refCount_ = ret.refCount;

    std::copy_n(initStr, inLen, this->str_);
  }
}

StringWrapper::StringWrapper(StringPoolInterface* sp)
    : strPool_{sp}
    , length_{0}
    , refCount_{nullptr}
    , str_{nullptr}
{
}

StringWrapper::StringWrapper(StringWrapper& otherStr)
    : strPool_{otherStr.strPool_}
    , length_{otherStr.length_}
    , refCount_{otherStr.refCount_}
    , str_{otherStr.str_}
{
  ++(*refCount_);
}

StringWrapper& StringWrapper::operator=(StringWrapper& otherStr)
{
  this->strPool_ = otherStr.strPool_;
  this->length_ = otherStr.length_;
  this->str_ = otherStr.str_;

  this->refCount_ = otherStr.refCount_;
  ++(*refCount_);

  return *this;
}

StringWrapper& StringWrapper::operator=(const char* initStr)
{
  if((*this->refCount_) > 1)
  {
    this->refCount_ -= 1;
  }

  u16 inLen = std::strlen(initStr);
  u16 len = std::max(static_cast< usize >(8), roundToPow2(inLen));
  auto ret = this->strPool_->allocate(len);
  if(ret.err == NO_ERROR)
  {
    this->str_ = ret.str;
    this->refCount_ = ret.refCount;

    std::copy_n(initStr, inLen, this->str_);
  }
  return *this;
}

const char StringWrapper::operator[](u16 pos) const
{
  u16 index = std::min(pos, this->length_);
  return this->str_[index];
}

char StringWrapper::operator[](u16 pos)
{
  u16 index = std::min(pos, this->length_);

  if((*this->refCount_) > 1)
  {
    auto ret = this->strPool_->allocate(this->length_);
    if(ret.err == NO_ERROR)
    {
      --(*this->refCount_);
      this->str_ = ret.str;
      this->refCount_ = ret.refCount;
    }
    else
    {
      return 0;
    }
  }
  return this->str_[index];
}

const char* StringWrapper::cStr() { return this->str_; }

// TODO: add string manipulation functions.

} // end namespace Montreal

#endif // STRINGS_HPP
