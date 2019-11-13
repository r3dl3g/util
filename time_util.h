/**
* @copyright (c) 2015-2019 Ing. Buero Rothfuss
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
* @brief     C++ API: time utilities
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <string>
#include <chrono>
#include <iosfwd>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util-export.h>
#include "sys_fs.h"


/**
* Provides an API to stream into OutputDebugString.
*/
namespace util {

  namespace time {

    // --------------------------------------------------------------------------
    using time_point = std::chrono::system_clock::time_point;
#if (__cplusplus >= 201700L) || defined (BSD)
    using file_time_point = sys_fs::file_time_type;
#endif
    using duration = std::chrono::system_clock::duration;

    // --------------------------------------------------------------------------
    std::tm time_t2tm (const std::time_t now);
    std::time_t tm2time_t (const std::tm& t);

    std::tm mktm (int year = 0, uint8_t month = 0, uint8_t day = 0,
                  uint8_t hour = 0, uint8_t minute = 0, uint8_t second = 0);

    time_point mktime_point (int year = 0, uint8_t month = 0, uint8_t day = 0,
                             uint8_t hour = 0, uint8_t minute = 0, uint8_t second = 0,
                             unsigned millis = 0);

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::tm local_time (time_point const& tp);
    UTIL_EXPORT std::tm local_time_now ();

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::ostream& format_datetime (std::ostream&,
                                               time_point const& tp,
                                               const char* year_delem = "-",
                                               const char* separator = " ",
                                               const char* time_delem = ":",
                                               bool add_millis = false);

    UTIL_EXPORT std::string format_datetime (time_point const& tp,
                                             const char* year_delem = "-",
                                             const char* separator = " ",
                                             const char* time_delem = ":",
                                             bool add_millis = false);

#if (__cplusplus >= 201700L) || defined (BSD)
    UTIL_EXPORT std::string format_datetime (file_time_point const& tp,
                                             const char* year_delem = "-",
                                             const char* separator = " ",
                                             const char* time_delem = ":",
                                             bool add_millis = false);
#endif

    UTIL_EXPORT time_point parse_datetime (const std::string& s);
    UTIL_EXPORT time_point parse_datetime (std::istream& in);

    UTIL_EXPORT std::ostream& operator<< (std::ostream&, time_point const&);
    UTIL_EXPORT std::istream& operator>> (std::istream&, time_point&);

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::ostream& format_date (std::ostream&,
                                           time_point const& tp,
                                           const char* delem = "-");

    UTIL_EXPORT std::string format_date (time_point const& tp,
                                         const char* delem = "-");

    UTIL_EXPORT time_point parse_date (const std::string& s);
    UTIL_EXPORT time_point parse_date (std::istream& in);

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::ostream& format_duration (std::ostream&,
                                               duration const& d,
                                               const char* separator = " ",
                                               const char* time_delem = ":",
                                               bool add_millis = false);

    UTIL_EXPORT std::ostream& format_duration_mt (std::ostream& out,
                                                  duration const& d,
                                                  int hours_per_mt = 8,
                                                  const char* separator = " ",
                                                  const char* time_delem = ":",
                                                  bool add_millis = false);

    UTIL_EXPORT std::ostream& format_duration_only_h (std::ostream& out,
                                                      duration const& d,
                                                      const char* time_delem = ":",
                                                      bool add_millis = false);

    UTIL_EXPORT std::string format_duration (duration const& d,
                                             const char* separator = " ",
                                             const char* time_delem = ":",
                                             bool add_millis = false);

    UTIL_EXPORT duration parse_duration (const std::string& s);
    UTIL_EXPORT duration parse_duration (std::istream& in);

    UTIL_EXPORT std::ostream& operator<< (std::ostream&, duration const&);
    UTIL_EXPORT std::istream& operator>> (std::istream&, duration&);

  } // namespace time

} // namespace util
