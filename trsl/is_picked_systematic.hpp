// (C) Copyright Renaud Detry   2007.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_IS_PICKED_SYSTEMATIC_HPP
#define TRSL_IS_PICKED_SYSTEMATIC_HPP

#include "trsl/persistent_filter_iterator.hpp"
#include "trsl/weight_accessor.hpp"

#include <algorithm>
#include <functional>
#include <utility>
#include <cassert>
#include <cstdlib> // rand & random

/** @brief public namespace */
namespace trsl {
    
  /**
   * @brief Functor to use with persistent_filter_iterator for range
   * sampling using systematic sampling.
   *
   * This class is meant to allow iteration over a sample from a
   * population. The sampling method is systematic sampling, see [1, 2].
   *
   * This class may be used as a functor to the
   * trsl::persistent_filter_iterator iterator adaptor. The
   * persistent_filter_iterator should be given an original
   * population, viewed as a range referenced by two Forward
   * Iterators. The persistent_filter_iterator will then allow to
   * iterate over a sample of the original population.
   *
   * Note that the iterator to use with this class is
   * trsl::persistent_filter_iterator, and not <a
   * href="http://www.boost.org/libs/iterator/doc/filter_iterator.html"
   * >boost::filter_iterator</a>.
   * See the doc on trsl::persistent_filter_iterator for a description of
   * the difference between these.
   *
   * Systematic sampling may perform very badly if the population
   * sequence follows a pattern.  If a pattern is likely to occur in
   * the population, the user may want to pipe the sampling iterator
   * after a trsl::random_permutation_iterator.
   *
   * @param ElementType Type of the elements in the original
   * population.  Just put the type here, e.g. MyClass; const and
   * reference modifiers are handled internally.
   *
   * @param WeightType Type of the element weight. Defaults to double.
   *
   * @param WeightAccessorType Type of an accessor that will allow to
   * extract the weight from an element. You would probably use the
   * default mp_weight_accessor<>, or
   * <tt>std::pointer_to_unary_function<const MyClass&,
   * double></tt>. Maybe pointer_to_unary_function should be the
   * default accessor?
   *
   * <b>References:</b>
   *
   * - [1] R. Douc, O. Cappe, and E. Moulines. Comparison of
   * resampling schemes for particle filtering. International
   * Symposium on Parallel and Distributed Processing and
   * Applications, 2005:64, 2005.
   *
   * - [2] J. Hol, T. Sch&ouml;n, and F. Gustafsson. On resampling
   * algorithms for particle filters. In Nonlinear Statistical Signal
   * Processing Workshop, 2006.
   */
  template<
    typename ElementType,
    typename WeightType = double,
    typename WeightAccessorType = mp_weight_accessor<WeightType, ElementType>
  > class is_picked_systematic
  {
  public:
    typedef ElementType element_type;
    typedef WeightType weight_type;
    typedef WeightAccessorType weight_accessor_type;
    
    /**
     * @brief Construction with user-provided random number.
     *
     * @param sampleSize Number of elements to pick out of
     * the population.
     *
     * @param populationWeight Total weight of the
     * population. Generally equal to 1.
     *
     * @param uniform01 Random number in <tt>[0,1[</tt>. This argument
     * is provided to allow the use a fancy random number
     * generator, such as boost or GSL.
     *
     * @param wac Weight accessor. Defaults to
     * mp_weight_accessor. Note that if you don't pass a
     * mp_weight_accessor, the default constructor for
     * mp_weight_accessor will make it always return 1, ignoring
     * element weights. See @ref accessor for accessor details.
     */
    is_picked_systematic(size_t sampleSize,
                         WeightType populationWeight,
                         WeightType uniform01,
                         WeightAccessorType const& wac = WeightAccessorType()) :
      wac_(wac), sampleSize_(sampleSize),
      populationWeight_(populationWeight), step_(populationWeight_ / sampleSize_)
      {
        initialize(uniform01*step_);
      }

    /**
     * @brief Construction with system-provided random number.
     *
     * @param sampleSize Number of elements to pick out of
     * the population.
     *
     * @param populationWeight Total weight of the
     * population. Generally equal to 1.
     *
     * @param wac Weight accessor. Defaults to
     * mp_weight_accessor. Note that if you don't pass a
     * mp_weight_accessor, the default constructor for
     * mp_weight_accessor will make it always return 1, ignoring
     * element weights. See @ref accessor for accessor details.
     *
     * Construction needs a random number in <tt>[0,1[</tt>. This
     * constructor uses the system function <tt>random</tt> for that
     * purpose.
     */
    is_picked_systematic(size_t sampleSize,
                         WeightType populationWeight,
                         WeightAccessorType const& wac = WeightAccessorType()) :
      wac_(wac), sampleSize_(sampleSize),
      populationWeight_(populationWeight), step_(populationWeight_ / sampleSize_)
      {
        initialize( (random() / (RAND_MAX+WeightType(1.0))) * step_);
      }

    
    /**
     * @brief Decides whether this element should be picked or not.
     *
     * Part of the requirements for persistent_filter_iterator functors.
     */
    bool operator()(const ElementType & e)
      {
#ifdef TRSL_USE_INTUITIVE_ALGORITHM
        // This algorithm is the intuitive implementation of
        // systematic sampling, where one pictures a wheel with
        // N_SAMPLE spokes and distributes the population around the
        // wheel as segments of tire of lenght proportional to their
        // weight; the spokes point to picked elements.
        WeightType arrow = k_*step_;
    
        if (cumulative_ <= arrow && arrow < cumulative_ + wac_(e))
        {
          k_++;
          return true;
        }
        cumulative_ += wac_(e);

        return false;
#else
        // This algorithm is a massaged version of the intuitive
        // algorithm.  Both algorithms are conceptually identical, but
        // this version is faster in practice.
        if (position_ < wac_(e))
        {
          position_ += step_;
          return true;
        }
        position_ -= wac_(e);
    
        return false;
#endif
      }

  private:
    void initialize(WeightType randomWeight)
      {
        assert(step_ <= populationWeight_);
  
#ifdef TRSL_USE_INTUITIVE_ALGORITHM
        cumulative_ = -randomWeight;
        k_ = 0;
#else
        position_ = randomWeight;
#endif
      }

  private:
    WeightAccessorType wac_;
    size_t sampleSize_;
    WeightType populationWeight_;
    WeightType step_;
  
#ifdef TRSL_USE_INTUITIVE_ALGORITHM
    WeightType cumulative_;
    size_t k_;
#else
    WeightType position_;
#endif
  };

}

#endif // include guard
