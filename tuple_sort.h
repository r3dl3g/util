/**
* @copyright (c) 2015-2019 Ing. Buero Rothfuss
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
* @brief     C++ API:ostreamfmt
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <algorithm>
#include <vector>

namespace util {

  namespace sort {

    // --------------------------------------------------------------------------
    enum class order : bool {
      up = false,
      down = true
    };

    order operator- (order lhs, order rhs) {
      return order(static_cast<unsigned char>(lhs) - static_cast<unsigned char>(rhs));
    }

    order operator~ (order lhs) {
      return order(!static_cast<bool>(lhs));
    }

    order operator! (order lhs) {
      return order(!static_cast<bool>(lhs));
    }
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
    void sort (int index, std::vector<std::tuple<Arguments ...>>& v) {
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

} // namespace util
