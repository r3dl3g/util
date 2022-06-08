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

// --------------------------------------------------------------------------
//
// Project includes
//
#include "string_util.h"

// --------------------------------------------------------------------------
//
// Library includes
//
#include <istream>
#include <locale>
#include <codecvt>
#include <algorithm>
#include <cctype>
#include <cstring>
#ifdef WIN32
#define min min
#define max max
#include <windows.h>
#endif //WIN32


namespace util {

  namespace string {

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> s_utf8_converter;

    struct codecvt_wchar_t_char : public std::codecvt<wchar_t, char, mbstate_t> {};

    std::wstring_convert<codecvt_wchar_t_char> s_converter;

    std::string utf16_to_utf8 (const std::wstring& ws) {
      return s_utf8_converter.to_bytes(ws);
    }

    std::string utf16_to_utf8 (wchar_t wc) {
      return s_utf8_converter.to_bytes(wc);
    }

    std::wstring utf8_to_utf16 (const std::string& s) {
      try {
        return s_utf8_converter.from_bytes(s);
      } catch (...) {
        return s_converter.from_bytes(s);
      }
    }

    bool starts_with (const std::string& str, const std::string& prefix) {
      const int r = str.compare(0, prefix.length(), prefix);
      return r == 0;
    }

    bool ends_with (const std::string& str, const std::string& suffix) {
      std::string::size_type l = str.length(), sl = suffix.length();
      return (l >= sl) && (str.compare(l - sl, sl, suffix) == 0);
    }

    static const std::string white_space = " (){}[],.;:'\"!@#$%^&/*-+";

    std::string::size_type find_left_space (const std::string& text, std::size_t cursor_pos) {
      std::string::size_type pos = text.find_last_not_of(white_space, cursor_pos - 1);
      if (pos != std::string::npos) {
        std::string::size_type pos2 = text.find_last_of(white_space, pos);
        if (pos2 != std::string::npos) {
          return pos2 + 1;
        }
      }
      return 0;
    }

    std::string::size_type find_right_space (const std::string& text, std::size_t cursor_pos) {
      std::string::size_type pos = text.find_first_of(white_space, cursor_pos + 1);
      if (pos != std::string::npos) {
        std::string::size_type pos2 = text.find_first_not_of(text[pos], pos);
        if (pos2 != std::string::npos) {
          return pos2;
        } else {
          return pos;
        }
      } else {
        return text.length();
      }
    }

    std::string merge (const std::vector<std::string>& v, const std::string& delimiter) {
      std::ostringstream oss;
      if (!v.empty()) {
        std::copy(v.begin(), std::prev(v.end()), std::ostream_iterator<std::string>(oss, delimiter.c_str()));
        oss << v.back();
      }
      return oss.str();
    }

    void ltrim (std::string& s) {
      s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), isspace));
    }

    std::string ltrimed (std::string s) {
      ltrim(s);
      return s;
    }

    void rtrim (std::string& s) {
      s.erase(std::find_if_not(s.rbegin(), s.rend(), isspace).base(), s.end());
    }

    std::string rtrimed (std::string s) {
      rtrim(s);
      return s;
    }

    void trim (std::string& s) {
      ltrim(s);
      rtrim(s);
    }

    std::string trimed (std::string s) {
      trim(s);
      return s;
    }

    void replace (std::string& str, const std::string& from, const std::string& to) {
      size_t start_pos = 0;
      while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
      }
    }

    void replace (std::string& str, const char* from, const char* to) {
      size_t start_pos = 0;
      size_t from_len = std::strlen(from);
      size_t to_len = std::strlen(to);
      while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from_len, to);
        start_pos += to_len;
      }
    }

    std::string replaced (std::string s, const std::string& from, const std::string& to) {
      replace(s, from, to);
      return s;
    }

    std::string replaced (std::string s, const char* from, const char* to) {
      replace(s, from, to);
      return s;
    }

#if !defined(__cpp_lib_quoted_string_io)
    UTIL_EXPORT std::ostream& operator<< (std::ostream& os, const quoted_out& q) {
      os << '\"' << util::string::replaced(q.text, "\"", "\\\"") << '\"';
      return os;
    }

    UTIL_EXPORT std::istream& operator>> (std::istream& is, quoted_in&& q) {
      if ((is.peek() != '"') && (is.peek() != '\'')) {
        throw std::runtime_error("Expected string delemiter ' or \" !");
      }
      char delim;
      is >> delim;
      std::getline(is, q.text, delim);
      replace(q.text, "\\\"", "\"");
      return is;
    }
