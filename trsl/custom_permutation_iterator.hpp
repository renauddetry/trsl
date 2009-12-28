// (C) Copyright Renaud Detry   2007-2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_CUSTOM_PERMUTATION_ITERATOR_HPP
#define TRSL_CUSTOM_PERMUTATION_ITERATOR_HPP

#include <trsl/reorder_iterator.hpp>
#include <trsl/common.hpp>
#include <trsl/error_handling.hpp>

namespace trsl
{

  template
  <
    class ElementIterator,
    class OrderTag = typename default_order_tag<ElementIterator>::type
  >
  class custom_permutation_iterator :
    public reorder_iterator
    <
      ElementIterator,
      OrderTag,
      custom_permutation_iterator<ElementIterator, OrderTag>
    >
  {
    typedef reorder_iterator
    <
      ElementIterator,
      OrderTag,
      custom_permutation_iterator<ElementIterator, OrderTag>
    >
    super_t;
    
  public:
    
    typedef ElementIterator element_iterator;
    typedef OrderTag order_tag;
    typedef typename super_t::index_t index_t;
    typedef typename super_t::position_t position_t;

    typedef typename super_t::position_container position_container;
    typedef typename super_t::position_container_ptr position_container_ptr;    

    custom_permutation_iterator() {}
    
    /**
     * @brief Constructs a reorder_iterator that will iterate through a
     * random subset of size @p permutationSize of a random permutation
     * of the population referenced by @p first and @p last.
     *
     * The @p permutationSize should be smaller or equal to the
     * size of the population. If it is not the case, a bad_parameter_value
     * is thrown.
     *
     * Performing a random permutation requires a series of random
     * integers, these are provided by rand_gen::uniform_int; see @ref
     * random for further details.
     *
     * Creating such a reorder_iterator and iterating through it is
     * generally much faster than re-ordering the population itself (or
     * a copy thereof), especially when elements are large, have a
     * complex copy-constructor, or a tall class hierarchy.
     */
    custom_permutation_iterator(ElementIterator first,
                                const position_container_ptr& index_collection) :
      super_t(first,
              index_collection) {}

    template<class OtherElementIterator>
    custom_permutation_iterator
    (custom_permutation_iterator<OtherElementIterator, OrderTag> const& r,
     typename boost::enable_if_convertible<OtherElementIterator, ElementIterator>::type* = 0) :
    super_t(r) {}    
  };
  
  template
  <
    class ElementIterator,
    class OrderTag
  >
  custom_permutation_iterator<ElementIterator, OrderTag>
  make_custom_permutation_iterator
  (ElementIterator first,
   const typename reorder_iterator
   <
     ElementIterator,
     OrderTag,
     custom_permutation_iterator<ElementIterator, OrderTag>
   >::position_container_ptr & index_collection,
   OrderTag orderTag = typename default_order_tag<ElementIterator>::type())
  {
    return custom_permutation_iterator<ElementIterator, OrderTag>
    (first, index_collection);
  }
  
} // namespace trsl

#endif // include guard
