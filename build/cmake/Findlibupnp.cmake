# Findlibupnp.cmake
#
# Custom script to find lipupnp

find_path(libupnp_INCLUDE_DIR upnp.h PATHS /opt/local/include/upnp)
find_library(libupnp_LIBRARY upnp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libupnp REQUIRED_VARS libupnp_LIBRARY libupnp_INCLUDE_DIR)

if(LIBUPNP_FOUND)
  set(libupnp_FOUND true)
  set(libupnp_LIBRARIES ${libupnp_LIBRARY})
endif()