#endif

    std::istream& operator>> (std::istream& is, name_in&& n) {
      std::ostringstream str;
      copy_until(std::istreambuf_iterator<char>(is),
                 std::istreambuf_iterator<char>(),
                 std::ostreambuf_iterator<char>(str),
                 [] (char c) {
        return std::isalnum(c) || (c == '.') || (c == '-') || (c == '_') || (c == '/') || (c == '@') || (c == '#');
      });
      n.text = str.str();
      return is;
    }

  } // namespace string

  namespace utf8 {

    bool is_continuation_char (char ch) {
      return ((ch & 0b11000000) == 0b10000000);
    }

    std::size_t get_left_char (const std::string& s, std::size_t pos) {
      if (pos < 1) {
        return pos;
      }
      std::size_t cp = pos - 1;
      while ((cp > 0) && utf8::is_continuation_char(s.at(cp))) {
        --cp;
      }
      return cp;
    }

    std::size_t get_right_char (const std::string& s, std::size_t pos) {
      std::size_t sz = s.size();
      std::size_t cp = std::min(sz, pos + 1);
      while ((cp < sz) && utf8::is_continuation_char(s.at(cp))) {
        ++cp;
      }
      return cp;
    }

  } // utf8

  namespace bom {

    // --------------------------------------------------------------------------
    const utf_bom_t utf_bom_t::no_utf;
    const utf_bom_t utf_bom_t::utf_8 = { 3, '\xef', '\xbb', '\xbf' };
    const utf_bom_t utf_bom_t::utf_32le = { 4, '\xff', '\xfe' };
    const utf_bom_t utf_bom_t::utf_32be = { 4, 0, 0, '\xfe', '\xff' };
    const utf_bom_t utf_bom_t::utf_16le = { 2, '\xff', '\xfe' };
    const utf_bom_t utf_bom_t::utf_16be = { 2, '\xfe', '\xff' };
    // --------------------------------------------------------------------------
    namespace {
      const utf_bom_t utf_boms[] = {
        utf_bom_t::utf_8,
        utf_bom_t::utf_32le,
        utf_bom_t::utf_32be,
        utf_bom_t::utf_16le,
        utf_bom_t::utf_16be
      };
    }

    // --------------------------------------------------------------------------
    bool utf_bom_t::operator== (const utf_bom_t& rhs) const {
      return (c[0] == rhs.c[0]) &&
          (c[1] == rhs.c[1]) &&
          ((size < 3) || (c[2] == rhs.c[2])) &&
          ((size < 4) || (c[3] == rhs.c[3]));
    }
    // --------------------------------------------------------------------------
    bool utf_bom_t::operator!= (const utf_bom_t& rhs) const {
      return !operator==(rhs);
    }
    // --------------------------------------------------------------------------
    utf_bom_t::utf_bom_t ()
      : size(0)
      , c{0}
    {}
    // --------------------------------------------------------------------------
    utf_bom_t::utf_bom_t (uint8_t s, char c0, char c1, char c2, char c3)
      : size(s)
      , c{c0, c1, c2, c3}
    {}
    // --------------------------------------------------------------------------
    utf_bom_t::utf_bom_t (std::istream& in)
      : size(0)
      , c{0}
    {
      read_utf_bom(in);
    }
    // --------------------------------------------------------------------------
    void utf_bom_t::read_utf_bom (std::istream& in) {
      in.read(c, 4);
      for (const utf_bom_t& utf_bom : utf_boms) {
        if (utf_bom == *this) {
          size = utf_bom.size;
          in.seekg(size);
          return;
        }
      }
      in.seekg(0);
      *this = no_utf;
    }
  } // namespace bom

#ifdef WIN32

  namespace win32 {

    std::string get_system_error_message (uint32_t msg) {
      LPSTR buffer = nullptr;
      FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    nullptr, msg, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&buffer, 0, nullptr);
      std::string s = buffer;
      LocalFree(buffer);
      return s;
    }

    std::string get_last_system_error_message () {
      return get_system_error_message(GetLastError());
    }

  }

#endif // WIN32

} // namespace util
