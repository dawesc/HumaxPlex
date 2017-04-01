/*
 * Options.h
 *
 *  Created on: 1 Apr 2017
 *      Author: chrisd
 */

#pragma once

#include "Boolean.h"
#include <string>

namespace com {namespace camding {namespace humaxplex {

class Options {
public:
  Options();
  ~Options();

  Boolean help = false;
  Boolean verbose = false;
  Boolean version = false;
  std::string dbFile;
  std::string sourceHostIp;
  std::string tempFolder;
  std::string destFolder;

};

}}}

