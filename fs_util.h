/**
 * @copyright (c) 2021-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    utility lib
 *
 * @brief     C++ API: filesystem utils
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/sys_fs.h>
#include <util-export.h>

namespace util {

  namespace fs {

    UTIL_EXPORT sys_fs::path get_user_home ();

    UTIL_EXPORT bool is_executable (const sys_fs::path&);

    UTIL_EXPORT int execute (const sys_fs::path&);

    UTIL_EXPORT int open_document (const sys_fs::path&);

    UTIL_EXPORT int execute_or_open (const sys_fs::path&);

  } // namespace fs

} // namespace util
