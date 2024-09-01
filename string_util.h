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
 * @brief     C++ API: std::string utilities
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <cstring>
#include <string>
#include <iomanip>
#include <iterator>
#include <vector>
#include <cstdint>


// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/ostreamfmt.h>
#include <util/util-export.h>


/**
* Provides an API to stream into OutputDebugString.
*/
namespace util {

  namespace string {

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::string utf16_to_utf8 (const std::wstring&);
    UTIL_EXPORT std::string utf16_to_utf8 (wchar_t wc);
    UTIL_EXPORT std::wstring utf8_to_utf16 (const std::string&);

    // --------------------------------------------------------------------------
    UTIL_EXPORT bool starts_with (const std::string& str, const std::string& prefix);
    UTIL_EXPORT bool ends_with (const std::string& str, const std::string& suffix);

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::string::size_type find_left_space (const std::string& text, std::size_t cursor_pos);
    UTIL_EXPORT std::string::size_type find_right_space (const std::string& text, std::size_t cursor_pos);

    // --------------------------------------------------------------------------
    template<char delimiter>
    class delimited : public std::string {};

    template<char delimiter>
    std::istream& operator>> (std::istream& is, delimited<delimiter>& output) {
      std::getline(is, output, delimiter);
      return is;
    }

    template<char delimiter>
    std::vector<std::string> split (const std::string& t) {
      std::istringstream iss(t);
      typedef std::istream_iterator<delimited<delimiter>> iterator;
      std::vector<std::string> v(iterator{iss}, iterator{});
      if (!t.empty() && (t.back() == '\n')) {
        v.emplace_back(std::string());
      }
      return v;
    }

    UTIL_EXPORT std::string merge (const std::vector<std::string>& v, const std::string& delimiter);

    template<char delimiter>
    std::string merge (const std::vector<std::string>& v) {
      return merge(v, std::string(1, delimiter));
    }

    // trim front
    UTIL_EXPORT void ltrim (std::string& s);
    UTIL_EXPORT std::string ltrimed (std::string s);

    // trim end
    UTIL_EXPORT void rtrim (std::string& s);
    UTIL_EXPORT std::string rtrimed (std::string s);

    // trim both ends
    UTIL_EXPORT void trim (std::string& s);
    UTIL_EXPORT std::string trimed (std::string s);

    // replace sequence in string
    UTIL_EXPORT void replace (std::string& s, const std::string& from, const std::string& to);
    UTIL_EXPORT void replace (std::string& s, const char* from, const char* to);

    UTIL_EXPORT std::string replaced (std::string s, const std::string& from, const std::string& to);
    UTIL_EXPORT std::string replaced (std::string s, const char* from, const char* to);

#if defined(__cpp_lib_quoted_string_io)

    inline auto quoted (const std::string& t) {
      return std::quoted(t);
    }

    inline auto quoted (const char* t) {
      return std::quoted(t);
    }

    inline auto quoted (std::string& t) {
      return std::quoted(t);
    }

#else

    struct quoted_out {
      inline explicit quoted_out (const std::string& t)
        : text(t)
      {}

      inline explicit quoted_out (std::string&& t)
        : text(std::move(t))
      {}

      inline explicit quoted_out (const char* t)
        : text(t)
      {}

      std::string text;
    };

    struct UTIL_EXPORT quoted_in {
      inline explicit quoted_in (std::string& t)
        : text(t)
      {}

      std::string& text;
    };

    UTIL_EXPORT std::ostream& operator<< (std::ostream& os, const quoted_out&);
    UTIL_EXPORT std::istream& operator>> (std::istream& is, quoted_in&&);

    inline quoted_out quoted (const std::string& t) {
      return quoted_out(t);
    }

    inline quoted_out quoted (const char* t) {
      return quoted_out(t);
    }

    inline quoted_in quoted (std::string& t) {
      return quoted_in(t);
    }

#endif

    struct UTIL_EXPORT name_in {
      explicit name_in (std::string& t)
        : text(t)
      {}

      std::string& text;
    };

