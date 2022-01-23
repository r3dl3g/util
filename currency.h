/**
 * @copyright (c) 2021-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    utility lib
 *
 * @brief     Rob private member helper functions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Project includes
//
#include <util/string_util.h>
#include <util/math_util.h>

namespace util {

  // --------------------------------------------------------------------------
  // Store a currency value together with a currency symbol and with a defined
  // precision
  template<uint32_t SYM, int N = 2, int D = math::pow10<N>()>
  struct currency {
    inline currency (int64_t full = 0)
      :full_(full)
    {}

    inline currency (int64_t e, int64_t c)
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

    inline currency operator* (int64_t f) const {
      return {full_ * f};
    }

    inline currency operator* (int32_t f) const {
      return {full_ * f};
    }

    inline currency operator/ (int64_t f) const {
      return {full_ / f};
    }

    inline currency operator/ (int32_t f) const {
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

    inline int64_t pre_decimals () const {
      return full_ / D;
    }

    inline int64_t decimals () const {
      return std::abs(full_ % D);
    }

    inline int64_t all_digits () const {
      return full_;
    }

    inline double as_real () const {
      return static_cast<double>(full_) / D;
    }

    std::string string () const;

    inline std::ostream& as_fix_real (std::ostream& out) const {
      out << std::fixed << std::setprecision(precision()) << as_real();
      return out;
    }

    inline std::ostream& with_symbol (std::ostream& out) const {
      const static auto symbol = utf8::uint32_to_utf8(SYM);
      out << symbol.buffer;
      return as_fix_real(out);
    }

    static currency from_string (const std::string&);

  private:
    int64_t full_;
  };

  using namespace utf8::literals;

#ifdef WIN32

  typedef currency<0xac82e2> Euro;
  typedef currency<0x24> Dollar;
  typedef currency<0xa3c2> Pound;
  typedef currency<0xa5c2> Yen;
  typedef currency<0xa982e2> Won;
  typedef currency<0x5143> Yuan;
  typedef currency<0xa682e2> Naira;
  typedef currency<0xa882e2> Rupee;
  typedef currency<0xbd82e2> Ruble;

#else

  typedef currency<u8"€"_UTF8> Euro;
  typedef currency<u8"$"_UTF8> Dollar;
  typedef currency<u8"£"_UTF8> Pound;
  typedef currency<u8"¥"_UTF8> Yen;
  typedef currency<u8"₩"_UTF8> Won;
  typedef currency<u8"元"_UTF8> Yuan;
  typedef currency<u8"₦"_UTF8> Naira;
  typedef currency<u8"₹"_UTF8> Rupee;
  typedef currency<u8"₽"_UTF8> Ruble;

#endif // _MSVC

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
    return m.with_symbol(out);
  }

  template<uint32_t SYM, int N, int D>
  inline std::istream& operator>> (std::istream& in, currency<SYM, N, D>& m) {
    // std::isdigit(in.peek(), in.getloc()) crashes with bad_cast exception
    while (in.good() && !std::isdigit(in.peek()) && (in.peek() != '-')) {
      in.ignore();
    }
    double v = 0;
    in >> v;
    m = v;
    return in;
  }

  template<uint32_t SYM, int N, int D>
  std::string currency<SYM, N, D>::string () const {
    return ostreamfmt(*this);
  }

  template<uint32_t SYM, int N, int D>
  currency<SYM, N, D> currency<SYM, N, D>::from_string (const std::string& str) {
    currency c;
    std::istringstream(str) >> c;
    return c;
  }


} // util
