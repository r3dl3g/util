/**
 * @copyright (c) 2016-2019 Ing. Buero Rothfuss
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
 * @brief     C++ API: file system mapper
 *
 * @file
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//


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

  } // namespace sort

} // namespace util