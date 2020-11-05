/**
 * @copyright (c) 2018-2018 Ing. Buero Rothfuss
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

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <functional>
#include <vector>
#include <sstream>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/string_util.h>
#include <util-export.h>


namespace util {

  namespace csv {

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::string parse_text (std::istream& in, int& ch);
    UTIL_EXPORT std::string parse_none_text (std::istream& in, int& ch, int splitChar = ';');
    UTIL_EXPORT std::string parse_entry (std::istream& in, int& ch, int splitChar = ';');
    UTIL_EXPORT std::vector<std::string> parse_csv_line (std::istream& in, int splitChar = ';');
    UTIL_EXPORT void read_csv_data (std::istream& in, char delimiter, bool ignoreFirst,
                                    std::function<void(const std::vector<std::string>&)> fn);

    // --------------------------------------------------------------------------
    struct skip {};

    // --------------------------------------------------------------------------
    namespace detail {

      void skip_text (std::istream& in, int& ch);
      void skip_none_text (std::istream& in, int& ch, int splitChar);
      void skip_entry (std::istream& in, int& ch, int splitChar);

      // --------------------------------------------------------------------------
      template<typename T>
      T csv_element (std::istream& in, int& ch, int splitChar) {
        T t = util::string::convert::to<T>(parse_entry(in, ch, splitChar));
        ch = in.get();
        return t;
      }

      template<>
      skip csv_element<skip> (std::istream& in, int& ch, int splitChar);

#ifdef CAN_CALL_VARIADIC_IN_ORDER
      template<typename ... Arguments>
      std::tuple<Arguments...> csv_tuple (std::istream& in, int& ch, int splitChar) {
        return std::make_tuple(csv_element<Arguments>(in, ch, splitChar)...);
      }
#else
	
      template<typename ... Arguments>
      struct csv_tuple {
        static std::tuple<Arguments...> read (std::istream& in, int& ch, int splitChar);
      };

      template<typename T>
      struct csv_tuple<T> {
        static std::tuple<T> read (std::istream& in, int& ch, int splitChar) {
          return std::make_tuple(csv_element<T>(in, ch, splitChar));
        }
      };

      template<typename T, typename ... Arguments>
      struct csv_tuple<T, Arguments...> {
        static std::tuple<T, Arguments...> read (std::istream& in, int& ch, int splitChar) {
          auto lhs = csv_tuple<T>::read(in, ch, splitChar);
          auto rhs = csv_tuple<Arguments...>::read(in, ch, splitChar);
          return std::tuple_cat(std::move(lhs), std::move(rhs));
        }
      };
#endif

    } // namespace detail

    // --------------------------------------------------------------------------
    template<typename ... Arguments>
    struct tuple_reader {
      typedef std::tuple<Arguments...> tuple;

      static void read_csv (std::istream& in, char delimiter, bool ignoreFirst, std::function<void(const tuple&)> fn) {
        bool ignore = ignoreFirst;
        int ch = in.get();
        while (in.good()) {
          while ((ch == '\n') || (ch == '\r')) {
            ch = in.get();
          }
          if (ignore) {
            while ((ch != '\n') && (ch != '\r')) {
              ch = in.get();
            }
            ignore = false;
          } else {
            fn(detail::csv_tuple<Arguments...>::read(in, ch, delimiter));
            ch = in.get();
          }
        }
      }
    };

  } // namespace csv

} // namespace util
