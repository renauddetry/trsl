// (C) Copyright Renaud Detry   2007.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_IS_PICKED_SYSTEMATIC_HPP
#define TRSL_IS_PICKED_SYSTEMATIC_HPP

#include "trsl/common.hpp"
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
   * @brief Functor to use with persistent_filter_iterator for
   * systematic sampling of a range.
   *
   * The sampling method is systematic sampling, see [1, 2].
   *
   * This class is intended to be used as a predicate functor to
   * trsl::persistent_filter_iterator to form a <em>sample
   * iterator</em>. The sample iterator accesses a population of
   * elements through a range defined by a pair of Forward Iterators
   * (begin/end), and provides on-the-fly iteration through a sample
   * of the population.
   *
   * Systematic sampling may perform very badly if the population
   * sequence follows a pattern.  If a pattern is likely to occur in
   * the population, the user may want to pipe the sample iterator
   * after a trsl::random_permutation_iterator.
   *
   * @param ElementType Type of the elements in the 
   * population. Constness and
   * reference modifiers are handled internally; this parameter should be
   * a bare type, e.g. <tt>Particle</tt> and <em>not</em> <tt>const Particle&</tt>.
   *
   * @param WeightType Element weight type, should be a floating point type.
   * Defaults to <tt>double</tt>.
   *
   * @param WeightAccessorType Type of the accessor that will allow to
   * extract weights from elements. Defaults to <tt>mp_weight_accessor</tt>,
   * see @ref accessor for further details on accessors.
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
     * @brief Construction with system-provided random number.
     *
     * The systematic sampling predicate initialization needs a random
     * number in <tt>[0,1[</tt>.  This constructor uses
     * trsl::random::uniform_01 to generate that number.  See @ref
     * random for more details.
     *
     * @param sampleSize Number of elements in the sample, within <tt>[0, infinity[</tt>.
     *
     * @param populationWeight Total weight of the
     * population. Generally equal to 1.
     *
     * @param wac Weight accessor. Defaults to
     * mp_weight_accessor (). Note that if you don't pass explicitly a
     * mp_weight_accessor(<tt>&ElementType::GETWEIGHTMETHOD</tt>), the
     * default constructor for mp_weight_accessor will set it to
     * always return 1, ignoring element weights. See @ref accessor
     * for accessor details.
     */
    is_picked_systematic(size_t sampleSize,
                         WeightType populationWeight,
                         WeightAccessorType const& wac = WeightAccessorType()) :
      wac_(wac), sampleSize_(sampleSize),
      populationWeight_(populationWeight), step_(populationWeight_ / sampleSize_)
      {
        initialize( rand_gen::uniform_01<WeightType>() * step_);
      }

    /**
     * @brief Construction with user-provided random number.
     *
     * The systematic sampling predicate initialization needs a random
     * number in <tt>[0,1[</tt>.  This constructor allows the user to
     * provide that number directly; the user is free to choose a
     * fancy random number generator, such as the <a
     * href="http://www.boost.org/libs/random/index.html" >Boost
     * Random Number Library</a> or <a
     * href="http://www.gnu.org/software/gsl/manual/html_node/Random-Number-Generation.html"
     * >GSL</a>.
     *
     * @param sampleSize Number of elements in the sample, within <tt>[0, infinity[</tt>.
     *
     * @param populationWeight Total weight of the
     * population. Generally equal to 1.
     *
     * @param uniform01 Random number in <tt>[0,1[</tt>.
     *
     * @param wac Weight accessor. Defaults to
     * mp_weight_accessor (). Note that if you don't pass explicitly a
     * mp_weight_accessor(<tt>&ElementType::GETWEIGHTMETHOD</tt>), the
     * default constructor for mp_weight_accessor will set it to
     * always return 1, ignoring element weights. See @ref accessor
     * for accessor details.
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
     * @brief Decides whether <tt>e</tt> should be picked or not (used by persistent_filter_iterator).
     *
     * Part of the requirements for persistent_filter_iterator predicates.
     */
    bool operator()(const ElementType & e)
      {
#ifdef TRSL_USE_INTUITIVE_ALGORITHM
        // This algorithm is the intuitive implementation of
        // systematic sampling, where one pictures a wheel with
        // N_SAMPLE spokes and distributes the population around the
        // wheel as segments of tire of length proportional to their
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
