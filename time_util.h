/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    utility lib
 *
 * @brief     C++ API: time utilities
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <string>
#include <chrono>
#include <sstream>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/util-export.h>
#include <util/sys_fs.h>


/**
* Provides an API to stream into OutputDebugString.
*/
namespace util {

  namespace time {

    // --------------------------------------------------------------------------
    using time_point = std::chrono::system_clock::time_point;
#if (USE_FILE_TIME_POINT)
    using file_time_point = sys_fs::file_time_type;
#endif
    using duration = std::chrono::system_clock::duration;

    // --------------------------------------------------------------------------
    UTIL_EXPORT time_point now ();

    UTIL_EXPORT std::tm mktm (int year = 0, int month = 0, int day = 0,
                              int hour = 0, int minute = 0, int second = 0, int isdst = -1);

    UTIL_EXPORT std::tm time_t2tm (std::time_t);
    UTIL_EXPORT std::tm time_t2utc (std::time_t);

    UTIL_EXPORT time_point time_t2time_point (std::time_t);
    UTIL_EXPORT std::time_t time_point2time_t (time_point);

    UTIL_EXPORT std::time_t get_local_time_offset ();

    UTIL_EXPORT std::time_t tm2time_t (const std::tm& t);
    UTIL_EXPORT std::time_t tm2time_t (std::tm&& t);

    UTIL_EXPORT time_point mktime_point (int year = 0, int month = 0, int day = 0,
                                         int hour = 0, int minute = 0, int second = 0,
                                         int millis = 0, int isdst = -1);

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::tm local_time (time_point const& tp);
    UTIL_EXPORT std::tm local_time_now ();

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::time_t utc2time_t (const std::tm& t);
    UTIL_EXPORT std::time_t utc2time_t (std::tm&& t);

    UTIL_EXPORT time_point mktime_point_from_utc (int year = 0, int month = 0, int day = 0,
                                                  int hour = 0, int minute = 0, int second = 0,
                                                  int millis = 0);

    UTIL_EXPORT std::tm utc_time (time_point const& tp);
    UTIL_EXPORT std::tm utc_time_now ();

    // --------------------------------------------------------------------------
    inline bool is_leap_year (int year) {
      return !(year % 4) && ((year % 100 ) || !(year % 400));
    }

    inline int year_of (const std::tm& t) {
      return t.tm_year + 1900;
    }

    inline int month_of (const std::tm& t) {
      return t.tm_mon + 1;
    }

    inline int day_of (const std::tm& t) {
      return t.tm_mday;
    }

    inline int weekday_of (const std::tm& t) {
      return (t.tm_wday + 6) % 7;
    }

    inline int tm_year (int year) {
      return year - 1900;
    }

    inline int tm_mon (int mon) {
      return mon - 1;
    }

    inline int tm_day (int day) {
      return day;
    }

    // --------------------------------------------------------------------------
    UTIL_EXPORT int week_of_year (const std::tm&);
    UTIL_EXPORT std::time_t first_day_of_week (int y, int w);

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::ostream& format_time (std::ostream& out, const std::tm& t, const char* delem = ":");
    UTIL_EXPORT std::string format_time (const std::tm& t, const char* delem = ":");

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::ostream& format_date (std::ostream& out, const std::tm& t, const char* delem = "-");
    UTIL_EXPORT std::ostream& format_date (std::ostream& out, const std::time_t& tp, const char* delem = "-");
    UTIL_EXPORT std::ostream& format_date (std::ostream&, time_point const& tp, const char* delem = "-");

    // --------------------------------------------------------------------------
    template<typename T>
    std::string format_date (const T& t, const char* delem = "-") {
      std::ostringstream strm;
      format_date(strm, t, delem);
      return strm.str();
    }

    // --------------------------------------------------------------------------
    UTIL_EXPORT time_point parse_date (const std::string& s);
    UTIL_EXPORT time_point parse_date (std::istream& in);

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::ostream& format_datetime (std::ostream&,
                                               const std::tm& t,
                                               const char* year_delem = "-",
                                               const char* separator = " ",
                                               const char* time_delem = ":");

    UTIL_EXPORT std::string format_datetime (const std::tm& t,
                                             const char* year_delem = "-",
                                             const char* separator = " ",
                                             const char* time_delem = ":");

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::ostream& format_datetime (std::ostream&,
                                               const std::time_t& tp,
                                               const char* year_delem = "-",
                                               const char* separator = " ",
                                               const char* time_delem = ":");

