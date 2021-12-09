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
#ifdef WIN32
#include <windows.h>
#include <shellapi.h>
#endif


// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/fs_util.h>
#include <util/string_util.h>


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

    bool is_executable (const sys_fs::path& f) {
#ifdef WIN32
      return util::string::ends_with(util::string::utf16_to_utf8(f.c_str()), ".exe");
#else
      return (sys_fs::status(f).permissions() &
              (sys_fs::perms::owner_exec|sys_fs::perms::group_exec|sys_fs::perms::others_exec))
          != sys_fs::perms::none;
#endif
    }

    int execute (const sys_fs::path& f) {
#ifdef WIN32
      return std::system(util::string::utf16_to_utf8(f.c_str()).c_str());
#else
      return std::system(f.c_str());
#endif
    }

    int open_document (const sys_fs::path& f) {
#ifdef WIN32
      return reinterpret_cast<int>(ShellExecuteW(NULL, L"open", f.c_str(), nullptr, nullptr, SW_SHOWNORMAL));
#else
      return std::system(("xdg-open \"" + f.string() + "\"").c_str());
#endif
    }

    int execute_or_open (const sys_fs::path& f) {
      return is_executable(f) ? execute(f) : open_document(f);
    }

  } // namespace fs

} // util
