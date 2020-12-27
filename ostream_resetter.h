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
#include <ostream>

namespace util {

  class UTIL_EXPORT ostream_resetter {
  public:
    inline ostream_resetter (std::ostream& out)
      : m_out(out)
      , m_fill(out.fill())
      , m_width(out.width())
    {}

    inline ~ostream_resetter () {
      m_out.fill(m_fill);
      m_out.width(m_width);
    }

  private:
    std::ostream& m_out;
    const char m_fill;
    std::streamsize m_width;
  };

} // namespace util