    UTIL_EXPORT std::string format_datetime (const std::time_t& tp,
                                             const char* year_delem = "-",
                                             const char* separator = " ",
                                             const char* time_delem = ":");

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::ostream& format_datetime (std::ostream&,
                                               time_point const& tp,
                                               const char* year_delem = "-",
                                               const char* separator = " ",
                                               const char* time_delem = ":",
                                               bool add_micros = false);

    UTIL_EXPORT std::string format_datetime (time_point const& tp,
                                             const char* year_delem = "-",
                                             const char* separator = " ",
                                             const char* time_delem = ":",
                                             bool add_micros = false);

    // --------------------------------------------------------------------------
#if (USE_FILE_TIME_POINT)
    UTIL_EXPORT std::string format_datetime (file_time_point const& tp,
                                             const char* year_delem = "-",
                                             const char* separator = " ",
                                             const char* time_delem = ":",
                                             bool add_micros = false);
#endif

    UTIL_EXPORT time_point parse_datetime (const std::string& s);
    UTIL_EXPORT time_point parse_datetime (std::istream& in);

    // --------------------------------------------------------------------------
    struct duration_parts {
      int hours;
      int mins;
      int secs;
      int micros;
    };

    // --------------------------------------------------------------------------
    UTIL_EXPORT duration mkduration (int hours = 0, int mins = 0, int secs = 0, int mcrsecs = 0);

    // --------------------------------------------------------------------------
    UTIL_EXPORT duration_parts duration2parts (duration const&);
    UTIL_EXPORT duration parts2duration (const duration_parts&);

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::ostream& format_duration (std::ostream&,
                                               duration const& d,
                                               const char* separator = " ",
                                               const char* time_delem = ":",
                                               bool add_micros = false,
                                               bool minimize = false);

    UTIL_EXPORT std::ostream& format_duration_mt (std::ostream& out,
                                                  duration const& d,
                                                  int hours_per_mt = 8,
                                                  const char* separator = " ",
                                                  const char* time_delem = ":",
                                                  bool add_micros = false,
                                                  bool minimize = false);

    UTIL_EXPORT std::ostream& format_duration_only_h (std::ostream& out,
                                                      duration const& d,
                                                      const char* time_delem = ":",
                                                      bool add_micros = false,
                                                      bool minimize = false);

    UTIL_EXPORT std::string format_duration (duration const& d,
                                             const char* separator = " ",
                                             const char* time_delem = ":",
                                             bool add_micros = false,
                                             bool minimize = false);

    UTIL_EXPORT std::string format_duration_mt (duration const& d,
                                                int hours_per_mt = 8,
                                                const char* separator = " ",
                                                const char* time_delem = ":",
                                                bool add_micros = false,
                                                bool minimize = false);

    UTIL_EXPORT std::string format_duration_only_h (duration const& d,
                                                    const char* time_delem = ":",
                                                    bool add_micros = false,
                                                    bool minimize = false);

    UTIL_EXPORT duration parse_duration (const std::string& s);
    UTIL_EXPORT duration parse_duration (std::istream& in);

    // --------------------------------------------------------------------------
    struct chronometer {
      typedef std::chrono::system_clock clock;
      typedef clock::duration duration;
      typedef clock::time_point time_point;

      inline chronometer () {
        start();
      }

      inline void start () {
        begin = clock::now();
      }

      inline duration stop () const {
        return clock::now() - begin;
      }

      template<typename P>
      inline duration process (P p) {
        start();
        p();
        return stop();
      }

      template<typename P>
      static inline duration run (P p) {
        chronometer c;
        p();
        return c.stop();
      }

    private:
      time_point begin;
    };

    // --------------------------------------------------------------------------
    struct average_chronometer {

      inline average_chronometer ()
        : count_(0)
        , duration_{}
      {}

      inline void start () {
        timer_.start();
      }

      inline void stop () {
        duration_ += timer_.stop();
        ++count_;
      }

      template<typename P>
      inline void process (P p) {
        start();
        p();
        stop();
      }

      inline chronometer::duration average_duration () const {
        if (count_ != 0) {
          return duration_ / count_;
        } else {
          return duration_;
        }
      }

      inline chronometer::duration cumulated_duration () const {
        return duration_;
      }

      inline std::size_t count () const {
        return count_;
      }

    private:
      std::size_t count_;
      chronometer::duration duration_;
      chronometer timer_;
    };

    // --------------------------------------------------------------------------
  } // namespace time

} // namespace util

namespace std {

  UTIL_EXPORT ostream& operator<< (ostream&, util::time::time_point const&);
  UTIL_EXPORT istream& operator>> (istream&, util::time::time_point&);

  UTIL_EXPORT ostream& operator<< (ostream&, util::time::duration const&);
  UTIL_EXPORT istream& operator>> (istream&, util::time::duration&);

} // namespace std
