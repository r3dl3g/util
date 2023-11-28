/**
 * @copyright (c) 2016-2023 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    utility lib
 *
 * @brief     iterator to use for_each with index
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <execution>
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/index_iterator.h>

namespace util {

  template<typename T>
  struct matrix {

    matrix (std::size_t x = 0, std::size_t y = 0, T t = {}) {
      resize(x, y < 1 ? x : y, t);
    }

    void resize (std::size_t x = 0, std::size_t y = 0, T t = {}) {
      max_x = x;
      max_y = y < 1 ? x : y;
      data.resize(max_y, std::vector<T>(max_y, t));
    }

    T& at (std::size_t x, std::size_t y) {
      return data.at(x % max_x).at(y % max_y);
    }

    const T& at (std::size_t x, std::size_t y) const {
      return data.at(x % max_x).at(y % max_y);
    }

    std::size_t size_x () const {
      return max_x;
    }

    std::size_t size_y () const {
      return max_y;
    }

  private:
    std::vector<std::vector<T>> data;
    std::size_t max_x;
    std::size_t max_y;
  };

  template<class ExecutionPolicy, typename F>
  void matrix_iterate (ExecutionPolicy&& exec,
                      int xmin, int xmax,
                      int ymin, int ymax,
                      F fn) {
    const std::size_t l_size = xmax - xmin;
    const std::size_t r_size = ymax - ymin;
    std::for_each(std::forward<ExecutionPolicy>(exec),
                  util::index_iterator(0),
                  util::index_iterator(l_size * r_size),
                  [&] (std::size_t i) {
      const std::size_t x = xmin + (i % l_size);
      const std::size_t y = ymin + (i / l_size);
      fn(x, y);
    });
  }

  template<typename F>
  bool matrix_traverse (int xmin, int xmax,
                        int ymin, int ymax,
                        F fn) {
    for (int x = xmin; x < xmax; ++x) {
      for (int l = x, r = ymin; (l >= xmin) && (r < ymax); --l, ++r) {
        if (!fn(l, r)) {
          return false;
        }
      }
    }
    for (int y = ymin + 1; y < ymax; ++y) {
      for (int l = xmax - 1, r = y; (l >= xmin) && (r < ymax); --l, ++r) {
        if (!fn(l, r)) {
          return false;
        }
      }
    }

    return true;
  }

} // util
