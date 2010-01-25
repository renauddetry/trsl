// (C) Copyright Renaud Detry   2007-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_RANDOM_PERMUTATION_ITERATOR_HPP
#define TRSL_RANDOM_PERMUTATION_ITERATOR_HPP

#include <trsl/prototype_reord_iter.hpp>
#include <trsl/common.hpp>
#include <trsl/error_handling.hpp>

namespace trsl
{
  /**  @addtogroup products_reorder */
  //@{
  
  /**
   * @brief Provides an iterator over a random permutation of a range.
   *
   * This class inherits from prototype_reord_iter. It adds constructors
   * which compute a random permutation of an input range.
   *
   * Template type parameters @p ElementIterator and @p OrderTag are
   * described in prototype_reord_iter.
   *
   * Helper functions: trsl::make_random_reord_iter.
   */
  template
  <
    class ElementIterator,
    class OrderTag = typename default_order_tag<ElementIterator>::type
  >
  class random_reord_iter :
    public detail::prototype_reord_iter
    <
      ElementIterator,
      OrderTag,
      random_reord_iter<ElementIterator, OrderTag>
    >
  {
    typedef detail::prototype_reord_iter
    <
      ElementIterator,
      OrderTag,
      random_reord_iter<ElementIterator, OrderTag>
    >
    super_t;
    
    typedef typename super_t::position_container position_container;
    typedef typename super_t::position_container_ptr position_container_ptr;
  
  public:

    typedef random_reord_iter<ElementIterator, OrderTag> this_t;
    typedef ElementIterator element_iterator;
    typedef OrderTag order_tag;
    typedef typename super_t::index_t index_t;
    typedef typename super_t::position_t position_t;
    
    random_reord_iter() {}
    
    /**
     * @brief Constructs an iterator that will iterate through a
     * random subset of size @p permutationSize of a random permutation
     * of the population referenced by @p first and @p last.
     *
     * The @p permutationSize should be smaller or equal to the size
     * of the population. It can also be set to trsl::same_size, which
     * will shuffle @em all elements between @p first and @p last. If @p
     * permutationSize is neither an integer smaller or equal to the
     * size of the input range, nor trsl::same_size, a
     * bad_parameter_value is thrown.
     *
     * Performing a random permutation requires a series of random
     * integers. With this constructor, these are provided by rand_gen::uniform_int; see @ref
     * random for further details.
     */
    random_reord_iter(ElementIterator first,
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
     * will shuffle @em all elements between @p first and @p last. If @p
     * permutationSize is neither an integer smaller or equal to the
     * size of the input range, nor trsl::same_size, a
     * bad_parameter_value is thrown.
     *
     * Performing a random permutation requires a series of random
     * integers. These are provided by @p rng, which must be such that rng(N) is
     * a valid expression that returns a random number in <tt>[0, N-1]</tt>.
     */
    template<class RandomNumberGenerator>
    random_reord_iter(ElementIterator first,
                                ElementIterator last,
                                boost::optional<unsigned> permutationSize,
                                RandomNumberGenerator& rng) :
      super_t(first,
              new_position_container(first, last, permutationSize, rng)) {}
    
    template<class OtherElementIterator>
    random_reord_iter
    (random_reord_iter<OtherElementIterator, OrderTag> const& r,
     typename boost::enable_if_convertible<OtherElementIterator, ElementIterator>::type* = 0) :
    super_t(r) {}
    
    /**
     * @brief Returns an iterator pointing to the begining of the
     * reordered range.
     */
    this_t begin() const
    {
      return super_t::begin();
    }
    
    /**
     * @brief Returns an iterator pointing to the end of the
     * reordered range.
     */
    this_t end() const
    {
      return super_t::end();
    }
    
    /**
     * @brief Returns the index that the current element has in the
     * input range.
     *
     * If ElementIterator is not random access, the complexity of
     * this function is linear in the number of elements in the
     * input range.
     */
    index_t src_index() const
    {
      return super_t::src_index();
    }
    
    /**
     * @brief Returns an iterator of the input range (@p
     * ElementIterator) which points to the element this iterator is
     * currently pointing to.
     */
    ElementIterator src_iterator() const
    {
      return super_t::src_iterator();
    }
    
  private:
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
          throw bad_parameter_value("random_reord_iter: "
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
   * @brief Helper function for creating a random_reord_iter.
   *
   * See @p trsl::random_reord_iter for a description
   * of arguments.
   *
   * Iterators created with this function have default order type (see
   * OrderType in prototype_reord_iter). If one wishes to select a
   * non-default order type, random_reord_iter must be used
   * explicitly.
   */
  template<class ElementIterator>
  random_reord_iter<ElementIterator>
  make_random_reord_iter
  (ElementIterator first,
   ElementIterator last,
   boost::optional<unsigned> permutationSize = same_size)
  {
    return random_reord_iter<ElementIterator>
    (first, last, permutationSize);
  }

  /**
   * @brief Helper function for creating a random_reord_iter.
   *
   * See @p trsl::random_reord_iter for a description
   * of arguments.
   *
   * Iterators created with this function have default order type (see
   * OrderType in prototype_reord_iter). If one wishes to select a
   * non-default order type, random_reord_iter must be used
   * explicitly.
   */
  template<class ElementIterator, class RandomNumberGenerator>
  random_reord_iter<ElementIterator>
  make_random_reord_iter
  (ElementIterator first,
   ElementIterator last,
   boost::optional<unsigned> permutationSize,
   RandomNumberGenerator& rng)
  {
    return random_reord_iter<ElementIterator>
    (first, last, permutationSize, rng);
  }
  
  //@}
} // namespace trsl

#endif // include guard
