/*
 * DBAccess.cpp
 *
 *  Created on: 1 Apr 2017
 *      Author: chrisd
 */

#include "DBAccess.h"
#include "Options.h"
#include <iostream>
#include <stdio.h>
#include <sqlite3.h> 

using com::camding::humaxplex::DBAccess;
using com::camding::humaxplex::Options;

DBAccess::DBAccess(const Options& options) :
  verbose_(options.verbose) {
  char *zErrMsg = 0;
  int rc;
  
  rc = sqlite3_open(options.dbFile.c_str(), &db_);
  
  if (rc) {
    std::string err = "Can't open database: ";
    err.append(sqlite3_errmsg(db_));
    throw std::runtime_error(err);
  }
  if (options.verbose) {
    std::cout << "Opened database successfully" << std::endl;
  }

  ExecuteUpdate("BEGIN;");
  ExecuteUpdate(" " \
    "CREATE TABLE IF NOT EXISTS hp_sys_params ( " \
    "  entry_key TEXT PRIMARY KEY, " \
    "  entry_val TEXT " \
    ");");
  ExecuteUpdate(" " \
    "CREATE TABLE IF NOT EXISTS hp_log ( " \
    "  entry_id INTEGER PRIMARY KEY AUTOINCREMENT, " \
    "  entry_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, " \
    "  log_level INTEGER, " \
    "  message TEXT " \
    ");");
  ExecuteUpdate(" " \
    "CREATE TABLE IF NOT EXISTS hp_files ( " \
    "  file_id INTEGER PRIMARY KEY AUTOINCREMENT, " \
    "  found_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, " \
    "  last_modified TIMESTAMP, " \
    "  file_size INTEGER, " \
    "  file_state INTEGER, " \
    "  source_file_name TEXT, " \
    "  temp_file_name TEXT, " \
    "  target_file_name TEXT " \
    ");");
}

DBAccess::~DBAccess() {
  if (changes_) {
    ExecuteUpdate("COMMIT;");
  }
  sqlite3_close(db_);
}

void DBAccess::ExecuteUpdate(const std::string& query) {
  changes_ = true;
  char *zErrMsg = 0;
  auto callback = [](void *data, int argc, char **argv, char **azColName)->int {
    int i;
    fprintf(stderr, "%s: ", (const char*)data);
    for(i=0; i<argc; i++){
       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
 };

  auto rc = sqlite3_exec(db_, query.c_str(), callback, 0, &zErrMsg);
  if( rc != SQLITE_OK ){
    std::string err = "Can't open database: ";
    err.append(sqlite3_errmsg(db_));
    throw std::runtime_error(err);
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }else if (verbose_) {
    std::cout << "Successfully run " << query << std::endl;
  }
}

void DBAccess::ExecuteQuery(const std::string& query) {

}
