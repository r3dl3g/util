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

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iomanip>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "time_util.h"
#include "string_util.h"
#include "ostreamfmt.h"
#include "ostream_resetter.h"


/**
* Provides an API to stream into OutputDebugString.
*/
namespace util {

  namespace time {

    std::tm time_t2tm (const std::time_t now) {
      // --------------------------------------------------------------------------
      std::tm t;
#ifdef WIN32
      localtime_s(&t, &now);
#else
      localtime_r(&now, &t);
#endif
      return t;
    }

    std::time_t tm2time_t (const std::tm& t_) {
      std::tm t = t_;
      return std::mktime(&t);
    }

    std::time_t tm2time_t (std::tm&& t) {
      return std::mktime(&t);
    }

    std::tm mktm (int year, int month, int day, int hour, int minute, int second) {
      return std::tm{ second, minute, hour, day, tm_mon(month), tm_year(year), 0 };
    }

    time_point mktime_point (int year, int month, int day, int hour, int minute, int second, int millis) {
      return std::chrono::system_clock::from_time_t(tm2time_t(mktm(year, month, day, hour, minute, second))) + std::chrono::milliseconds(millis);
    }

    // --------------------------------------------------------------------------
    std::tm local_time (time_point const& tp) {
      return time_t2tm(std::chrono::system_clock::to_time_t(tp));
    }

    std::tm local_time_now () {
      return local_time(std::chrono::system_clock::now());
    }

    // --------------------------------------------------------------------------
    UTIL_EXPORT int week_of_year (const std::tm& t) {
      return ((t.tm_yday + 7 - weekday_of(t)) / 7);
    }

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::time_t first_day_of_week (int year, int w) {
      const int yday = w * 7;
      const auto t = tm2time_t(mktm(year, 1, yday));
      const auto tm = time_t2tm(t);
      if (tm.tm_wday == 1) { // already monday
        return t;
      }
      return tm2time_t(mktm(year_of(tm), 1, tm.tm_yday + 1 - weekday_of(tm)));
    }

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::ostream& format_datetime (std::ostream& out,
                                               const std::tm& t,
                                               const char* date_delem,
                                               const char* separator,
                                               const char* time_delem) {
      format_date(out, t, date_delem);
      out << separator;
      format_time(out, t, time_delem);

      return out;
    }

    std::string format_datetime (time_point const& tp,
                                 const char* date_delem,
                                 const char* separator,
                                 const char* time_delem) {
      std::ostringstream strm;
      format_datetime(strm, tp, date_delem, separator, time_delem);
      return strm.str();
    }

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::ostream& format_datetime (std::ostream& out,
                                               const std::time_t& tp,
                                               const char* date_delem,
                                               const char* separator,
                                               const char* time_delem) {
      return format_datetime(out, time_t2tm(tp), date_delem, separator, time_delem);
    }

    std::string format_datetime (const std::time_t& tp,
                                 const char* date_delem,
                                 const char* separator,
                                 const char* time_delem) {
      std::ostringstream strm;
      format_datetime(strm, tp, date_delem, separator, time_delem);
      return strm.str();
    }

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::ostream& format_datetime (std::ostream& out,
                                               time_point const& tp,
                                               const char* date_delem,
                                               const char* separator,
                                               const char* time_delem,
                                               bool add_millis) {
      format_datetime(out, time_t2tm(std::chrono::system_clock::to_time_t(tp)), date_delem, separator, time_delem);

      if (add_millis) {
        auto t0 = std::chrono::time_point_cast<std::chrono::seconds>(tp);
        auto micros = std::chrono::duration_cast<std::chrono::microseconds>(tp - t0);
        ostream_resetter r(out);
        out << '.' << std::setfill('0') << std::setw(6) << micros.count();
      }

      return out;
    }

    std::string format_datetime (time_point const& tp,
                                 const char* date_delem,
                                 const char* separator,
                                 const char* time_delem,
                                 bool add_millis) {
      std::ostringstream strm;
      format_datetime(strm, tp, date_delem, separator, time_delem, add_millis);
      return strm.str();
    }

