/**
 * @copyright (c) 2018-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    utility lib
 *
 * @brief     C++ API: command_line
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <functional>
#include <vector>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/string_util.h>


namespace util {

  namespace command_line {

    // -----------------------------------------------------------------------
    struct arg {
      std::string short_cmd;
      std::string long_cmd;
      std::string needed_arg;
      std::string explanation;
      std::function<void(std::string)> action;
    };

    // -----------------------------------------------------------------------
    struct toggle_param {

      toggle_param (bool& f)
        : flag(f)
      {}
      
      void operator() (const std::string&) {
        flag = !flag;
      }

    private:
      bool& flag;
    };

    // -----------------------------------------------------------------------
    inline toggle_param mk_toggle_param (bool& t) {
      return toggle_param(t);
    }

    // -----------------------------------------------------------------------
    template<typename T>
    struct increment_param {

      increment_param (T& f)
        : flag(f)
      {}
      
      void operator() (const std::string&) {
        ++flag;
      }

    private:
      T& flag;
    };

    // -----------------------------------------------------------------------
    template<typename T>
    inline increment_param<T> mk_increment_param (T& t) {
      return increment_param<T>(t);
    }

    // -----------------------------------------------------------------------
    template<typename T>
    struct value_param {

      value_param (T& f)
        : flag(f)
      {}
      
      void operator() (const std::string& s) {
        flag = util::string::convert::to<T>(s);
      }

    private:
      T& flag;
    };

    // -----------------------------------------------------------------------
    template<typename T>
    inline value_param<T> mk_value_param (T& t) {
      return value_param<T>(t);
    }

    // -----------------------------------------------------------------------
    struct UTIL_EXPORT parser {
      typedef std::vector<std::string> arg_list;
      typedef arg_list::const_iterator iterator;

      explicit parser (const std::string& app_name = {}, const std::vector<arg>& args = {});

      void add (const std::vector<arg>& args);
      void add (arg&& a);

      void process (int argc, const char* argv[]);
      void process (const arg_list& args);

      void show_help (std::ostream&);

      inline const arg_list& remaining_args () const {
        return remaining;
      }

      inline const std::string& name () const {
        return app_name;
      }

    private:
      static void execute (const arg& cmd, const std::string& match, iterator& i, const iterator& e);

      std::string app_name;
      std::vector<arg> commands;
      arg_list remaining;
    };

  } // namespace command_line

} // namespace util
