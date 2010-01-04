// (C) Copyright Renaud Detry   2008-2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_PPFILTER_ITERATOR_HPP
#define TRSL_PPFILTER_ITERATOR_HPP

#include <trsl/error_handling.hpp>
#include <trsl/common.hpp>

#include <trsl/ppfilter_iterator_prototype.hpp>

namespace trsl
{

  /**
   * @brief Random permutation, persistent filter iterator.
   *
   * This class pipes a random_permutation_iterator (upstream) with
   * a persistent_filter_iterator (downstream).  It is intended to be
   * used with is_picked_systematic. Systematic sampling of a random
   * permutation achieves <em>probability sampling</em>.
   *
   * @p ElementIterator should model <em>Random Access Iterator</em>.
   */
  template<class Predicate, class ElementIterator>
  class ppfilter_iterator :
    public ppfilter_iterator_prototype
    <Predicate, ElementIterator, ppfilter_iterator<Predicate, ElementIterator> >
  {
    typedef ppfilter_iterator_prototype
    <Predicate, ElementIterator, ppfilter_iterator<Predicate, ElementIterator> >
    base_t;
    
    typedef typename base_t::upstream_iterator upstream_iterator;
    typedef typename base_t::downstream_iterator downstream_iterator;

  public:
    
    typedef typename base_t::element_iterator element_iterator;

    ppfilter_iterator() {}
      
    /**
     * @brief Constructor.
     */
    explicit ppfilter_iterator(Predicate f,
                               ElementIterator first, ElementIterator last) :
      base_t(f, first, last) {}

    /**
     * @brief Constructor.
     */
    template<class RandomNumberGenerator>
    explicit ppfilter_iterator(Predicate f,
                               ElementIterator first, ElementIterator last,
                               RandomNumberGenerator& rng) :
      base_t(f, first, last, rng) {}
    
    /**
     * @brief Allows conversion from a ppfilter_iterator to a const
     * ppfilter_iterator, won't allow conversion from a const
     * ppfilter_iterator to a ppfilter_iterator.
     *
     * By &ldquo;const ppfilter_iterator&rdquo;, we mean that the @p
     * ElementIterator is const, e.g.
     * <tt>std::vector<Particle>::const_iterator</tt>.
     */
    template<class OtherElementIterator>
    ppfilter_iterator
    (ppfilter_iterator<Predicate, OtherElementIterator> const& r,
     typename boost::enable_if_convertible
     <
       OtherElementIterator,
       ElementIterator
     >::type* = 0) :
      base_t(r) {}

  };
  
} // namespace trsl

#endif // include guard
