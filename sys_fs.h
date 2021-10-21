/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    standard lib
 *
 * Customer   -
 *
 * @brief     C++ API: file system mapper
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//

#ifdef PRINT_SYS_FS_MESSAGES
# define print_sys_fs_msg(A) #pragma message A
#else
# define print_sys_fs_msg(A)
#endif

#if defined USE_BOOST
print_sys_fs_msg("c++ include boost/filesystem.hpp")
# include <boost/filesystem.hpp>

print_sys_fs_msg("c++ use boost::filesystem")
namespace sys_fs = boost::filesystem;

namespace boost {
  namespace filesystem {
    typedef time_t file_time_type;
  }
}

#elif defined ANDROID

#include <ghc/filesystem.hpp>
namespace sys_fs = ghc::filesystem;

#else

// Use c++ feature checking
#if defined __has_include
print_sys_fs_msg("c++ has include")

# if __has_include(<filesystem>)
print_sys_fs_msg("c++ has include filesystem")
#  define has_filesystem 1

# elif __has_include(<experimental/filesystem>)
print_sys_fs_msg("c++ has include experimental/filesystem")
#  define has_experimental_filesystem 1

# elif __has_include(<boost/filesystem.hpp>)
print_sys_fs_msg("c++ has include boost:filesystem")
#  define USE_BOOST 1

# else
print_sys_fs_msg("c++ has no include filesystem")

# endif

// fallback to simple version checking
#elif (__cplusplus >= 201703L)
print_sys_fs_msg("c++ level is equal or above 2017-03")
# define has_filesystem 1

#elif (__clang_major__ >= 9)
print_sys_fs_msg("c++ use clang above 8")
# define has_filesystem 1

#elif (_MSC_VER >= 1900) || (__GNUC__ > 5) || ((__GNUC__ == 5) && (__GNUC_MINOR__ > 3))
print_sys_fs_msg("Fall back to experimental/filesystem")
#  define has_experimental_filesystem 1

#endif


#if defined has_filesystem
print_sys_fs_msg("c++ include std filesystem")
# include <filesystem>

# if WIN32 && __clang__
print_sys_fs_msg("c++ use std::experimental::filesystem")
namespace sys_fs = std::experimental::filesystem;

# else
print_sys_fs_msg("c++ use std::filesystem")
namespace sys_fs = std::filesystem;

# endif

#elif defined has_experimental_filesystem
print_sys_fs_msg("c++ include experimental/filesystem")
# include <experimental/filesystem>

print_sys_fs_msg("c++ use std::experimental::filesystem")
namespace sys_fs = std::experimental::filesystem;

#else

print_sys_fs_msg("No C++ filesystem available")

#endif

#endif
