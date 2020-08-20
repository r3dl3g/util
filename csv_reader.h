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

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <functional>
#include <vector>
#include <sstream>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util-export.h>


namespace util {

  namespace csv {

    struct reader {
      typedef std::vector<std::string> string_list;
      typedef string_list::const_iterator iterator;

      reader (char delimiter = ';', bool ignore = false);

      inline char get_csv_delimiter () const {
        return csv_delimiter;
      }

      inline void set_csv_delimiter (char delimiter) {
        csv_delimiter = delimiter;
      }

      bool is_ignore_first_line () const {
        return ignore_first_line;
      }

      void set_ignore_first_line (bool ignore) {
        ignore_first_line = ignore;
      }

      /**
       * Reads a csv file line by line and calls the given consumer for each parsed csv line.
       */
      void read_csv_data (std::istream& in, std::function<void(const string_list&)> fn);

      /**
       * Parses the next csv line.
       */
      string_list parse_csv_line (std::istream& in);

      /**
       * Parses the next entry from a csv file.
       */
      int parse_entry (std::istream& in, int ch, string_list& list);

      /**
       * Parses a next until the split char or a line is found, or the end of the stream is reached.
       */
      static int parse_none_text (std::istream& in, string_list& list, int ch, int splitChar);

      /**
       * Parse a buffer until the endChar is found or the stream end is reached
       */
      static int parse_text (std::istream& in, string_list& list, int endChar);

    private:
      char csv_delimiter;
      bool ignore_first_line;
    };

  } // namespace csv

} // namespace util
