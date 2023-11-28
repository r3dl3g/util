/**
 * @copyright (c) 2016-2023 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    utility lib
 *
 * @brief     iterator to use for_each with index
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <execution>
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t

namespace util {

  // --------------------------------------------------------------------------
  //
  // helper to use for_each with std::execution::par_unseq with indices
  //
  struct index_iterator {

    using iterator_category = std::random_access_iterator_tag;
    using difference_type   = std::size_t;
    using value_type        = std::size_t;
    using pointer           = value_type;
    using reference         = value_type;
    
    index_iterator (std::size_t i = 0)
      : value(i)
    {}

    reference operator*() {
      return value;
    }
    
    pointer operator->() {
      return value;
    }

    // Prefix increment
    index_iterator& operator++ () {
      ++value;
      return *this;
    }

    // Postfix increment
    index_iterator operator++(int) {
      return index_iterator(value++);
    }

    // Bidirectional iterator requirements
    index_iterator& operator--() {
      --value;
      return *this;
    }

    index_iterator operator--(int) {
      return index_iterator(value--);
    }

    // Random access iterator requirements
    reference operator[] (const difference_type& n) const {
      return value + n;
    }

    bool operator< (const index_iterator& rhs) const {
      return value < rhs.value;
    }

    index_iterator& operator+= (const difference_type& n) {
      value += n;
      return *this;
    }

    index_iterator& operator-= (const difference_type& n) {
      value -= n;
      return *this;
    }

    difference_type operator+ (const index_iterator& rhs) const {
      return value + rhs.value;
    }

    difference_type operator- (const index_iterator& rhs) const {
      return value - rhs.value;
    }

    const value_type& base() const {
      return value;
    }

    friend bool operator== (const index_iterator& a, const index_iterator& b) {
      return (a.value == b.value);
    }
    
    friend bool operator!= (const index_iterator& a, const index_iterator& b) {
      return (a.value != b.value);
    }

  private:
    std::size_t value;
  };

} // util
