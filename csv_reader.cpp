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

    void reader::read_csv_data (std::istream& in, std::function<void(const string_list&)> fn) {
      string_list line;
      bool ignoreFirst = is_ignore_first_line();
      while ((line = parse_csv_line(in)).size() > 0) {
        if (ignoreFirst) {
          ignoreFirst = false;
        } else {
          fn(line);
        }
      }
    }

    reader::string_list reader::parse_csv_line (std::istream& in) {
      string_list list;

      int ch = in.get();
      while ((ch == '\n') || (ch == '\r')) {
        ch = in.get();
      }
      ch = parse_entry(in, ch, list);
      while (ch == get_csv_delimiter()) {
        ch = parse_entry(in, in.get(), list);
      }
      return list;
    }

    /*
     * Parses the next entry from a csv file.
     */
    int reader::parse_entry (std::istream& in, int ch, string_list& list) {
      if ((ch == '"') || (ch == '\'')) {
        ch = parse_text(in, list, ch);
      } else if (ch != -1) {
        ch = parse_none_text(in, list, ch, get_csv_delimiter());
      }
      return ch;
    }

    /*
     * Parses a next until the split char or a line is found, or the end of the stream is reached.
     */
    int reader::parse_none_text (std::istream& in, string_list& list, int ch, int splitChar) {
      std::ostringstream buffer;
      while ((ch != splitChar) && (ch != '\n') && (ch != '\r') && (ch != -1)) {
        buffer.put((char) ch);
        ch = in.get();
      }
      list.emplace_back(buffer.str());
      return ch;
    }

    /*
     * Parse a buffer until the endChar is found or the stream end is reached
     */
    int reader::parse_text (std::istream& in, string_list& list, int endChar) {
      std::ostringstream buffer;
      int ch = endChar;
      while (ch != -1) {
        ch = in.get();
        if (ch == endChar) {
          ch = in.get();
          if (ch != endChar) {
            list.emplace_back(buffer.str());
            return ch;
          }
        }
        buffer.put((char) ch);
      }
      return -1;
    }

  } // namespace csv

} // namespace util
