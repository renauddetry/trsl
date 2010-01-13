// (C) Copyright Renaud Detry   2007-2010.
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

  /**
   * @brief Provides an iterator over a custom permutation of a range.
   *
   * This class inherits from reorder_iterator. It adds a constructor
   * which takes as arguments (1) an iterator pointing to an input
   * range and (2) an array of <i>positions</i> (see reorder_iterator)
   * defining a permutation of the input range. The elements returned
   * by custom_permutation_iterator are ordered as defined by the
   * array of positions. See reorder_iterator for more details on how
   * this iterator works.
   *
   * Template type parameters @p ElementIterator and @p OrderTag are
   * described in reorder_iterator.
   *
   * Helper functions: trsl::make_custom_permutation_iterator.
   */
  template
  <
    class ElementIterator,
    class OrderTag = typename default_order_tag<ElementIterator>::type
  >
  class custom_permutation_iterator :
    public detail::reorder_iterator
    <
      ElementIterator,
      OrderTag,
      custom_permutation_iterator<ElementIterator, OrderTag>
    >
  {
    typedef detail::reorder_iterator
    <
      ElementIterator,
      OrderTag,
      custom_permutation_iterator<ElementIterator, OrderTag>
    >
    super_t;
        
  public:
    
    typedef custom_permutation_iterator<ElementIterator, OrderTag> this_t;
    typedef ElementIterator element_iterator;
    typedef OrderTag order_tag;
    typedef typename super_t::index_t index_t;
    typedef typename super_t::position_t position_t;

    typedef typename super_t::position_container position_container;
    typedef typename super_t::position_container_ptr position_container_ptr;    

    custom_permutation_iterator() {}
    
    /**
     * @brief Constructs a custom_permutation_iterator that will
     * iterate through a permutation of the range referenced by @p
     * first, following the order defined by @p position_collection.
     *
     * @param first Iterator pointing to the first element of the
     * input range. It must be of type @p ElementIterator.
     *
     * @param position_collection Collection of <i>positions</i>. This
     * is currently a  <tt>boost::shared_ptr< std::vector< position_t >
     * ></tt>, where @p position_t depends on @p OrderTag.  If @p OrderTag is
     * @p iterator_order_tag, then @p position_t is @p ElementIterator. If
     * @p OrderTag is @p index_order_tag, then @p position_t is @p size_t. By
     * default, @p OrderTag is @p index_order_tag if and only if
     * @p ElementIterator models Random Access Iterator.
     */
    custom_permutation_iterator(ElementIterator first,
                                const position_container_ptr& position_collection) :
      super_t(first,
              position_collection) {}

    template<class OtherElementIterator>
    custom_permutation_iterator
    (custom_permutation_iterator<OtherElementIterator, OrderTag> const& r,
     typename boost::enable_if_convertible<OtherElementIterator, ElementIterator>::type* = 0) :
    super_t(r) {}    

    /**
     * @brief @copybrief detail::reorder_iterator::src_index()
     *
     * @copydetails detail::reorder_iterator::src_index()
     */
    this_t begin() const
    {
      return super_t::begin();
    }
    
    /**
     * @brief @copybrief detail::reorder_iterator::src_index()
     *
     * @copydetails detail::reorder_iterator::src_index()
     */
    this_t end() const
    {
      return super_t::end();
    }
    
    /**
     * @brief @copybrief detail::reorder_iterator::src_index()
     *
     * @copydetails detail::reorder_iterator::src_index()
     */
    index_t src_index() const
    {
      return super_t::src_index();
    }
    
    /**
     * @brief @copybrief detail::reorder_iterator::src_index()
     *
     * @copydetails detail::reorder_iterator::src_index()
     */
    ElementIterator src_iterator() const
    {
      return super_t::src_iterator();
    }
  };
  
  /**
   * @brief Helper function for creating a custom_permutation_iterator.
   *
   * See @p trsl::custom_permutation_iterator for a description
   * of @p first and @p position_collection arguments.
   *
   * Iterators created with this function have default order type (see
   * OrderType in reorder_iterator). If one wishes to select a
   * non-default order type, custom_permutation_iterator must be used
   * explicitly.
   */
  template
  <
    class ElementIterator
  >
  custom_permutation_iterator<ElementIterator>
  make_custom_permutation_iterator
  (ElementIterator first,
   const typename custom_permutation_iterator<ElementIterator>::position_container_ptr & position_collection)
  {
    return custom_permutation_iterator<ElementIterator>
    (first, position_collection);
  }
  
} // namespace trsl

#endif // include guard
