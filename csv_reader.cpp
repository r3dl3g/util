/**
 * @copyright (c) 2018-2018 Ing. Buero Rothfuss
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
 * @brief     C++ API: command_line
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//

// --------------------------------------------------------------------------
//
// Library includes
//
#include "csv_reader.h"

namespace util {

  namespace csv {

    reader::reader (char delimiter, bool ignore)
      : csv_delimiter(delimiter)
      , ignore_first_line(ignore)
    {}

    void reader::read_csv_data (std::istream& in, std::function<void(const string_list&)> fn) const {
      bool ignoreFirst = is_ignore_first_line();
      while (in.good()) {
        string_list line = parse_csv_line(in);
        if (ignoreFirst) {
          ignoreFirst = false;
        } else {
          fn(line);
        }
      }
    }

    reader::string_list reader::parse_csv_line (std::istream& in) const {
      string_list list;

      int ch = in.get();
      while ((ch == '\n') || (ch == '\r')) {
        ch = in.get();
      }
      list.emplace_back(csv::parse_entry(in, ch, get_csv_delimiter()));
      while (ch == get_csv_delimiter()) {
        ch = in.get();
        list.emplace_back(csv::parse_entry(in, ch, get_csv_delimiter()));
      }
      return list;
    }

    /*
     * Parse a buffer until the endChar is found or the stream end is reached
     */
    std::string parse_text (std::istream& in, int& ch) {
      const int endChar = ch;
      std::ostringstream buffer;
      while (ch != -1) {
        ch = in.get();
        if (ch == endChar) {
          ch = in.get();
          if (ch != endChar) {
            return buffer.str();
          }
        }
        buffer.put((char) ch);
      }
      return buffer.str();
    }

    /*
     * Parses a next until the split char or a line is found, or the end of the stream is reached.
     */
    std::string parse_none_text (std::istream& in, int& ch, int splitChar) {
      std::ostringstream buffer;
      while ((ch != splitChar) && (ch != '\n') && (ch != '\r') && (ch != -1)) {
        buffer.put((char) ch);
        ch = in.get();
      }
      return buffer.str();
    }

    /*
     * Parses the next entry from a csv file.
     */
    std::string parse_entry (std::istream& in, int& ch, int splitChar) {
      if ((ch == '"') || (ch == '\'')) {
        return parse_text(in, ch);
      } else {
        return parse_none_text(in, ch, splitChar);
      }
    }

  } // namespace csv

} // namespace util
