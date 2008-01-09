// (C) Copyright Toon Knapen    2001.
// (C) Copyright David Abrahams 2003.
// (C) Copyright Roland Richter 2003.
// (C) Copyright Renaud Detry   2007.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is a fork of boost/iterator/permutation_iterator.hpp
// Original copyright notice follows.

/** @file */

#ifndef TRSL_RANDOM_PERMUTATION_ITERATOR_HPP
#define TRSL_RANDOM_PERMUTATION_ITERATOR_HPP

#include "trsl/common.hpp"
#include "trsl/error_handling.hpp"

#include <iterator>
#include <vector>
#include <algorithm>
#include <boost/iterator.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/shared_ptr.hpp>

namespace trsl
{
  template<class ElementIterator>
  class random_permutation_iterator;
  
  /** @brief Used internally. */
  namespace detail
  {
    /** @brief Used internally. */
    template<class ElementIterator>
    struct random_permutation_iterator_base
    {
      typedef size_t index_t;
      typedef std::vector<index_t> index_collection;
      typedef boost::shared_ptr<index_collection> index_collection_ptr;
      typedef typename index_collection::const_iterator index_iterator;
    
      typedef boost::iterator_adaptor< 
        random_permutation_iterator<ElementIterator>,
        index_iterator,
        typename boost::detail::iterator_traits<ElementIterator>::value_type,
        boost::use_default,
        typename boost::detail::iterator_traits<ElementIterator>::reference
      > type;
    };
  }
  
  
  /**
   * @brief Provides an iterator over a random permutation of a range.
   *
   * This class is a fork of <a
   * href="http://www.boost.org/libs/iterator/doc/permutation_iterator.html"
   * >boost::permutation_iterator</a>.  With <a
   * href="http://www.boost.org/libs/iterator/doc/permutation_iterator.html"
   * >boost::permutation_iterator</a>, the user provides a population,
   * and a range of index that define a permutation over the
   * population. It allows for much flexibility, but leaves the user
   * responsible for generating and storing an array of index. When
   * only <em>random</em> permutations are needed, the array of index
   * can be managed internally; this is what
   * trsl::random_permutation_iterator does.
   *
   * The index array is stored within the iterator, by means of a <a
   * href="http://www.boost.org/libs/smart_ptr/shared_ptr.htm"
   * >boost::shared_ptr</a>; thus, all copies of a random permutation
   * iterator share the same index array.
   *
   * When iterating over a permutation of a population range using an
   * index range, the iteration is actually performed over the index
   * range; the population range is only used when
   * dereferencing. Thus, every trsl::random_permutation_iterator
   * knows where it begins and where it ends, hence provided begin()
   * and end() methods.
   *
   * @p ElementIterator should model <em>Random Access Iterator</em>.
   * random_permutation_iterator models <em>Random Access Iterator</em>.
   * See the doc on <a
   * href="http://www.boost.org/libs/iterator/doc/permutation_iterator.html"
   * >boost::permutation_iterator</a> for further details.
   */
  template<class ElementIterator>
  class random_permutation_iterator
    : public detail::random_permutation_iterator_base<ElementIterator>::type
  {
    typedef detail::random_permutation_iterator_base<ElementIterator> base_t;
    typedef typename base_t::type super_t;
    
    typedef typename base_t::index_t index_t;
    typedef typename base_t::index_collection index_collection;
    typedef typename base_t::index_collection_ptr index_collection_ptr;
    typedef typename index_collection::const_iterator index_iterator;
    
    friend class boost::iterator_core_access;
  public:
    typedef ElementIterator element_iterator;
    
    random_permutation_iterator() :
      m_elt_iter(),
      m_index_collection(new index_collection)
    {}
        
    /**
     * @brief Constructs an iterator that will iterate through a
     * random permutation of the population referenced by @p first and @p
     * last.
     *
     * Performing a random permutation requires a series of random
     * integers, these are provided by rand_gen::uniform_int; see @ref
     * random for further details.
     */
    explicit random_permutation_iterator(ElementIterator first,
                                         ElementIterator last)
      : m_elt_iter(first), m_index_collection(new index_collection)
      {
        index_t size = std::distance(first, last);
        m_index_collection->resize(size);
        for (index_t i = 0; i < size; ++i)
          (*m_index_collection)[i] = i;
        std::random_shuffle(m_index_collection->begin(),
                            m_index_collection->end(),
                            rand_gen::uniform_int);
        this->base_reference() = m_index_collection->begin();
      }
    
    /**
     * @brief Constructs an iterator that will iterate through the
     * first @p permutationSize elements of a random permutation of the
     * population referenced by @p first and @p last.
     *
     * Let \f$n\f$ be the size of the population. This constructor
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
     */
    explicit random_permutation_iterator(ElementIterator first,
                                         ElementIterator last,
                                         index_t permutationSize)
      : m_elt_iter(first), m_index_collection(new index_collection)
      {
        index_t size = std::distance(first, last);
        if (permutationSize > size)
        {
          throw bad_parameter_value(
            "random_permutation_iterator: "
            "parameter permutationSize out of range.");
        }
        m_index_collection->resize(size);
        for (index_t i = 0; i < size; ++i)
          (*m_index_collection)[i] = i;
        std::random_shuffle(m_index_collection->begin(),
                            m_index_collection->end(),
                            rand_gen::uniform_int);
        m_index_collection->resize(permutationSize);
        this->base_reference() = m_index_collection->begin();
      }
    
    /**
     * @brief Allows conversion from a random_permutation_iterator to
     * a const random_permutation_iterator, won't allow conversion
     * from a const random_permutation_iterator to a
     * random_permutation_iterator.
     *
     * By &ldquo;const random_permutation_iterator&rdquo;, we mean that the @p
     * ElementIterator is const, e.g.
     * <tt>std::vector<Particle>::const_iterator</tt>.
     */
    template<class OtherElementIterator>
    random_permutation_iterator
    (random_permutation_iterator<OtherElementIterator> const& r,
     typename boost::enable_if_convertible<
     OtherElementIterator, ElementIterator>::type* = 0) :
      super_t(r.base()), m_elt_iter(r.m_elt_iter),
      m_index_collection(r.m_index_collection)
      {}
    
    /**
     * @brief Returns a random_permutation_iterator pointing to
     * the begining of the permutation.
     */
    random_permutation_iterator<ElementIterator> begin() const
      {
        random_permutation_iterator<ElementIterator> indexIterator(*this);
        indexIterator.base_reference() =
          indexIterator.m_index_collection->begin();
        return indexIterator;
      }
    
    /**
     * @brief Returns a random_permutation_iterator pointing to
     * the end of the permutation.
     */
    random_permutation_iterator<ElementIterator> end() const
      {
        random_permutation_iterator<ElementIterator> indexIterator(*this);
        indexIterator.base_reference() =
          indexIterator.m_index_collection->end();
        return indexIterator;
      }
  private:
      
    typename super_t::reference dereference() const
      { return *(m_elt_iter + *this->base()); }
    
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
    template <class> friend class random_permutation_iterator;
#else
  public:
#endif 
    ElementIterator m_elt_iter;
  private:
    index_collection_ptr m_index_collection;
  };
  
  /** @brief Makes a random_permutation_iterator. */
  template<class ElementIterator>
  random_permutation_iterator<ElementIterator> 
  make_random_permutation_iterator(ElementIterator first, ElementIterator last)
  {
    return random_permutation_iterator<ElementIterator>(first, last);
  }
  
  
} // namespace trsl

#endif
