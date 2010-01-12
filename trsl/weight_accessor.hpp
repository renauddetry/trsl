// (C) Copyright Renaud Detry   2007-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_WEIGHT_ACCESSOR_HPP
#define TRSL_WEIGHT_ACCESSOR_HPP

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>

namespace trsl {
  
  /**
   * @brief Weight accessor that always returns 1.
   *
   * This accessor can be passed to sample from a population of
   * equal-weight elements. The total weight of the population should
   * naturally be set to the number of elements in the population.
   * This accessor is of type <em>functor</em>, see @ref accessor for
   * more details.
   */
  template<typename WeightType = double>
  struct unit_weight_accessor
  {
  private:
    BOOST_STATIC_ASSERT((boost::is_floating_point<WeightType>::value));
  public:
    /**
     * @brief Functor implementation.
     *
     * @return 1.
     */
    template<typename ElementType>
    WeightType operator()(ElementType const& e) const
      {
        return 1;
        // You can create your own accessor by copying this class
        // and returning something like
        //   return e.getWeight();
      }
  };

  /**
   * @brief Weight accessor that returns the value of the given element.
   *
   * This accessor can be passed to sample from a population of
   * elements that can be casted to a float value representing their weight.
   * This accessor is of type <em>functor</em>, see @ref accessor for
   * more details.
   */
  template<typename WeightType = double>
  struct identity_weight_accessor
  {
  private:
    BOOST_STATIC_ASSERT((boost::is_floating_point<WeightType>::value));
  public:
    /**
     * @brief Functor implementation.
     *
     * @return e.
     */
    template<typename ElementType>
    WeightType operator()(ElementType const& e) const
    {
      return e;
    }
  };
  
}

#endif // include guard
