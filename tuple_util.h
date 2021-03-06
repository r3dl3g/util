/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    utility lib
 *
 * @brief     C++ API: tuple utilities
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <algorithm>
#include <vector>

#include <util/sort_order.h>
#include <util/string_util.h>

namespace util {

  namespace tuple {

    namespace sort {

      using namespace util::sort;

      // --------------------------------------------------------------------------
      template<int I, typename ... Arguments>
      struct tuple_compare {
        bool operator() (const std::tuple<Arguments ...>& lhs, const std::tuple<Arguments ...>& rhs) {
          return std::get<I>(lhs) < std::get<I>(rhs);
        }
      };

      // --------------------------------------------------------------------------
      template<order O, int I, typename ... Arguments>
      struct sorter {
        static void sort (int index, std::vector<std::tuple<Arguments ...>>& v);
      };

      // --------------------------------------------------------------------------
      template<typename ... Arguments>
      struct sorter<order::up, -1, Arguments ...> {
        static void sort (int, std::vector<std::tuple<Arguments ...>>&) {}
      };

      // --------------------------------------------------------------------------
      template<int I, typename ... Arguments>
      struct sorter<order::up, I, Arguments ...> {
        static void sort (int index, std::vector<std::tuple<Arguments ...>>& v) {
          if (index == I) {
            std::stable_sort(v.begin(), v.end(), tuple_compare<I, Arguments ...>());
          } else {
            sorter<order::up, I - 1, Arguments ...>::sort(index, v);
          }
        }
      };

      // --------------------------------------------------------------------------
      template<typename ... Arguments>
      struct sorter<order::down, -1, Arguments ...> {
        static void sort (int, std::vector<std::tuple<Arguments ...>>&) {}
      };

      // --------------------------------------------------------------------------
      template<int I, typename ... Arguments>
      struct sorter<order::down, I, Arguments ...> {
        static void sort (int index, std::vector<std::tuple<Arguments ...>>& v) {
          if (index == I) {
            std::stable_sort(v.rbegin(), v.rend(), tuple_compare<I, Arguments ...>());
          } else {
            sorter<order::down, I - 1, Arguments ...>::sort(index, v);
          }
        }
      };

      // --------------------------------------------------------------------------
      template<order O, typename ... Arguments>
      inline void sort (int index, std::vector<std::tuple<Arguments ...>>& v) {
        sorter<O, sizeof...(Arguments) - 1, Arguments ...>::sort(index, v);
      }

      // --------------------------------------------------------------------------
      template<typename ... Arguments>
      void by (order o, int t, std::vector<std::tuple<Arguments ...>>& data) {
        switch (o) {
          case order::up:
            sort<order::up, Arguments ...>(t, data);
            break;
          case order::down:
            sort<order::down, Arguments ...>(t, data);
            break;
        }
      }

    } // namespace sort

    namespace convert {

      //-----------------------------------------------------------------------------
      template<int N, typename T>
      struct as {
        static std::string string (int index, const T& t) {
          if (N == index) {
            return util::string::convert::from(std::get<N>(t));
          }
          return as<N - 1, T>::string(index, t);
        }
      };

      template<typename T>
      struct as<-1, T> {
        static std::string string (int, const T& t) {
          return {};
        }
      };

      template<typename ... Arguments>
      std::string as_string (int index, const std::tuple<Arguments...>& t) {
        return as<sizeof...(Arguments) - 1, std::tuple<Arguments...>>::string(index, t);
      }

      //-----------------------------------------------------------------------------
      template<int N, typename T>
      struct to {
        static void stream (std::ostream& out, const T& t) {
          to<N - 1, T>::stream(out, t);
          out << "," << std::get<N>(t);
        }
      };

      template<typename T>
      struct to<0, T> {
        static void stream (std::ostream& out, const T& t) {
          out << std::get<0>(t);
        }
      };

      template<typename ... Arguments>
      void to_stream (std::ostream& out, const std::tuple<Arguments...>& t) {
        to<sizeof...(Arguments) - 1, std::tuple<Arguments...>>::stream(out, t);
      }

      //-----------------------------------------------------------------------------
      template<std::size_t N, std::size_t I, typename T, typename ... Args>
      struct from {
        static std::tuple<T, Args...> vector (const std::vector<std::string>& v) {
          return std::tuple_cat(std::make_tuple(util::string::convert::to<T>(v[I])),
                                from<N - 1, I + 1, Args...>::vector(v));
        }
      };

      template<std::size_t I, typename T>
      struct from<1, I, T> {
        static std::tuple<T> vector (const std::vector<std::string>& v) {
          return std::make_tuple(util::string::convert::to<T>(v[I]));
        }
      };

      template<typename ... Args>
      std::tuple<Args...> from_vector (const std::vector<std::string>& v) {
        return from<sizeof...(Args), 0, Args...>::vector(v);
      }
    } // namespace convert

  } // namespace tuple

} // namespace util

namespace std {

  template<typename ... Arguments>
  ostream& operator<< (ostream& out, const std::tuple<Arguments...>& t) {
    util::tuple::convert::to_stream(out, t);
    return out;
  }

} // namespace std
