/*
 * main.cpp
 *
 *  Created on: 1 Apr 2017
 *      Author: chrisd
 */

#include <com/camding/humaxplex/HumaxPlexMain.h>

using com::camding::humaxplex::HumaxPlexMain;

int main(int argc, char* argv[])
{
  HumaxPlexMain instance;
  return instance.Run(argc, argv);
}
