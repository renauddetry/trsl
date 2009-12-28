// (C) Copyright Renaud Detry   2007-2009.
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
    random_permutation_iterator(ElementIterator first,
                                ElementIterator last,
                                boost::optional<unsigned> permutationSize = same_size) :
      super_t(first,
              new_position_container(first, last,
                                     permutationSize,
                                     rand_gen::uniform_int)) {}

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
      position_container_ptr index_collection(new position_container);
      
      detail::fill_index_container<ElementIterator, position_container, position_t>
      (*index_collection, first, last, order_tag());

      if (permutationSize)
      {
        size_t size = index_collection->size();
        if (*permutationSize > size)
          throw bad_parameter_value("random_permutation_iterator: "
                                    "parameter permutationSize out of range.");
        detail::partial_random_shuffle(index_collection->begin(),
                                       index_collection->begin()+*permutationSize,
                                       index_collection->end(),
                                       rng);
        index_collection->resize(*permutationSize);
      }
      else
      {
        std::random_shuffle(index_collection->begin(),
                            index_collection->end(),
                            rng);
      }

      return index_collection;
    }
  };
  
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
