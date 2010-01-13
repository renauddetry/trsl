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
   * range and (2) an array of @em positions (see reorder_iterator)
   * defining a permutation of the input range. The elements returned
   * by custom_reord_iter are ordered as defined by the
   * array of positions. See reorder_iterator for more details on how
   * this iterator works.
   *
   * Template type parameters @p ElementIterator and @p OrderTag are
   * described in reorder_iterator.
   *
   * Helper functions: trsl::make_custom_reord_iter.
   */
  template
  <
    class ElementIterator,
    class OrderTag = typename default_order_tag<ElementIterator>::type
  >
  class custom_reord_iter :
    public detail::reorder_iterator
    <
      ElementIterator,
      OrderTag,
      custom_reord_iter<ElementIterator, OrderTag>
    >
  {
    typedef detail::reorder_iterator
    <
      ElementIterator,
      OrderTag,
      custom_reord_iter<ElementIterator, OrderTag>
    >
    super_t;
        
  public:
    
    typedef custom_reord_iter<ElementIterator, OrderTag> this_t;
    typedef ElementIterator element_iterator;
    typedef OrderTag order_tag;
    typedef typename super_t::index_t index_t;
    typedef typename super_t::position_t position_t;

    typedef typename super_t::position_container position_container;
    typedef typename super_t::position_container_ptr position_container_ptr;    

    custom_reord_iter() {}
    
    /**
     * @brief Constructs a custom_reord_iter that will
     * iterate through a permutation of the range referenced by @p
     * first, following the order defined by @p position_collection.
     *
     * @param first Iterator pointing to the first element of the
     * input range. It must be of type @p ElementIterator.
     *
     * @param position_collection Collection of @em positions. This
     * is currently a  <tt>boost::shared_ptr< std::vector< position_t >
     * ></tt>, where @p position_t depends on @p OrderTag.  If @p OrderTag is
     * @p iterator_order_tag, then @p position_t is @p ElementIterator. If
     * @p OrderTag is @p index_order_tag, then @p position_t is @p size_t. By
     * default, @p OrderTag is @p index_order_tag if and only if
     * @p ElementIterator models Random Access Iterator.
     */
    custom_reord_iter(ElementIterator first,
                      const position_container_ptr& position_collection) :
      super_t(first,
              position_collection) {}

    template<class OtherElementIterator>
    custom_reord_iter
    (custom_reord_iter<OtherElementIterator, OrderTag> const& r,
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
   * @brief Helper function for creating a custom_reord_iter.
   *
   * See @p trsl::custom_reord_iter for a description
   * of @p first and @p position_collection arguments.
   *
   * Iterators created with this function have default order type (see
   * OrderType in reorder_iterator). If one wishes to select a
   * non-default order type, custom_reord_iter must be used
   * explicitly.
   */
  template
  <
    class ElementIterator
  >
  custom_reord_iter<ElementIterator>
  make_custom_reord_iter
  (ElementIterator first,
   const typename custom_reord_iter<ElementIterator>::position_container_ptr & position_collection)
  {
    return custom_reord_iter<ElementIterator>
    (first, position_collection);
  }
  
} // namespace trsl

#endif // include guard