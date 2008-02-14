// (C) Copyright Renaud Detry   2007.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_SORT_ITERATOR_HPP
#define TRSL_SORT_ITERATOR_HPP

#include "trsl/reorder_iterator.hpp"
#include "trsl/common.hpp"
#include "trsl/error_handling.hpp"

#include <functional>

namespace trsl
{

  namespace detail {
  
    template<
      class RandomIterator,
      class Comparator
      > class at_index_comp
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
   * @brief Constructs a reorder_iterator that will iterate through the
   * first @p permutationSize elements of a random permutation of the
   * population referenced by @p first and @p last.
   *
   * Let \f$n\f$ be the size of the population. This function
   * shuffles an array of \f$n\f$ index, then discards its last
   * \f$n-permutationSize\f$ elements. A more efficient method
   * will be implemented for the next release.
   *
   * The @p permutationSize should be smaller or equal to the
   * size of the population. If it is not the case, a bad_parameter_value
   * is thrown.
   *
   * Performing a random permutation requires a series of random
   * integers, these are provided by rand_gen::uniform_int; see @ref
   * random for further details.

   * @p ElementIterator should model <em>Random Access Iterator</em>.
   */
  template<class ElementIterator, class ElementComparator>
  reorder_iterator<ElementIterator>
  sort_iterator(ElementIterator first,
                ElementIterator last,
                ElementComparator comp,
                unsigned permutationSize)
  {
    ptrdiff_t size = std::distance(first, last);
    if (size < 0)
      throw bad_parameter_value(
        "sort_iterator: "
        "bad input range.");
    if (permutationSize > unsigned(size))
      throw bad_parameter_value(
        "sort_iterator: "
        "parameter permutationSize out of range.");
        
    typedef
      typename reorder_iterator<ElementIterator>::index_container
      index_container;
    typedef
      typename reorder_iterator<ElementIterator>::index_container_ptr
      index_container_ptr;
    typedef
      typename reorder_iterator<ElementIterator>::index_t
      index_t;
  
    index_container_ptr index_collection(new index_container);
        
    index_collection->resize(size);
    for (index_t i = 0; i < index_t(size); ++i)
      (*index_collection)[i] = i;
    
    if (permutationSize == unsigned(size))
      std::sort(index_collection->begin(),
                index_collection->end(),
                detail::at_index_comp
                <ElementIterator, ElementComparator>(first, comp));
    else
    {
      std::partial_sort(index_collection->begin(),
                        index_collection->begin()+permutationSize,
                        index_collection->end(),
                        detail::at_index_comp
                        <ElementIterator, ElementComparator>(first, comp));
      index_collection->resize(permutationSize);
    }
    
    return reorder_iterator<ElementIterator>(first, index_collection);
  }

  /**
   * @brief Constructs an iterator that will iterate through a
   * random permutation of the population referenced by @p first and @p
   * last.
   *
   * Performing a random permutation requires a series of random
   * integers, these are provided by rand_gen::uniform_int; see @ref
   * random for further details.
   */
  template<class ElementIterator, class ElementComparator>
  reorder_iterator<ElementIterator>
  sort_iterator(ElementIterator first,
                ElementIterator last,
                ElementComparator comp)
  {
    return sort_iterator(first,
                         last,
                         comp,
                         std::distance(first, last));
  }

  template<class ElementIterator>
  reorder_iterator<ElementIterator>
  sort_iterator(ElementIterator first,
                ElementIterator last)
  {
    return sort_iterator(first,
                         last,
                         std::less
                         <typename std::iterator_traits
                         <ElementIterator>::value_type>());
  }

} // namespace trsl

#endif // include guard
