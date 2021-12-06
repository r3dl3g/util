/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    utility lib
 *
 * @brief     C++ API: sort order enum
 *
 * @license   MIT license. See accompanying file LICENSE.
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

    inline order operator- (order lhs, order rhs) {
      return order(static_cast<unsigned char>(lhs) - static_cast<unsigned char>(rhs));
    }

    inline order operator! (order lhs) {
      return order::up == lhs ? order::down : order::up;
    }

    inline order operator~ (order lhs) {
      return operator!(lhs);
    }

  } // namespace sort

} // namespace util
