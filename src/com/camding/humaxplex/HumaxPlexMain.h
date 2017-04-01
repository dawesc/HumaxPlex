/*
 * HumaxPlexMain.h
 *
 *  Created on: 1 Apr 2017
 *      Author: chrisd
 */

#pragma once

#include "Options.h"

namespace com {namespace camding {namespace humaxplex {

class HumaxPlexMain {
public:
  HumaxPlexMain();
  virtual ~HumaxPlexMain();
  
  virtual int Run(int argc, char* argv[]);

private:
  int ProcessCommandLineArgs(int argc, char* argv[]);

  Options options_;
};

}}}
