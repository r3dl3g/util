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
 * @brief     C++ API: math utils
 *
 * @license   MIT license. See accompanying file LICENSE.
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

    //
    // Calc signum
    // @see: https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c/4609795#4609795
    //
    template<typename T>
    inline constexpr int signum (T x, std::false_type) {
      return T(0) < x;
    }

    template<typename T>
    inline constexpr int signum (T x, std::true_type) {
      return (T(0) < x) - (x < T(0));
    }

    template<typename T>
    inline constexpr int signum (T x) {
      return signum(x, std::is_signed<T>());
    }

    // --------------------------------------------------------------------------
    template<typename T>
    bool is_between (T lower, T value, T upper) {
      return static_cast<typename std::make_unsigned<T>::type>(value - lower) <= (upper - lower);
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline T limit (T lower, T value, T upper) {
      #if (__cplusplus >= 201703L)
        return std::clamp(value, lower, upper);
      #else
        return std::min(std::max(value, lower), upper);
      #endif
    }

    // --------------------------------------------------------------------------
    template<int weight, typename T>
    inline T rolling_avg (T avarage, T score) {
      return (avarage * (weight - 1) + score) / weight;
    }

  } // namespace math

} // util
