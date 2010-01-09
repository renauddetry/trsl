// (C) Copyright Renaud Detry   2007-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_RANDOM_PERMUTATION_ITERATOR_HPP
#define TRSL_RANDOM_PERMUTATION_ITERATOR_HPP

#include <trsl/reorder_iterator.hpp>
#include <trsl/common.hpp>
#include <trsl/error_handling.hpp>

namespace trsl
{

  /**
   * @brief Provides an iterator over a random permutation of a range.
   *
   * This class inherits from reorder_iterator. It adds constructors
   * which compute a random permutation of an input range.
   *
   * Template type parameters @p ElementIterator and @p OrderTag are
   * described in reorder_iterator.
   *
   * random_permutation_iterator inherits methods from
   * reorder_iterator (begin(), end(), src_index(), src_iterator()).
   * See reorder_iterator for a description of these methods.
   *
   * Helper functions: trsl::make_random_permutation_iterator.
   */
  template
  <
    class ElementIterator,
    class OrderTag = typename default_order_tag<ElementIterator>::type
  >
  class random_permutation_iterator :
    public reorder_iterator
    <
      ElementIterator,
      OrderTag,
      random_permutation_iterator<ElementIterator, OrderTag>
    >
  {
    typedef reorder_iterator
    <
      ElementIterator,
      OrderTag,
      random_permutation_iterator<ElementIterator, OrderTag>
    >
    super_t;
    
    typedef typename super_t::position_container position_container;
    typedef typename super_t::position_container_ptr position_container_ptr;
  
  public:

    typedef ElementIterator element_iterator;
    typedef OrderTag order_tag;
    typedef typename super_t::index_t index_t;
    typedef typename super_t::position_t position_t;
    
    random_permutation_iterator() {}
    
    /**
     * @brief Constructs an iterator that will iterate through a
     * random subset of size @p permutationSize of a random permutation
     * of the population referenced by @p first and @p last.
     *
     * The @p permutationSize should be smaller or equal to the size
     * of the population. It can also be set to trsl::same_size, which
     * will shuffle <i>all</i> elements between @p first and @p last. If @p
     * permutationSize is neither an integer smaller or equal to the
     * size of the input range, nor trsl::same_size, a
     * bad_parameter_value is thrown.
     *
     * Performing a random permutation requires a series of random
     * integers. With this constructor, these are provided by rand_gen::uniform_int; see @ref
     * random for further details.
     */
    random_permutation_iterator(ElementIterator first,
                                ElementIterator last,
                                boost::optional<unsigned> permutationSize = same_size) :
      super_t(first,
              new_position_container(first, last,
                                     permutationSize,
                                     rand_gen::uniform_int)) {}

    /**
     * @brief Constructs an iterator that will iterate through a
     * random subset of size @p permutationSize of a random permutation
     * of the population referenced by @p first and @p last.
     *
     * The @p permutationSize should be smaller or equal to the size
     * of the population. It can also be set to trsl::same_size, which
     * will shuffle <i>all</i> elements between @p first and @p last. If @p
     * permutationSize is neither an integer smaller or equal to the
     * size of the input range, nor trsl::same_size, a
     * bad_parameter_value is thrown.
     *
     * Performing a random permutation requires a series of random
     * integers. These are provided by @p rng, which must be such that rng(N) is
     * a valid expression that returns a random number in <tt>[0, N-1]</tt>.
     */
    template<class RandomNumberGenerator>
    random_permutation_iterator(ElementIterator first,
                                ElementIterator last,
                                boost::optional<unsigned> permutationSize,
                                RandomNumberGenerator& rng) :
      super_t(first,
              new_position_container(first, last, permutationSize, rng)) {}
    
    template<class OtherElementIterator>
    random_permutation_iterator
    (random_permutation_iterator<OtherElementIterator, OrderTag> const& r,
     typename boost::enable_if_convertible<OtherElementIterator, ElementIterator>::type* = 0) :
    super_t(r) {}
    
  protected:
    template<class RandomNumberGenerator>
    static position_container_ptr
    new_position_container(ElementIterator first,
                           ElementIterator last,
                           boost::optional<unsigned> permutationSize,
                           RandomNumberGenerator& rng)
    {            
      position_container_ptr position_collection(new position_container);
      
      detail::fill_index_container<ElementIterator, position_container, position_t>
      (*position_collection, first, last, order_tag());

      if (permutationSize)
      {
        size_t size = position_collection->size();
        if (*permutationSize > size)
          throw bad_parameter_value("random_permutation_iterator: "
                                    "parameter permutationSize out of range.");
        detail::partial_random_shuffle(position_collection->begin(),
                                       position_collection->begin()+*permutationSize,
                                       position_collection->end(),
                                       rng);
        position_collection->resize(*permutationSize);
      }
      else
      {
        std::random_shuffle(position_collection->begin(),
                            position_collection->end(),
                            rng);
      }

      return position_collection;
    }
  };
  
  /**
   * @brief Helper function for creating a random_permutation_iterator.
   *
   * See @p trsl::random_permutation_iterator for a description
   * of arguments.
   *
   * Iterators created with this function have default order type (see
   * OrderType in reorder_iterator). If one wishes to select a
   * non-default order type, random_permutation_iterator must be used
   * explicitly.
   */
  template<class ElementIterator>
  random_permutation_iterator<ElementIterator>
  make_random_permutation_iterator
  (ElementIterator first,
   ElementIterator last,
   boost::optional<unsigned> permutationSize = same_size)
  {
    return random_permutation_iterator<ElementIterator>
    (first, last, permutationSize);
  }

  /**
   * @brief Helper function for creating a random_permutation_iterator.
   *
   * See @p trsl::random_permutation_iterator for a description
   * of arguments.
   *
   * Iterators created with this function have default order type (see
   * OrderType in reorder_iterator). If one wishes to select a
   * non-default order type, random_permutation_iterator must be used
   * explicitly.
   */
  template<class ElementIterator, class RandomNumberGenerator>
  random_permutation_iterator<ElementIterator>
  make_random_permutation_iterator
  (ElementIterator first,
   ElementIterator last,
   boost::optional<unsigned> permutationSize,
   RandomNumberGenerator& rng)
  {
    return random_permutation_iterator<ElementIterator>
    (first, last, permutationSize, rng);
  }
  
} // namespace trsl

#endif // include guard
