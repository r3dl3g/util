/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
 * @brief     Rob private member helper functions
 *
 * @file
 */

#pragma once

namespace util {

  namespace math {

    //
    // Const calculate power of 10.
    //
    template<unsigned EXP>
    constexpr unsigned long pow10 () {
      return 10 * pow10<EXP-1>();
    }

    template<>
    constexpr unsigned long pow10<0> () {
      return 1;
    }

    //
    // Const calculate power of 2.
    //
    template<unsigned EXP>
    constexpr unsigned long pow2 () {
      return 2 * pow2<EXP-1>();
    }

    template<>
    constexpr unsigned long pow2<0> () {
      return 1;
    }

  } // namespace math

} // util
