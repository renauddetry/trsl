// (C) Copyright Renaud Detry   2007-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_SORT_ITERATOR_HPP
#define TRSL_SORT_ITERATOR_HPP

#include <trsl/reorder_iterator.hpp>
#include <trsl/common.hpp>
#include <trsl/error_handling.hpp>
#include <boost/optional.hpp>
#include <functional>

namespace trsl
{

  namespace detail {
  
    /** @internal @brief Used internally */
    template
    <
      class RandomIterator,
      class Comparator
    >
    class at_index_comp
    {
    public:
      
      at_index_comp(const RandomIterator &first, const Comparator &comp) :
        elements_(first), comp_(comp)
        {}
      
      bool operator() (unsigned i, unsigned j)
        {
          return comp_(*(elements_+i), *(elements_+j));
        }
      
      RandomIterator elements_;
      Comparator comp_;
    };
  
  }

  /**
   * @brief Provides an iterator over a sorted permutation of a range.
   *
   * This class inherits from reorder_iterator. It adds constructors
   * which compute a sorted permutation of an input range.
   *
   * Template type parameters @p ElementIterator and @p OrderTag are
   * described in reorder_iterator.
   *
   * sort_iterator inherits methods from
   * reorder_iterator (begin(), end(), src_index(), src_iterator()).
   * See reorder_iterator for a description of these methods.
   *
   * Helper functions: trsl::make_sort_iterator.
   */
  template
  <
    class ElementIterator,
    class OrderTag = typename default_order_tag<ElementIterator>::type
  >
  class sort_iterator :
    public reorder_iterator
    <
      ElementIterator,
      OrderTag,
      sort_iterator<ElementIterator, OrderTag>
    >
  {
    typedef 
    reorder_iterator
    <
      ElementIterator,
      OrderTag,
      sort_iterator<ElementIterator, OrderTag>
    >
    super_t;
    
    typedef typename super_t::position_container position_container;
    typedef typename super_t::position_container_ptr position_container_ptr;
    
  public:
    
    typedef ElementIterator element_iterator;
    typedef OrderTag order_tag;
    typedef typename super_t::index_t index_t;
    typedef typename super_t::position_t position_t;
    
    typedef typename std::iterator_traits<ElementIterator>::value_type element_t;
    
    sort_iterator() {}
    
    /**
     * @brief Constructs an iterator that will iterate through a
     * sorted (increasing order) permutation of the population
     * referenced by @p first and @p last.
     *
     * The @p permutationSize should be smaller or equal to the size
     * of the population. It can also be set to trsl::same_size, which
     * will sort <i>all</i> elements between @p first and @p last. If @p
     * permutationSize is neither an integer smaller or equal to the
     * size of the input range, nor trsl::same_size, a
     * bad_parameter_value is thrown.
     */
    sort_iterator(ElementIterator first,
                  ElementIterator last,
                  boost::optional<unsigned> permutationSize = same_size) :
      super_t(first,
              new_position_container(first, last,
                                     permutationSize,
                                     std::less<element_t>())) {}

    /**
     * @brief Constructs an iterator that will iterate through a
     * sorted (custom order) permutation of the population
     * referenced by @p first and @p last.
     *
     * The @p permutationSize should be smaller or equal to the size
     * of the population. It can also be set to trsl::same_size, which
     * will sort <i>all</i> elements between @p first and @p last. If @p
     * permutationSize is neither an integer smaller or equal to the
     * size of the input range, nor trsl::same_size, a
     * bad_parameter_value is thrown.
     *
     * A comparator is provided through @p comp. <tt>Comparator</tt>
     * has to model <a
     * href="http://www.sgi.com/tech/stl/StrictWeakOrdering.html"
     * >Strict Weak Ordering</a>.  In particular <a
     * href="http://www.sgi.com/tech/stl/less.html"
     * ><tt>std::less<ElementType>()</tt></a> and <a
     * href="http://www.sgi.com/tech/stl/greater.html"
     * ><tt>std::greater<ElementType>()</tt></a> will work, whereas <a
     * href="http://www.sgi.com/tech/stl/less_equal.html"
     * ><tt>std::less_equal<ElementType>()</tt></a> and <a
     * href="http://www.sgi.com/tech/stl/greater_equal.html"
     * ><tt>std::greater_equal<ElementType>()</tt></a> will
     * <em>not</em>.
     */
    template<class ElementComparator>
    sort_iterator(ElementIterator first,
                  ElementIterator last,
                  boost::optional<unsigned> permutationSize,
                  ElementComparator comp) :
      super_t(first,
              new_position_container(first, last, permutationSize, comp)) {}
    
    template<class OtherElementIterator>
    sort_iterator
    (sort_iterator<OtherElementIterator, OrderTag> const& r,
     typename boost::enable_if_convertible<OtherElementIterator, ElementIterator>::type* = 0) :
    super_t(r) {}

  protected:
    template<class ElementComparator>
    static position_container_ptr
    new_position_container(ElementIterator first,
                           ElementIterator last,
                           boost::optional<unsigned> permutationSize,
                           ElementComparator comp)
      {            
        position_container_ptr position_collection(new position_container);
      
        detail::fill_index_container
        <ElementIterator, position_container, position_t>
          (*position_collection, first, last, order_tag());
            
        if (permutationSize)
        {
          size_t size = position_collection->size();
          if (*permutationSize > size)
            throw bad_parameter_value("sort_iterator: "
                                      "parameter permutationSize out of range.");
          std::partial_sort(position_collection->begin(),
                            position_collection->begin()+*permutationSize,
                            position_collection->end(),
                            detail::at_index_comp
                            <ElementIterator, ElementComparator>(first, comp));
          position_collection->resize(*permutationSize);
        }
        else
        {
          std::sort(position_collection->begin(),
                    position_collection->end(),
                    detail::at_index_comp
                    <ElementIterator, ElementComparator>(first, comp));
        }
      
        return position_collection;
      }
  };

  /**
   * @brief Helper function for creating a sort_iterator.
   *
   * See @p trsl::sort_iterator for a description
   * of arguments.
   *
   * Iterators created with this function have default order type (see
   * OrderType in reorder_iterator). If one wishes to select a
   * non-default order type, sort_iterator must be used
   * explicitly.
   */
  template<class ElementIterator>
  sort_iterator<ElementIterator>
  make_sort_iterator
  (ElementIterator first,
   ElementIterator last,
   boost::optional<unsigned> permutationSize)
  {
    return sort_iterator<ElementIterator>
    (first, last, permutationSize);
  }

  /**
   * @brief Helper function for creating a sort_iterator.
   *
   * See @p trsl::sort_iterator for a description
   * of arguments.
   *
   * Iterators created with this function have default order type (see
   * OrderType in reorder_iterator). If one wishes to select a
   * non-default order type, sort_iterator must be used
   * explicitly.
   */
  template<class ElementIterator, class ElementComparator>
  sort_iterator<ElementIterator>
  make_sort_iterator
  (ElementIterator first,
   ElementIterator last,
   boost::optional<unsigned> permutationSize,
   ElementComparator comp)
  {
    return sort_iterator<ElementIterator>
    (first, last, permutationSize, comp);
  }
  
} // namespace trsl

#endif // include guard
