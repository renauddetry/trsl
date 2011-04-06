// (C) Copyright Renaud Detry   2007-2011.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_ERROR_HANDLING_HPP
#define TRSL_ERROR_HANDLING_HPP

#include <stdexcept>

namespace trsl {

  /** @brief Exception for runtime errors in TRSL. */
  class runtime_error : public std::runtime_error
  {
  public:
    runtime_error(const std::string& s) : std::runtime_error(s) {}
  };

  /**
   * @brief Thrown when a TRSL component receives a parameter
   * that has a forbidden value.
   */
  class bad_parameter_value : public runtime_error
  {
  public:
    bad_parameter_value(const std::string& s) : runtime_error(s) {}
  };

}

#endif // include guard
