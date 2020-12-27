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

#if (__cplusplus >= 201700L)

# include <filesystem>
namespace sys_fs = std::filesystem;

#elif defined(USE_BOOST)

# include <boost/filesystem.hpp>
namespace sys_fs = boost::filesystem;

#elif (_MSC_VER >= 1900) || (__GNUC__ > 5) || ((__GNUC__ == 5) && (__GNUC_MINOR__ > 3)) || defined(__clang__)

# include <experimental/filesystem>

namespace sys_fs = std::experimental::filesystem;

#else

# pragma error "No C++ filesystem available"

#endif

