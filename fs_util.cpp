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

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/fs_util.h>


namespace util {

  namespace fs {

    sys_fs::path get_user_home () {
      char const* home = getenv("HOME");
      if (home || ((home = getenv("USERPROFILE")))) {
        return {home};
      } else {
        char const* hdrive = getenv("HOMEDRIVE");
        char const* hpath = getenv("HOMEPATH");
        if (hdrive && hpath) {
          return sys_fs::path(hdrive) / hpath;
        }
      }
      return {"."};
    }


  } // namespace fs

} // util
