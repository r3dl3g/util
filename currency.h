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

// --------------------------------------------------------------------------
//
// Project includes
//
#include "string_util.h"
#include "math_util.h"

namespace util {

  // --------------------------------------------------------------------------
  // Store a currency value together with a currency symbol and with a defined
  // precision
  template<uint32_t SYM, int N = 2, int D = math::pow10<N>()>
  struct currency {
    inline currency (long full = 0)
      :full_(full)
    {}

    inline currency (long e, long c)
      : full_(e * D + c)
    {}

    inline constexpr currency (long double d)
      : full_(static_cast<long>(d * D))
    {}

    inline constexpr currency (double d)
      : full_(static_cast<long>(d * D))
    {}

    inline constexpr uint32_t symbol () const {
      return SYM;
    }

    inline constexpr uint32_t precision () const {
      return N;
    }

    inline bool operator== (const currency& rhs) const {
      return full_ == rhs.full_;
    }

    inline currency operator* (long f) const {
      return {full_ * f};
    }

    inline currency operator* (int f) const {
      return {full_ * f};
    }

    inline currency operator/ (long f) const {
      return {full_ / f};
    }

    inline currency operator/ (int f) const {
      return {full_ / f};
    }

    inline currency operator* (double f) const {
      return {as_real() * f};
    }

    inline currency operator/ (double f) const {
      return {as_real() / f};
    }

    inline currency& operator+= (const currency& rhs) {
      full_ += rhs.full_;
      return *this;
    }

    inline long pre_decimals () const {
      return full_ / D;
    }

    inline long decimals () const {
      return std::abs(full_ % D);
    }

    inline long all_digits () const {
      return full_;
    }

    inline double as_real () const {
      return static_cast<double>(full_) / D;
    }

  private:
    long full_;
  };

  using namespace utf8::literals;

  typedef currency<u8"€"_UTF8> Euro;
  typedef currency<u8"$"_UTF8> Dollar;
  typedef currency<u8"£"_UTF8> Pound;
  typedef currency<u8"¥"_UTF8> Yen;
  typedef currency<u8"₩"_UTF8> Won;
  typedef currency<u8"元"_UTF8> Yuan;
  typedef currency<u8"₦"_UTF8> Naira;
  typedef currency<u8"₹"_UTF8> Rupee;
  typedef currency<u8"₽"_UTF8> Ruble;

  namespace literals {

    constexpr currency<0> operator "" _CUR(long double v) { return {v}; }
    constexpr Euro operator "" _EUR(long double v)        { return {v}; }
    constexpr Dollar operator "" _USD(long double v)      { return {v}; }
    constexpr Pound operator "" _GBP(long double v)       { return {v}; }
    constexpr Yen operator "" _JPY(long double v)         { return {v}; }
    constexpr Yuan operator "" _CNY(long double v)        { return {v}; }
    constexpr Rupee operator "" _INR(long double v)       { return {v}; }
    constexpr Ruble operator "" _RUB(long double v)       { return {v}; }
    constexpr Won operator "" _KRW(long double v)         { return {v}; }

  }

  template<uint32_t SYM, int N, int D>
  inline bool operator< (const currency<SYM, N, D>& lhs, const currency<SYM, N, D>& rhs) {
    return lhs.all_digits() < rhs.all_digits();
  }

  template<uint32_t SYM, int N, int D>
  inline std::ostream& operator<< (std::ostream& out, const currency<SYM, N, D>& m) {
    const static auto symbol = utf8::uint32_to_utf8(SYM);
    out << symbol.buffer << m.pre_decimals() << "." << std::setfill('0') << std::setw(m.precision()) << m.decimals();
    return out;
  }

  template<uint32_t SYM, int N, int D>
  inline std::istream& operator>> (std::istream& in, currency<SYM, N, D>& m) {
    while (in.good() && !std::isdigit(in.peek()) && (in.peek() != '-')) {
      in.ignore();
    }
    double v = 0;
    in >> v;
    m = v;
    return in;
  }

} // util