    UTIL_EXPORT std::istream& operator >> (std::istream& is, name_in&&);

    inline name_in name (std::string& t) {
      return name_in(t);
    }

    template<class InIt, class OutIt, class Pr>
    inline void copy_until (InIt first, InIt last, OutIt dest, Pr pred) {
      while ((first != last) && pred(*first)) {
        *dest++ = *first++;
      }
    }

    namespace convert {

      template<typename T>
      inline std::string from (const T& t) {
        return ostreamfmt(t);
      }

      template<>
      inline std::string from<std::string>(const std::string& t) {
        return t;
      }

      template<typename T>
      inline T to (const std::string& s) {
        T t = {};
        std::istringstream(s) >> t;
        return t;
      }

      // some specialisation for string to string conversion.
      template<>
      inline std::string to<std::string> (const std::string& s) {
        return s;
      }

    }


  } // namespace string

  namespace utf8 {

    UTIL_EXPORT bool is_continuation_char (char ch);

    UTIL_EXPORT std::size_t get_left_char (const std::string& s, std::size_t pos);
    UTIL_EXPORT std::size_t get_right_char (const std::string& s, std::size_t pos);

    // literal to convert a single utf-8 character code point sequence to a constexpr uint32
    // f.e. to use it as template parameter.
    namespace literals {

      constexpr uint32_t operator "" _UTF8(const char* utf8, std::size_t i) {
#if defined(WIN32) && (_MSC_VER < 1901)
        typedef unsigned char uc;
        return static_cast<uc>(utf8[0]) |
          (i > 0 ? static_cast<uc>(utf8[1]) << 8 : 0) |
          (i > 1 ? static_cast<uc>(utf8[2]) << 16 : 0) |
          (i > 2 ? static_cast<uc>(utf8[3]) << 24 : 0);
#else
        uint32_t w = static_cast<unsigned char>(utf8[0]);
        for (int n = 1; n < i; ++n) {
          w |= static_cast<unsigned char>(utf8[n]) << (8 * n);
        }
        return w;
      }
#endif // _MSC_VER

    }

    // helper class to keep a char array with defined size
    template<std::size_t N>
    struct utf8_buffer {
      char buffer[N];
      constexpr utf8_buffer (char const utf8[N] = { 0 }) {
        std::memcpy(buffer, utf8, N);
      }
    };

    namespace detail {
      union uint_utf8_union {
        uint32_t wc;
        utf8_buffer<5> cp;
      };
      union utf8_uint_union {
        utf8_buffer<5> cp;
        uint32_t wc;
        constexpr utf8_uint_union (char const utf8[5])
          : cp(utf8)
        {}
      };
    }

    // convert an utf-8 character code point sequence to an uitn32
    constexpr uint32_t utf8_to_uint32 (char const* utf8) {
      return detail::utf8_uint_union(utf8).wc;
    }

    // reconvert an uitn32 to an utf-8 character code point sequence
    constexpr utf8_buffer<5> uint32_to_utf8 (uint32_t wc) {
      return detail::uint_utf8_union{wc}.cp;
    }

  } // namespace utf8

  namespace bom {
    // --------------------------------------------------------------------------
    struct UTIL_EXPORT utf_bom_t {
      const static utf_bom_t no_utf;
      const static utf_bom_t utf_8;
      const static utf_bom_t utf_32le;
      const static utf_bom_t utf_32be;
      const static utf_bom_t utf_16le;
      const static utf_bom_t utf_16be;

      utf_bom_t ();
      utf_bom_t (uint8_t s, char c0, char c1, char c2 = 0, char c3 = 0);
      explicit utf_bom_t (std::istream&);

      void read_utf_bom (std::istream& in);

      uint8_t size;
      char c[4];
      bool operator== (const utf_bom_t& rhs) const;
      bool operator!= (const utf_bom_t& rhs) const;
    };

  } // namespace bom

  namespace win32 {

    UTIL_EXPORT std::string get_system_error_message (uint32_t msg);
    UTIL_EXPORT std::string get_last_system_error_message ();

  }

} // namespace util