    // --------------------------------------------------------------------------
#if (__cplusplus >= 201700L) || defined (BSD)
    std::string format_datetime (file_time_point const& ftp,
                                 const char* date_delem,
                                 const char* separator,
                                 const char* time_delem,
                                 bool add_millis) {
      std::time_t now = file_time_point::clock::to_time_t(ftp);
      auto tp = std::chrono::system_clock::from_time_t(now);
      return format_datetime(tp, date_delem, separator, time_delem, add_millis);
    }
#endif

    time_point parse_datetime (const std::string& s) {
      std::istringstream strm(s);
      return parse_datetime(strm);
    }

    time_point parse_datetime (std::istream& strm) {
      int year = 0, month = 1, day = 1, hour = 0, minute = 0, second = 0, millis = 0;
      if (strm.good()) {
        strm >> year;
        while (strm.good() && !isdigit(strm.peek())) {
          strm.ignore(1);
        }
        if (strm.good()) {
          strm >> month;
          while (strm.good() && !isdigit(strm.peek())) {
            strm.ignore(1);
          }
          if (strm.good()) {
            strm >> day;
            while (strm.good() && !isdigit(strm.peek())) {
              strm.ignore(1);
            }
            if (strm.good()) {
              strm >> hour;
              while (strm.good() && !isdigit(strm.peek())) {
                strm.ignore(1);
              }
              if (strm.good()) {
                strm >> minute;
                while (strm.good() && !isdigit(strm.peek())) {
                  strm.ignore(1);
                }
                if (strm.good()) {
                  strm >> second;
                  if (strm.good() && (strm.peek() == '.')) {
                    while (strm.good() && !isdigit(strm.peek())) {
                      strm.ignore(1);
                    }
                    if (strm.good()) {
                      strm >> millis;
                    }
                  }
                }
              }
            }
          }
        }
      }

      return mktime_point(year, month, day, hour, minute, second, millis);
    }

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::ostream& format_time (std::ostream& out,
                                           const std::tm& t,
                                           const char* delem) {
      ostream_resetter r(out);
      out << std::setfill('0')
          << std::setw(2) << t.tm_hour << delem
          << std::setw(2) << t.tm_min << delem
          << std::setw(2) << t.tm_sec;

      return out;
    }

    UTIL_EXPORT std::string format_time (const std::tm& t,
                                         const char* delem) {
      std::ostringstream strm;
      format_time(strm, t, delem);
      return strm.str();
    }

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::ostream& format_date (std::ostream& out,
                                           const std::tm& t,
                                           const char* delem) {
      ostream_resetter r(out);
      out << std::setfill('0')
          << year_of(t) << delem
          << std::setw(2) << month_of(t) << delem
          << std::setw(2) << t.tm_mday;

      return out;
    }

    UTIL_EXPORT std::ostream& format_date (std::ostream& out,
                                           const std::time_t& tp,
                                           const char* delem) {
      return format_date(out, time_t2tm(tp), delem);
    }

    UTIL_EXPORT std::ostream& format_date (std::ostream& out,
                                           time_point const& tp,
                                           const char* delem) {
      return format_date(out, std::chrono::system_clock::to_time_t(tp), delem);
    }

    // --------------------------------------------------------------------------
    UTIL_EXPORT time_point parse_date (std::istream& strm) {
      int year = 0, month = 1, day = 1;
      if (strm.good()) {
        strm >> year;
        while (strm.good() && !isdigit(strm.peek())) {
          strm.ignore(1);
        }
        if (strm.good()) {
          strm >> month;
          while (strm.good() && !isdigit(strm.peek())) {
            strm.ignore(1);
          }
          if (strm.good()) {
            strm >> day;
          }
        }
      }

      return mktime_point(year, month, day);
    }

    UTIL_EXPORT time_point parse_date (const std::string& s) {
      std::istringstream strm(s);
      return parse_date(strm);
    }

