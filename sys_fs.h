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

#define NOT_PRINT_SYS_FS_MESSAGES
#ifdef PRINT_SYS_FS_MESSAGES
#  define DO_PRAGMA(x) #x
#  define print_sys_fs_msg(x) _Pragma(DO_PRAGMA(message("INFO: " #x)))
#else
# define print_sys_fs_msg(A)
#endif

#if defined USE_BOOST_FS

# define has_boost_filesystem 1

#elif defined ANDROID

# define has_ghc_filesystem 1

#elif defined __has_include
print_sys_fs_msg("c++ has include")

# if __has_include(<filesystem>)
print_sys_fs_msg("c++ has include filesystem")
#  define has_filesystem 1

# elif __has_include(<experimental/filesystem>)
print_sys_fs_msg("c++ has include experimental/filesystem")
#  define has_experimental_filesystem 1

# elif __has_include(<boost/filesystem.hpp>)
print_sys_fs_msg("c++ has include boost:filesystem")
#  define has_boost_filesystem 1

# else
print_sys_fs_msg("c++ found no include filesystem")

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

#elif WIN32 && __clang__
print_sys_fs_msg("c++ use std::experimental::filesystem")
#  define has_experimental_filesystem 1

#endif


#if defined has_filesystem
print_sys_fs_msg("c++ include <filesystem>")
#include <filesystem>

print_sys_fs_msg("c++ use std::filesystem")
namespace sys_fs = std::filesystem;

#elif defined has_experimental_filesystem
print_sys_fs_msg("c++ include <experimental/filesystem>")
# include <experimental/filesystem>

print_sys_fs_msg("c++ use std::experimental::filesystem")
namespace sys_fs = std::experimental::filesystem;

#elif defined has_boost_filesystem

print_sys_fs_msg("c++ include <boost/filesystem.hpp>")
# include <boost/filesystem.hpp>

print_sys_fs_msg("c++ use boost::filesystem")
namespace sys_fs = boost::filesystem;

# ifndef WIN32
namespace boost {
  namespace filesystem {
    typedef time_t file_time_type;
  }
}
# endif // !WIN32


#elif defined has_ghc_filesystem
print_sys_fs_msg("c++ include <ghc/filesystem.hpp>")
#include <ghc/filesystem.hpp>

print_sys_fs_msg("c++ use ghc::filesystem")
namespace sys_fs = ghc::filesystem;

#else

print_sys_fs_msg("No C++ filesystem available")

#endif
