/*
 * Boolean.cpp
 *
 *  Created on: 1 Apr 2017
 *      Author: chrisd
 */

#include "Boolean.h"
#include <boost/algorithm/string.hpp>

using com::camding::humaxplex::Boolean;
using std::string;

Boolean::Boolean() {
  /* Not used */
}

Boolean::Boolean(const string& str)
    : value_(Boolean::FromString(str)) {
  /* Not used */
}

Boolean::Boolean(bool value)
    : value_(value) {
  /* Not used */
}

Boolean::Boolean(const Boolean& booleanVar)
    : value_(booleanVar) {
  /* Not used */
}

Boolean::~Boolean() { /* Not used */ }

Boolean::operator bool() const {
  return value_;
}

bool Boolean::operator*() const {
  return value_;
}

bool Boolean::FromString(const string& str) {
  if (str.empty()) {
    return false;
  }

  return (
      str == "1"
      || boost::iequals(str, "y")
      || boost::iequals(str, "yes")
      || boost::iequals(str, "true")
  );
}

namespace com {
namespace camding {
namespace humaxplex {

std::istream& operator >>(std::istream& stream, Boolean& boolean) {
  return stream;
}

}}}