    // --------------------------------------------------------------------------
    std::ostream& format_duration_mt (std::ostream& out,
                                      duration const& d,
                                      int hours_per_mt,
                                      const char* separator,
                                      const char* time_delem,
                                      bool add_millis) {
      ostream_resetter r(out);

      auto t0 = std::chrono::duration_cast<std::chrono::seconds>(d);
      auto t = t0.count();
      auto secs = t % 60;
      t = (t - secs) / 60;
      auto min = t % 60;
      t = (t - min) / 60;
      auto hours = t % hours_per_mt;
      auto days = (t - hours) / hours_per_mt;
      out << days << separator << std::setfill('0')
          << std::setw(2) << hours << time_delem
          << std::setw(2) << min << time_delem
          << std::setw(2) << secs;
      if (add_millis) {
        auto tp = std::chrono::duration_cast<std::chrono::microseconds>(d);
        auto micros = std::chrono::duration_cast<std::chrono::microseconds>(tp - t0);
        out << '.' << std::setfill('0') << std::setw(6) << micros.count();
      }
      return out;
    }

    // --------------------------------------------------------------------------
    std::ostream& format_duration_only_h (std::ostream& out,
                                          duration const& d,
                                          const char* time_delem,
                                          bool add_millis) {
      ostream_resetter r(out);

      auto t0 = std::chrono::duration_cast<std::chrono::seconds>(d);
      auto t = t0.count();
      auto secs = t % 60;
      t = (t - secs) / 60;
      auto min = t % 60;
      auto hours = (t - min) / 60;
      out << std::setfill('0')
          << std::setw(2) << hours << time_delem
          << std::setw(2) << min << time_delem
          << std::setw(2) << secs;
      if (add_millis) {
        auto tp = std::chrono::duration_cast<std::chrono::microseconds>(d);
        auto micros = std::chrono::duration_cast<std::chrono::microseconds>(tp - t0);
        out << '.' << std::setfill('0') << std::setw(6) << micros.count();
      }
      return out;
    }
    // --------------------------------------------------------------------------
    std::ostream& format_duration (std::ostream& out,
                                   duration const& d,
                                   const char* separator,
                                   const char* time_delem,
                                   bool add_millis) {
      return format_duration_mt(out, d, 24, separator, time_delem, add_millis);
    }

    // --------------------------------------------------------------------------
    std::string format_duration (duration const& d,
                                 const char* separator,
                                 const char* time_delem,
                                 bool add_millis) {
      std::ostringstream strm;
      format_duration(strm, d, separator, time_delem, add_millis);
      return strm.str();
    }

    // --------------------------------------------------------------------------
    duration parse_duration (const std::string& s) {
      std::istringstream strm(s);
      return parse_duration(strm);
    }

    duration parse_duration (std::istream& in) {
      int day = 0, hour = 0, minute = 0, second = 0, millis = 0;

      if (in.good()) {
        in >> day;
        while (in.good() && !isdigit(in.peek())) {
          in.ignore(1);
        }
        if (in.good()) {
          in >> hour;
          while (in.good() && !isdigit(in.peek())) {
            in.ignore(1);
          }
          if (in.good()) {
            in >> minute;
            while (in.good() && !isdigit(in.peek())) {
              in.ignore(1);
            }
            if (in.good()) {
              in >> second;
              if (in.good() && (in.peek() == '.')) {
                while (in.good() && !isdigit(in.peek())) {
                  in.ignore(1);
                }
                if (in.good()) {
                  in >> millis;
                }
              }
            }
          }
        }
      }

      return std::chrono::milliseconds(((((((day * 24) + hour) * 60) + minute) * 60) + second) * 1000 + millis);
    }

  } // namespace time

} // namespace util

namespace std {

  ostream& operator<< (ostream& out, util::time::time_point const& tp) {
    return util::time::format_datetime(out, tp);
  }

  istream& operator>> (istream& in, util::time::time_point& tp) {
    tp = util::time::parse_datetime(in);
    return in;
  }

  ostream& operator<< (ostream& out, util::time::duration const& d) {
    return util::time::format_duration(out, d, " ", ":", true);
  }

  istream& operator>> (istream& in, util::time::duration& d) {
    d = util::time::parse_duration(in);
    return in;
  }

} // namespace std
