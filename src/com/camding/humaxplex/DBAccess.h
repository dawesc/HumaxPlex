/*
 * DBAccess.h
 *
 *  Created on: 1 Apr 2017
 *      Author: chrisd
 */

#pragma once

#include <string>

struct sqlite3; //Forward declare

namespace com {
namespace camding {
namespace humaxplex {

class Options; //Forward declare

class DBAccess {

public:
  DBAccess(const Options& options);
  virtual ~DBAccess();
  void ExecuteUpdate(const std::string& query);
  void ExecuteQuery(const std::string& query);

private:
  sqlite3* db_;
  bool changes_ = false;
  bool verbose_ = false;
};

}}}
