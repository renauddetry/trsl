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

#ifndef TRSL_PERMUTATION_ITERATOR_HPP
#define TRSL_PERMUTATION_ITERATOR_HPP

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
   * >boost::iterator::permutation_iterator</a>.  With
   * <a
   * href="http://www.boost.org/libs/iterator/doc/permutation_iterator.html"
   * >boost::iterator::permutation_iterator</a>, the user provides a
   * population, and an range of ordered index that define a
   * permutation over the population. This allows for much flexibility, but the user is
   * responsible for generating and storing an array of index. When
   * only random permutations are needed, the array of index could be
   * managed internally; this is what trsl::random_permutation_iterator
   * does. The array is stored within each iterator, by means of a <a
   * href="http://www.boost.org/libs/smart_ptr/shared_ptr.htm"
   * >boost::shared_ptr</a>.
   *
   * When iterating over a permutation of a population range using an
   * index range, the iteration is actually performed over the index
   * range; the population range is only used when dereferencing. Thus, every
   * trsl::random_permutation_iterator knows where it begins and where it
   * ends, hence provided trsl::random_permutation_iterator::begin() and
   * trsl::random_permutation_iterator::end() methods.
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
    random_permutation_iterator() : m_elt_iter() {}
    
    explicit random_permutation_iterator(ElementIterator begin,
                                         ElementIterator end)
      : m_elt_iter(begin), m_index_collection(new index_collection)
      {
        index_t size = std::distance(begin, end);
        m_index_collection->resize(size);
        for (index_t i = 0; i < size; ++i)
          (*m_index_collection)[i] = i;
        std::random_shuffle(m_index_collection->begin(),
                            m_index_collection->end());
        this->base_reference() = m_index_collection->begin();
      }
    
    explicit random_permutation_iterator(ElementIterator begin,
                                         ElementIterator end,
                                         index_t permutationSize)
      : m_elt_iter(begin), m_index_collection(new index_collection)
      {
        index_t size = std::distance(begin, end);
        m_index_collection->resize(size);
        for (index_t i = 0; i < size; ++i)
          (*m_index_collection)[i] = i;
        std::random_shuffle(m_index_collection->begin(),
                            m_index_collection->end());
        m_index_collection->resize(permutationSize);
        this->base_reference() = m_index_collection->begin();
      }
    
    template<class OtherElementIterator>
    random_permutation_iterator
    (random_permutation_iterator<OtherElementIterator> const& r,
     typename boost::enable_if_convertible<
     OtherElementIterator, ElementIterator>::type* = 0) :
      super_t(r.base()), m_elt_iter(r.m_elt_iter),
      m_index_collection(r.m_index_collection)
      {}
    
    random_permutation_iterator<ElementIterator> begin() const
      {
        random_permutation_iterator<ElementIterator> indexIterator(*this);
        indexIterator.base_reference() =
          indexIterator.m_index_collection->begin();
        return indexIterator;
      }
    
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
    index_collection_ptr m_index_collection;
  };
  
  
  template<class ElementIterator>
  random_permutation_iterator<ElementIterator> 
  make_random_permutation_iterator(ElementIterator begin, ElementIterator end)
  {
    return random_permutation_iterator<ElementIterator>(begin, end);
  }
  
  
} // namespace trsl

#endif
