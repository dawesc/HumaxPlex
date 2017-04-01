# FindSQLite3.cmake
#
# Custom script to find SQLite3

find_path(SQLite3_INCLUDE_DIR sqlite3.h)
find_library(SQLite3_LIBRARY sqlite3)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SQLite3 REQUIRED_VARS SQLite3_LIBRARY SQLite3_INCLUDE_DIR)

if(SQLITE3_FOUND)
  set(SQLite3_FOUND true)
  set(SQLite3_LIBRARIES ${SQLite3_LIBRARY})
endif()