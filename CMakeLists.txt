cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/build/cmake")

project(HumaxPlex)

########################################
## Dependencies

find_package(Boost 1.58.0 
	REQUIRED COMPONENTS log log_setup iostreams 
		filesystem system thread chrono regex program_options)

find_package(SQLite3 REQUIRED)
find_package(libupnp REQUIRED)

if(WINDOWS)
  set(System_LIBRARIES iphlpapi psapi wsock32 ws2_32 rpcrt4 dbghelp pdh)
elseif(LINUX)
  set(System_LIBRARIES pthread rt)
endif()

########################################
## Targets

set(MAIN_LIBRARY "libhumax_plex")
set(MAIN_EXECUTABLE "humax_plex")

set(MAIN_SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

file(GLOB_RECURSE MAIN_SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/com/*.cpp")

add_library(${MAIN_LIBRARY} SHARED
  ${MAIN_SOURCE_FILES})
target_compile_features(${MAIN_LIBRARY} PRIVATE cxx_range_for)
target_include_directories(${MAIN_LIBRARY} PUBLIC
  ${MAIN_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${SQLite3_INCLUDE_DIR}
  ${libupnp_INCLUDE_DIRS})
target_link_libraries(${MAIN_LIBRARY}
  ${Boost_LIBRARIES}
  ${SQLite3_LIBRARIES}
  ${libupnp_LIBRARIES})

install(TARGETS ${MAIN_LIBRARY}
  RUNTIME DESTINATION bin COMPONENT runtime
  LIBRARY DESTINATION lib COMPONENT runtime)
install(DIRECTORY "${MAIN_SOURCE_DIR}/"
  DESTINATION include COMPONENT devel
  FILES_MATCHING PATTERN "*.h")

add_executable(${MAIN_EXECUTABLE} "${CMAKE_SOURCE_DIR}/src/main.cpp")
target_compile_features(${MAIN_EXECUTABLE} PRIVATE cxx_range_for)
target_link_libraries(${MAIN_EXECUTABLE} 
	${MAIN_LIBRARY} 
	${Boost_LIBRARIES}
	${SQLite3_LIBRARIES}
  ${libupnp_LIBRARIES})
install(TARGETS ${MAIN_EXECUTABLE} DESTINATION bin COMPONENT runtime)
