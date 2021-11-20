/**
 * @copyright (c) 2018-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: command_line
 *
 * @file
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
