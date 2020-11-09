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
* @brief     C++ API: std::string utilities
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <string>
#include <vector>
#include <iterator>
#include <sstream>
#include <iomanip>


// --------------------------------------------------------------------------
//
// Library includes
//
#include <util-export.h>
#include <util/ostreamfmt.h>

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

#if __cplusplus <= 201103L

    struct quoted_out {
      inline quoted_out (const std::string& t)
        : text(t)
      {}
      inline quoted_out (const char* t)
        : text(t)
      {}

      std::string text;
    };

    struct quoted_in {
      inline quoted_in (std::string& t)
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

#else

    inline auto quoted (const std::string& t) {
      return std::quoted(t);
    }

    inline auto quoted (const char* t) {
      return std::quoted(t);
    }

    inline auto quoted (std::string& t) {
      return std::quoted(t);
    }

#endif

    struct name_in {
      name_in (std::string& t)
        : text(t)
      {}

      std::string& text;
    };

    UTIL_EXPORT std::istream& operator >> (std::istream& is, name_in&&);

    inline name_in name (std::string& t) {
      return name_in(t);
    }

    template<class _InIt, class _OutIt, class _Pr>
    inline void copy_until (_InIt _First, _InIt _Last, _OutIt _Dest, _Pr _Pred) {
      while ((_First != _Last) && _Pred(*_First)) {
        *_Dest++ = *_First++;
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
        T t = T();
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

  } // namespace utf8

  namespace bom {
    // --------------------------------------------------------------------------
    struct utf_bom_t {
      const static utf_bom_t no_utf;
      const static utf_bom_t utf_8;
      const static utf_bom_t utf_32le;
      const static utf_bom_t utf_32be;
      const static utf_bom_t utf_16le;
      const static utf_bom_t utf_16be;

      utf_bom_t (uint8_t s = 0, char c0 = 0, char c1 = 0, char c2 = 0, char c3 = 0);
      utf_bom_t (std::istream&);

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
