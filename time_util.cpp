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
#include "ostream_resetter.h"


/**
* Provides an API to stream into OutputDebugString.
*/
namespace util {

  namespace time {

    // --------------------------------------------------------------------------
    std::tm mktm (int year, int month, int day, int hour, int minute, int second, int isdst) {
      return std::tm{ second, minute, hour, day, tm_mon(month), tm_year(year), 0, 0, isdst };
    }

    // --------------------------------------------------------------------------
    std::tm time_t2tm (const std::time_t now) {
      std::tm t{};
#ifdef WIN32
      localtime_s(&t, &now);
#else
      localtime_r(&now, &t);
#endif
      return t;
    }

    // --------------------------------------------------------------------------
    std::tm time_t2utc (const std::time_t now) {
      std::tm t{};
#ifdef WIN32
      gmtime_s(&t, &now);
#else
      gmtime_r(&now, &t);
#endif
      return t;
    }

    // --------------------------------------------------------------------------
    time_point time_t2time_point (std::time_t t) {
      return std::chrono::system_clock::from_time_t(t);
    }

    std::time_t time_point2time_t (time_point tp) {
      return std::chrono::system_clock::to_time_t(tp);
    }

    // --------------------------------------------------------------------------
    std::time_t get_local_time_offset () {
      static std::time_t offset = [] () {
        std::tm t_ = mktm(2000, 1, 1, 0, 0, 0, 0);
        const auto t = std::mktime(&t_);
        std::tm tl_ = time_t2tm(t);
        const auto tl = std::mktime(&tl_);
        std::tm tu_ = time_t2utc(t);
        const auto tu = std::mktime(&tu_);
        return (tl - tu);
      } ();
      return offset;
    }

    // --------------------------------------------------------------------------
    std::time_t tm2time_t (const std::tm& t_) {
      std::tm t = t_;
      return std::mktime(&t);
    }

    std::time_t tm2time_t (std::tm&& t) {
      return std::mktime(&t);
    }

    time_point mktime_point (int year, int month, int day, int hour, int minute, int second, int millis, int isdst) {
      return std::chrono::system_clock::from_time_t(tm2time_t(mktm(year, month, day, hour, minute, second, isdst))) +
          std::chrono::milliseconds(millis);
    }

    // --------------------------------------------------------------------------
    std::tm local_time (time_point const& tp) {
      return time_t2tm(std::chrono::system_clock::to_time_t(tp));
    }

    std::tm local_time_now () {
      return local_time(std::chrono::system_clock::now());
    }

    // --------------------------------------------------------------------------
    std::time_t utc2time_t (const std::tm& t) {
      return tm2time_t(t) + get_local_time_offset();
    }

    std::time_t utc2time_t (std::tm&& t) {
      return tm2time_t(std::move(t)) + get_local_time_offset();
    }

    time_point mktime_point_from_utc (int year, int month, int day, int hour, int minute, int second, int millis) {
      return std::chrono::system_clock::from_time_t(utc2time_t(mktm(year, month, day, hour, minute, second, 0))) +
          std::chrono::milliseconds(millis);
    }

    std::tm utc_time (time_point const& tp) {
      return time_t2utc(std::chrono::system_clock::to_time_t(tp));
    }

