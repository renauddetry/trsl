// (C) Copyright Toon Knapen    2001.
// (C) Copyright David Abrahams 2003.
// (C) Copyright Roland Richter 2003.
// (C) Copyright Renaud Detry   2007.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// This file is a fork of boost/iterator/permutation_iterator.hpp

/** @file */

#ifndef TRSL_REORDER_ITERATOR_HPP
#define TRSL_REORDER_ITERATOR_HPP

#include "trsl/error_handling.hpp"
#include "trsl/common.hpp"

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
  class reorder_iterator;
  
  namespace detail
  {
    /** @brief Used internally. */
    template<class ElementIterator>
    struct reorder_iterator_base
    {
      typedef ElementIterator element_iterator;
      typedef size_t index_t;
      typedef std::vector<index_t> index_container;
      typedef boost::shared_ptr<index_container> index_container_ptr;
      typedef typename index_container::const_iterator index_iterator;
    
      typedef boost::iterator_adaptor< 
        reorder_iterator<ElementIterator>,
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
   * only a specific reordering of the elements is needed (e.g. a <em>random</em> permutation), the array of index
   * can be managed internally; this is what
   * trsl::reorder_iterator does. The construction of the index array is left to subclasses.
   *
   * The index array is stored within the iterator, by means of a <a
   * href="http://www.boost.org/libs/smart_ptr/shared_ptr.htm"
   * >boost::shared_ptr</a>; thus, all copies of a random permutation
   * iterator share the same index array.
   *
   * When iterating over a permutation of a population range using an
   * index range, the iteration is actually performed over the index
   * range; the population range is only used when
   * dereferencing. Thus, every trsl::reorder_iterator
   * knows where it begins and where it ends, hence provided begin()
   * and end() methods.
   *
   * @p ElementIterator should model <em>Random Access Iterator</em>.
   * Subclasses of reorder_iterator that provide at least default constructor will model <em>Random Access Iterator</em>.
   * See the doc on <a
   * href="http://www.boost.org/libs/iterator/doc/permutation_iterator.html"
   * >boost::permutation_iterator</a> for further details.
   */
  template<class ElementIterator>
  class reorder_iterator
    : public detail::reorder_iterator_base<ElementIterator>::type
  {
    friend class boost::iterator_core_access;
    
      public:

    typedef detail::reorder_iterator_base<ElementIterator> base_t;
    typedef typename base_t::type super_t;
    
    typedef typename base_t::index_t index_t;
    typedef typename base_t::index_container index_container;
    typedef typename base_t::index_container_ptr index_container_ptr;
    typedef typename base_t::index_iterator index_iterator;

    
        typedef typename base_t::element_iterator element_iterator;

    reorder_iterator() :
      m_elt_iter(),
      m_index_collection(new index_container)
    {}
      
    explicit reorder_iterator(ElementIterator first,
                              index_container_ptr index_collection)
      : super_t(index_collection->begin()),
        m_elt_iter(first),
        m_index_collection(index_collection)
      {}
    
    /**
     * @brief Allows conversion from a reorder_iterator to
     * a const reorder_iterator, won't allow conversion
     * from a const reorder_iterator to a
     * reorder_iterator.
     *
     * By &ldquo;const reorder_iterator&rdquo;, we mean that the @p
     * ElementIterator is const, e.g.
     * <tt>std::vector<Particle>::const_iterator</tt>.
     */
    template<class OtherElementIterator>
    reorder_iterator
    (reorder_iterator<OtherElementIterator> const& r,
     typename boost::enable_if_convertible<OtherElementIterator, ElementIterator>::type* = 0) :
      super_t(r.base()), m_elt_iter(r.m_elt_iter),
      m_index_collection(r.m_index_collection)
      {}

    /**
     * @brief Returns a reorder_iterator pointing to
     * the begining of the permutation.
     */
    reorder_iterator<ElementIterator> begin() const
      {
        reorder_iterator<ElementIterator> indexIterator(*this);
        indexIterator.base_reference() =
          indexIterator.m_index_collection->begin();
        return indexIterator;
      }
    
    /**
     * @brief Returns a reorder_iterator pointing to
     * the end of the permutation.
     */
    reorder_iterator<ElementIterator> end() const
      {
        reorder_iterator<ElementIterator> indexIterator(*this);
        indexIterator.base_reference() =
          indexIterator.m_index_collection->end();
        return indexIterator;
      }
      
  static reorder_iterator<ElementIterator> create_random_permutation(element_iterator first,
                              element_iterator last,
                              unsigned permutationSize = 0)
      {
        ptrdiff_t size = std::distance(first, last);
        if (size < 0)
          throw bad_parameter_value(
            "random_permutation_iterator: "
            "bad input range.");
        if (permutationSize > size)
          throw bad_parameter_value(
            "random_permutation_iterator: "
            "parameter permutationSize out of range.");
        
        index_container_ptr index_collection(new index_container);
        
        index_collection->resize(size);
        for (index_t i = 0; i < size; ++i)
          (*index_collection)[i] = i;
        std::random_shuffle(index_collection->begin(),
                            index_collection->end(),
                            rand_gen::uniform_int);
        index_collection->resize(permutationSize);
        return reorder_iterator<ElementIterator>(first, index_collection);
      }

  private:
      
    typename super_t::reference dereference() const
      { return *(m_elt_iter + *this->base()); }
    
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
    template <class> friend class reorder_iterator;
#else
  public:
#endif 
    ElementIterator m_elt_iter;
  protected:
    index_container_ptr m_index_collection;
  };
  
} // namespace trsl

#endif // include guard