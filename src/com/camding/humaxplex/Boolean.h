/*
 * Boolean.h
 *
 *  Created on: 1 Apr 2017
 *      Author: chrisd
 */

#pragma once

#include <istream>
#include <ostream>
#include <string>

namespace com {
namespace camding {
namespace humaxplex {

class Boolean
{
public:
  Boolean();
  Boolean(const std::string& str);
  Boolean(bool value);
  Boolean(const Boolean& booleanVar);
  ~Boolean();

  operator bool() const;
  bool operator *() const;

  static bool FromString(const std::string& str);

private:
  bool value_ = false;
};

std::istream& operator>>(std::istream& istream, Boolean& boolean);

}}}
