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
 * @brief     simple 2x2 rotating matrix
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

  template<class ExecutionPolicy, typename T, typename F>
  void matrix_iterate (ExecutionPolicy&& exec,
                      T xmin, T xmax,
                      T ymin, T ymax,
                      F fn) {
    const T l_size = std::max<T>(xmax - xmin, 0);
    const T r_size = std::max<T>(ymax - ymin, 0);
    std::for_each(std::forward<ExecutionPolicy>(exec),
                  util::index_iterator<T>(0),
                  util::index_iterator<T>(l_size * r_size),
                  [&] (T i) {
      const T x = xmin + (i % l_size);
      const T y = ymin + (i / l_size);
      fn(x, y);
    });
  }

  template<typename T, typename F>
  bool matrix_traverse (T xmin, T xmax,
                        T ymin, T ymax,
                        F fn) {
    using S = std::make_signed_t<T>;
    for (T x = xmin; x < xmax; ++x) {
      T r = ymin;
      for (S l = x; (l >= static_cast<S>(xmin)) && (r < ymax); --l, ++r) {
        if (!fn(static_cast<T>(l), r)) {
          return false;
        }
      }
    }
    for (T y = ymin + 1; y < ymax; ++y) {
      T r = y;
      for (S l = xmax - 1; (l >= static_cast<S>(xmin)) && (r < ymax); --l, ++r) {
        if (!fn(static_cast<T>(l), r)) {
          return false;
        }
      }
    }

    return true;
  }

} // util