    std::tm utc_time_now () {
      return utc_time(std::chrono::system_clock::now());
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
      const auto wd = weekday_of(tm);
      if (wd == 0) { // already monday
        return t;
      }
      return tm2time_t(mktm(year_of(tm), 1, tm.tm_yday - wd + 1));
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

    std::string format_datetime (const std::tm& t,
                                 const char* date_delem,
                                 const char* separator,
                                 const char* time_delem) {
      std::ostringstream os;
      format_datetime(os, t, date_delem, separator, time_delem);
      return os.str();
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
      std::ostringstream os;
      format_datetime(os, tp, date_delem, separator, time_delem);
      return os.str();
    }

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::ostream& format_datetime (std::ostream& out,
                                               time_point const& tp,
                                               const char* date_delem,
                                               const char* separator,
                                               const char* time_delem,
                                               bool add_micros) {
      format_datetime(out, time_t2tm(std::chrono::system_clock::to_time_t(tp)), date_delem, separator, time_delem);

      if (add_micros) {
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
                                 bool add_micros) {
      std::ostringstream os;
      format_datetime(os, tp, date_delem, separator, time_delem, add_micros);
      return os.str();
    }

    // --------------------------------------------------------------------------
#if (USE_FILE_TIME_POINT)
    std::string format_datetime (file_time_point const& ftp,
                                 const char* date_delem,
                                 const char* separator,
                                 const char* time_delem,
                                 bool add_micros) {
#if WIN32
# if defined USE_MINGW
      using namespace std::chrono;
      auto sctp = time_point_cast<system_clock::duration>(ftp - file_time_point::clock::now()
                  + system_clock::now());
      const auto tse = system_clock::to_time_t(sctp);
      const time_point tp = time_point(time_point::duration(tse));
# elif _MSVC_LANG < 20173L
      const auto tse = ftp.time_since_epoch().count() - __std_fs_file_time_epoch_adjustment;
      const time_point tp = time_point(time_point::duration(tse));
# else
      const auto tse = ftp.time_since_epoch().count() - std::filesystem::__std_fs_file_time_epoch_adjustment;
      const time_point tp = time_point(time_point::duration(tse));
# endif
#elif __cplusplus > 201703L // C++20
      const auto systemTime = std::chrono::clock_cast<std::chrono::system_clock>(ftp);
      const time_point tp = std::chrono::system_clock::to_time_t(systemTime);
#elif __cplusplus < 201401L
      // this is gcc 5.1 or pior
      std::time_t tt = file_time_point::clock::to_time_t(ftp);
      const auto tp = std::chrono::system_clock::from_time_t(tt);
#else
      using namespace std::chrono;
      const time_point tp = time_point_cast<system_clock::duration>(ftp - file_time_point::clock::now()
                  + system_clock::now());
#endif
      return format_datetime(tp, date_delem, separator, time_delem, add_micros);
    }
#endif

    bool skip_delemiter (std::istream& is) {
      char ch = is.peek();
      while (is.good() && (('.' == ch) || (':' == ch) || ('-' == ch) || (' ' == ch) || ('\\' == ch) || ('T' == ch))) {
        is.ignore();
        ch = is.peek();
      }
      return is.good() && ('\n' != ch) && ('\r' != ch);
    }

    time_point parse_datetime (const std::string& s) {
      std::istringstream is(s);
      return parse_datetime(is);
    }

    time_point parse_datetime (std::istream& is) {
      int year = 0, month = 1, day = 1, hour = 0, minute = 0, second = 0, millis = 0;
      if (skip_delemiter(is)) {
        is >> year;
        if (skip_delemiter(is)) {
          is >> month;
          if (skip_delemiter(is)) {
            is >> day;
            if (skip_delemiter(is)) {
              is >> hour;
              if (skip_delemiter(is)) {
                is >> minute;
                if (skip_delemiter(is)) {
                  is >> second;
                  if (is.good() && (is.peek() == '.')) {
                    if (skip_delemiter(is)) {
                      is >> millis;
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
      std::ostringstream is;
      format_time(is, t, delem);
      return is.str();
    }

    // --------------------------------------------------------------------------
    UTIL_EXPORT std::ostream& format_date (std::ostream& out,
                                           const std::tm& t,
                                           const char* delem) {
      ostream_resetter r(out);
      out << std::setfill('0') << year_of(t) << delem
          << std::setw(2) << month_of(t) << delem
          << std::setw(2) << day_of(t);

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
    UTIL_EXPORT time_point parse_date (std::istream& is) {
      int year = 0, month = 1, day = 1;
      if (skip_delemiter(is)) {
        is >> year;
        if (skip_delemiter(is)) {
          is >> month;
          if (skip_delemiter(is)) {
            is >> day;
          }
        }
      }

      return mktime_point(year, month, day);
    }

    UTIL_EXPORT time_point parse_date (const std::string& s) {
      std::istringstream is(s);
      return parse_date(is);
    }

    // --------------------------------------------------------------------------
    duration_parts duration2parts (duration const& d) {
      using namespace std::chrono;
      const auto hrs = duration_cast<hours>(d);
      const auto mins = duration_cast<minutes>(d - hrs);
      const auto secs = duration_cast<seconds>(d - hrs - mins);
      const auto ms = duration_cast<microseconds>(d - hrs - mins - secs);
      return {
        static_cast<int>(hrs.count()),
        static_cast<int>(mins.count()),
        static_cast<int>(secs.count()),
        static_cast<int>(ms.count())
      };
    }

    duration parts2duration (const duration_parts& p) {
      using namespace std::chrono;
      return hours(p.hours) + minutes(p.mins) + seconds(p.secs) + microseconds(p.micros);
    }

    // --------------------------------------------------------------------------
    std::ostream& format_duration_mt (std::ostream& out,
                                      duration const& d,
                                      int hours_per_mt,
                                      const char* separator,
                                      const char* time_delem,
                                      bool add_micros,
                                      bool minimize) {
      ostream_resetter r(out);

      duration_parts p = duration2parts(d);

      auto days = p.hours / hours_per_mt;
      p.hours %= hours_per_mt;
      bool has_prefix = !minimize;
      if (days || has_prefix) {
        out << days << separator;
        has_prefix = true;
      }
      out << std::setfill('0');
      if (p.hours || has_prefix) {
        out << std::setw(2) << p.hours << time_delem;
        has_prefix = true;
      }
      if (p.mins || has_prefix) {
        out << std::setw(2) << p.mins << time_delem;
        has_prefix = true;
      }
      if (has_prefix) {
        out << std::setw(2);
      }
      out << p.secs;
      if (add_micros) {
        out << '.' << std::setfill('0') << std::setw(6) << p.micros;
      }
      return out;
    }

    // --------------------------------------------------------------------------
    std::ostream& format_duration_only_h (std::ostream& out,
                                          duration const& d,
                                          const char* time_delem,
                                          bool add_micros,
                                          bool minimize) {
      ostream_resetter r(out);

      duration_parts p = duration2parts(d);
      out << std::setfill('0');
      bool has_prefix = !minimize;
      if (p.hours || has_prefix) {
        out << std::setw(2) << p.hours << time_delem;
        has_prefix = true;
      }
      if (p.mins || has_prefix) {
        out << std::setw(2) << p.mins << time_delem;
        has_prefix = true;
      }
      if (has_prefix) {
        out << std::setw(2);
      }
      out << p.secs;
      if (add_micros) {
        out << '.' << std::setfill('0') << std::setw(6) << p.micros;
      }
      return out;
    }
    // --------------------------------------------------------------------------
    std::ostream& format_duration (std::ostream& out,
                                   duration const& d,
                                   const char* separator,
                                   const char* time_delem,
                                   bool add_micros,
                                   bool minimize) {
      return format_duration_mt(out, d, 24, separator, time_delem, add_micros, minimize);
    }

    // --------------------------------------------------------------------------
    std::string format_duration (duration const& d,
                                 const char* separator,
                                 const char* time_delem,
                                 bool add_micros,
                                 bool minimize) {
      std::ostringstream os;
      format_duration(os, d, separator, time_delem, add_micros, minimize);
      return os.str();
    }

    // --------------------------------------------------------------------------
    std::string format_duration_mt (duration const& d,
                                    int hours_per_mt,
                                    const char* separator,
                                    const char* time_delem,
                                    bool add_micros,
                                    bool minimize) {
      std::ostringstream os;
      format_duration_mt(os, d, hours_per_mt, separator, time_delem, add_micros, minimize);
      return os.str();
    }

    // --------------------------------------------------------------------------
    std::string format_duration_only_h (duration const& d,
                                        const char* time_delem,
                                        bool add_micros,
                                        bool minimize) {
      std::ostringstream os;
      format_duration_only_h(os, d, time_delem, add_micros, minimize);
      return os.str();
    }

    // --------------------------------------------------------------------------
    duration parse_duration (const std::string& s) {
      std::istringstream is(s);
      return parse_duration(is);
    }

    duration parse_duration (std::istream& is) {
      int day = 0, hour = 0, minute = 0, second = 0, millis = 0;

      if (skip_delemiter(is)) {
        is >> day;
        if (skip_delemiter(is)) {
          is >> hour;
          if (skip_delemiter(is)) {
            is >> minute;
            if (skip_delemiter(is)) {
              is >> second;
              if (is.good() && (is.peek() == '.')) {
                if (skip_delemiter(is)) {
                  is >> millis;
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
