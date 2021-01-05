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


// Use c++ feature checking
#if defined __has_include

# if __has_include(<filesystem>)
#  define has_filesystem 1
# elif __has_include(<experimental/filesystem>)
#  define has_experimental_filesystem 1
# endif

// fallback to simple version checking
#elif (__cplusplus >= 201703L)

# define has_filesystem 1

#elif (_MSC_VER >= 1900) || (__GNUC__ > 5) || ((__GNUC__ == 5) && (__GNUC_MINOR__ > 3))

#  define has_experimental_filesystem 1

#endif


#if defined has_filesystem

# include <filesystem>
namespace sys_fs = std::filesystem;

#elif defined has_experimental_filesystem

# include <experimental/filesystem>
namespace sys_fs = std::experimental::filesystem;

#elif defined USE_BOOST

# include <boost/filesystem.hpp>
namespace sys_fs = boost::filesystem;

#else

# pragma error "No C++ filesystem available"

#endif

