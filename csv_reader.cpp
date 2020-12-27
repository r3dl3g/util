/**
 * @copyright (c) 2018-2021 Ing. Buero Rothfuss
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

    /*
     * Parse into a buffer until the endChar is found or the stream end is reached
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
     * Parse into a buffer until the split char or a line is found, or the end of the stream is reached.
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

    std::vector<std::string> parse_csv_line (std::istream& in, int splitChar) {
      std::vector<std::string> list;

      int ch = in.get();
      while ((ch == '\n') || (ch == '\r')) {
        ch = in.get();
      }
      list.emplace_back(csv::parse_entry(in, ch, splitChar));
      while (ch == splitChar) {
        ch = in.get();
        list.emplace_back(csv::parse_entry(in, ch, splitChar));
      }
      return list;
    }

    void read_csv_data (std::istream& in, char delimiter, bool ignoreFirst, std::function<void(const std::vector<std::string>&)> fn) {
      while (in.good()) {
        std::vector<std::string> line = parse_csv_line(in, delimiter);
        if (ignoreFirst) {
          ignoreFirst = false;
        } else {
          fn(line);
        }
      }
    }

    namespace detail {

      /*
       * Parse until the endChar is found or the stream end is reached
       */
      void skip_text (std::istream& in, int& ch) {
        const int endChar = ch;
        while (ch != -1) {
          ch = in.get();
          if (ch == endChar) {
            ch = in.get();
            if (ch != endChar) {
              return;
            }
          }
        }
      }

      /*
       * Parse until the split char or a line is found, or the end of the stream is reached.
       */
      void skip_none_text (std::istream& in, int& ch, int splitChar) {
        while ((ch != splitChar) && (ch != '\n') && (ch != '\r') && (ch != -1)) {
          ch = in.get();
        }
      }

      /*
       * Parses the next entry from a csv file.
       */
      void skip_entry (std::istream& in, int& ch, int splitChar) {
        if ((ch == '"') || (ch == '\'')) {
          skip_text(in, ch);
        } else {
          skip_none_text(in, ch, splitChar);
        }
      }

      template<>
      skip csv_element<skip> (std::istream& in, int& ch, int splitChar) {
        skip_entry(in, ch, splitChar);
        ch = in.get();
        return {};
      }

    }

  } // namespace csv

} // namespace util
