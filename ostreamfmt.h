/**
* @copyright (c) 2015-2021 Ing. Buero Rothfuss
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
* @brief     C++ API:ostreamfmt
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <sstream>

/**
* Allows to use ostringstream formatting as one-line.
* F.e.: ostreamfmt("Hello Word" << 2) produces a std:sstring with "Hello Word2".
*/
#define ostreamfmt(a) static_cast<const std::ostringstream&>(std::ostringstream() << a).str() // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)

struct str_fmt {
  template <typename T>
  inline str_fmt& operator<< (T const& value) {
    buffer << value;
    return *this;
  }

  inline operator std::string () const {
    return buffer.str();
  }

private:
  std::ostringstream buffer;
};
