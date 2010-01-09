// (C) Copyright Renaud Detry   2007-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_WEIGHT_ACCESSOR_HPP
#define TRSL_WEIGHT_ACCESSOR_HPP

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
  template<typename WeightType>
  struct unit_weight_accessor
  {
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
   * @brief Method Pointer weight accessor.
   *
   * Weight accessor for element classes that provide access to
   * their weight through a method signed <tt>WeightType
   * (ElementType::*)() const</tt>.
   *
   * This class is very similar to
   * <tt>std::const_mem_fun_ref_t</tt>. The only difference is in its constructor which doesn't have to be explicit.
   *
   * See @ref accessor for more details.
   */
  template<typename WeightType, typename ElementType>
  class mp_weight_accessor
  {
  public:
    /** @brief Pointer to a const method of ElementType that returns double */
    typedef WeightType (ElementType::*WeightAccessorMethodPointer)() const;
    
    /**
     * @brief Constructor from a WeightAccessorMethodPointer.
     *
     * @param wptr Pointer to the method of ElementType that
     * returns the element weight. If no pointer is passed, operator()
     * will return 1 all the time.
     *
     */
    mp_weight_accessor(WeightAccessorMethodPointer wptr) :
      wptr_(wptr) {}
    
    /**
     * @brief Functor implementation.
     *
     * @return <tt>e.*wptr_()</tt>.
     */ 
    WeightType operator()(ElementType const& e) const
      {
        return (e.*wptr_)();
      }
  private:
    WeightAccessorMethodPointer wptr_;
  };

}

#endif // include guard
