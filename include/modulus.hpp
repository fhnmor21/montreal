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

#ifndef MODULUS_HPP
#define MODULUS_HPP

#include "basic_types.hpp"

namespace Montreal
{

// this adds modulus semantics to integers
template < typename IntegerType >
class Modulus
{
  void mod()
  {
    if(this->value_ <= -(this->modulusVal_))
    {
      this->value_ %= this->modulusVal_;
    }
    this->value_ += this->modulusVal_;
    this->value_ %= this->modulusVal_;
  }
  Modulus() = delete;
  usize modulusVal_;
  IntegerType value_;

public:
  Modulus(usize modulus, IntegerType val)
      : modulusVal_{modulus}
      , value_{val}
  {
    this->mod();
  }
  explicit Modulus(Modulus& other)
      : modulusVal_{other.modulusVal_}
      , value_{other.value_}
  {
    this->mod();
  }

  Modulus& operator=(IntegerType val)
  {
    this->value_ = val;
    this->mod();
    return *this;
  }
  Modulus& operator=(Modulus& other)
  {
    // NOTE: assing a modulus to another change the modulus size
    this->modulusVal_ = other.modulusVal_;
    this->value_ = other.value_;
    this->mod();
    return *this;
  }

  Modulus operator+(IntegerType val)
  {
    this->value_ += val;
    this->mod();
    return *this;
  }
  Modulus operator+(Modulus& other)
  {
    this->value_ += other.value_;
    this->mod();
    return *this;
  }

  Modulus operator-(IntegerType val)
  {
    this->value_ -= val;
    this->mod();
    return *this;
  }
  Modulus operator-(Modulus& other)
  {
    this->value_ -= other.value_;
    this->mod();
    return *this;
  }

  Modulus operator*(IntegerType val)
  {
    this->value_ *= val;
    this->mod();
    return *this;
  }
  Modulus operator*(Modulus& other)
  {
    this->value_ *= other.value_;
    this->mod();
    return *this;
  }

  Modulus operator/(IntegerType val)
  {
    assert(val > 0);
    this->value_ = static_cast< IntegerType >(this->value_ / val);
    this->mod();
    return *this;
  }
  Modulus operator/(Modulus& other)
  {
    assert(other.value_ > 0);
    this->value_ = other.value_;
    this->mod();
    return *this;
  }

  Modulus& operator+=(IntegerType val)
  {
    this->value_ += val;
    this->mod();
    return *this;
  }

  Modulus& operator+=(Modulus& other)
  {
    this->value_ += other.value_;
    this->mod();
    return *this;
  }

  Modulus& operator-=(IntegerType val)
  {
    this->value_ -= val;
    this->mod();
    return *this;
  }
  Modulus& operator-=(Modulus& other)
  {
    this->value_ -= other.value_;
    this->mod();
    return *this;
  }

  Modulus& operator++()
  {
    this->value_ += 1;
    this->mod();
    return *this;
  }

  Modulus& operator--()
  {
    this->value_ -= 1;
    this->mod();
    return *this;
  }

  bool operator==(IntegerType val) { return this->value_ == val; }

  bool operator==(Modulus& other) { return this->value_ == other.value_; }

  bool operator!=(IntegerType val) { return this->value_ != val; }

  bool operator!=(Modulus& other) { return this->value_ != other.value_; }

  bool operator>=(IntegerType val) { return this->value_ >= val; }

  bool operator>=(Modulus& other) { return this->value_ >= other.value_; }

  bool operator<=(IntegerType val) { return this->value_ <= val; }

  bool operator<=(Modulus& other) { return this->value_ <= other.value_; }

  bool operator>(IntegerType val) { return this->value_ > val; }

  bool operator>(Modulus& other) { return this->value_ > other.value_; }

  bool operator<(IntegerType val) { return this->value_ < val; }

  bool operator<(Modulus& other) { return this->value_ < other.value_; }

  const IntegerType& toInt() const { return this->value_; }
};

using m8 = Modulus< i8 >;
using m16 = Modulus< i16 >;
using m32 = Modulus< i32 >;
using m64 = Modulus< i64 >;

} // end namespace Montreal

#endif // MODULUS_HPP
