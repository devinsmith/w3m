cmake_minimum_required(VERSION 3.7.2)

project(w3s)

find_package(OpenSSL REQUIRED)
find_package(Curses REQUIRED)

# w3s requires some libraries that are not packaged as CMake Find
# modules but PkgConfig is aware of them.
find_package(PkgConfig REQUIRED)
pkg_check_modules(GC REQUIRED bdw-gc)

include(CheckCSourceCompiles)

# See if this system can build a program with the strlcat function
# (which is present on most BSD operating systems, but not on Linux)
check_c_source_compiles("
#include <string.h>

int main(void)
{
  const char *p = \"hello\";
  char buf[25];

  strlcat(buf, p, sizeof(buf));
  return 0;
}
" HAS_STD_STRLCAT)

if (NOT HAS_STD_STRLCAT)
  # libbsd will provide BSD-specific functions.
  pkg_check_modules(BSD REQUIRED libbsd)
endif()

include(CheckFunctionExists)
if(NOT POW_FUNCTION_EXISTS AND NOT NEED_LINKING_AGAINST_LIBM)
  CHECK_FUNCTION_EXISTS(pow POW_FUNCTION_EXISTS)
  if(NOT POW_FUNCTION_EXISTS)
      unset(POW_FUNCTION_EXISTS CACHE)
      list(APPEND CMAKE_REQUIRED_LIBRARIES m)
      CHECK_FUNCTION_EXISTS(pow POW_FUNCTION_EXISTS)
      if(POW_FUNCTION_EXISTS)
          set(NEED_LINKING_AGAINST_LIBM True CACHE BOOL "" FORCE)
      else()
          message(FATAL_ERROR "Failed making the pow() function available")
      endif()
  endif()
endif()

option(USE_IMAGE "Use w3m images" ON)
option(USE_M17N "Use m17n" ON)

set(AUXBIN_DIR ${CMAKE_INSTALL_PREFIX}/libexec/w3s)
set(CGIBIN_DIR ${CMAKE_INSTALL_PREFIX}/libexec/w3s/cgi-bin)
set(CONF_DIR /etc/w3s)
set(ETC_DIR /etc)
set(HELP_DIR ${CMAKE_INSTALL_PREFIX}/share/doc/w3s)
set(RC_DIR ~/.w3s)

if (USE_M17N)
  add_subdirectory(libwc)
endif()

add_subdirectory(libindep)
add_subdirectory(libexec)
add_subdirectory(src)

install(DIRECTORY doc DESTINATION share/doc/w3s)
