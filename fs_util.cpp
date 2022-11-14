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
#else
#include <cstring>
#endif


// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/fs_util.h>
#include <util/string_util.h>


namespace util {

  namespace fs {

#ifndef WIN32
    void getenv_s (std::size_t* len, char* buffer, std::size_t max_len, char const* name) {
      const char* value = getenv(name);
      if (value) {
        std::strncpy(buffer, value, max_len);
        *len = std::strlen(buffer);
      } else {
        *len = 0;
        buffer[0] = 0;
      }
    }
#endif

    sys_fs::path get_user_home () {
      std::size_t len;
      char home[255] = {};
      getenv_s(&len, home, 255, "HOME");
      if (len) {
        return {home};
      } else {
        getenv_s(&len, home, 255, "USERPROFILE");
        if (len) {
          return {home};
        } else {
          getenv_s(&len, home, 255, "HOMEDRIVE");
          if (len) {
            home[len] = std::filesystem::path::preferred_separator;
            getenv_s(&len, home + len + 1, 254 - len, "HOMEPATH");
            if (len) {
              return { home };
            }
          }
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
      return 0 != ShellExecuteW(NULL, L"open", f.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
#else
      return std::system(("xdg-open \"" + f.string() + "\"").c_str());
#endif
    }

    int execute_or_open (const sys_fs::path& f) {
      return is_executable(f) ? execute(f) : open_document(f);
    }

    // @see: https://raymii.org/s/articles/Execute_a_command_and_get_both_output_and_exit_code.html
    command_result command (const sys_fs::path& f) {
        command_result result { 255, "" };
        std::array<char, 0xffff> buffer {};
#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#define WEXITSTATUS
#endif
#ifdef WIN32
        FILE *pipe = popen(util::string::utf16_to_utf8(f.c_str()).c_str(), "r");
#else
        FILE *pipe = popen(f.c_str(), "r");
#endif
        if (pipe == nullptr) {
            throw std::runtime_error("popen() failed!");
        }
        try {
            std::size_t bytesread;
            while ((bytesread = fread(buffer.data(), sizeof(buffer.at(0)), sizeof(buffer), pipe)) != 0) {
                result.output.append(buffer.data(), bytesread);
            }
        } catch (...) {
            pclose(pipe);
            throw;
        }
        result.exit_code = WEXITSTATUS(pclose(pipe));
        return result;
    }

  } // namespace fs

} // util
