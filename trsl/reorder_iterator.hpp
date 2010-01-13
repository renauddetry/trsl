// (C) Copyright Toon Knapen    2001.
// (C) Copyright David Abrahams 2003.
// (C) Copyright Roland Richter 2003.
// (C) Copyright Renaud Detry   2007-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_REORDER_ITERATOR_HPP
#define TRSL_REORDER_ITERATOR_HPP

#include <trsl/error_handling.hpp>
#include <trsl/common.hpp>

#include <iterator>
#include <vector>
#include <boost/iterator.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_base_of.hpp>
#include <boost/optional.hpp>

namespace trsl
{

  boost::none_t const same_size = boost::none;
  
  /**
   * @brief Indicates that @em positions should be implemented with iterators.
   */
  struct iterator_order_tag {};
  /**
   * @brief Indicates that @em positions should be implemented with integers.
   */
  struct index_order_tag {};
  /**
   * @brief default_order_tag<ElementIterator>::type is index_order_tag if
   * ElementIterator is
   * random access. Else, it corresponds to iterator_order_tag.
   */
  template<class ElementIterator>
  struct default_order_tag
  {
    /**
     * @brief Defined as index_order_tag if ElementIterator is
     * random access. Else, it corresponds to iterator_order_tag.
     */
    typedef typename boost::mpl::if_
    <
      boost::is_convertible
      <
        typename boost::iterator_traversal<ElementIterator>::type,
        boost::random_access_traversal_tag
      >,
      index_order_tag,
      iterator_order_tag
    >::type type;
  };
    
  namespace detail
  {
    
    /** @internal @brief Used internally */
    template<class ElementIterator, class OrderTag, class DerivedType>
    struct reorder_iterator_base
    {      
      typedef ElementIterator element_iterator;
      typedef OrderTag order_tag;
      typedef size_t index_t;
      typedef typename boost::mpl::if_
      <
        boost::is_same<OrderTag, index_order_tag>,
        index_t,
        ElementIterator
      >::type position_t;
      typedef std::vector<position_t> position_container;
      typedef boost::shared_ptr<position_container> position_container_ptr;
      typedef typename position_container::const_iterator position_iterator;
      
      typedef boost::iterator_adaptor
      < 
        DerivedType,
        position_iterator,
        typename boost::detail::iterator_traits<ElementIterator>::value_type,
        boost::use_default,
        typename boost::detail::iterator_traits<ElementIterator>::reference
      >
      type;
    };
    
    /** @internal @brief Used internally */
    template<class ElementIterator, class IndexContainer, class IndexType>
    void
    fill_index_container(IndexContainer& collection,
                         ElementIterator first,
                         ElementIterator last,
                         index_order_tag)
    {
      collection.resize(std::distance(first, last));
      for (IndexType i = 0; i < IndexType(std::distance(first, last)); ++i)
        collection[i] = i;
    }
    
    /** @internal @brief Used internally */
    template<class ElementIterator, class IndexContainer, class IndexType>
    void
    fill_index_container(IndexContainer& collection,
                         ElementIterator first,
                         ElementIterator last,
                         iterator_order_tag)
    {
      for (ElementIterator i = first; i != last; ++i)
        collection.push_back(i);
    }    
    
  }
  
  
  /**
   * @brief Provides an iterator over a permutation of a range.
   *
   * This class is a fork of <a
   * href="http://www.boost.org/libs/iterator/doc/permutation_iterator.html"
   * >boost::permutation_iterator</a>.  With <a
   * href="http://www.boost.org/libs/iterator/doc/permutation_iterator.html"
   * >boost::permutation_iterator</a>, the user provides a population,
   * and a range of index that defines a permutation over the
   * population. It allows for much flexibility, but leaves the user
   * responsible for storing an array of index. This
   * class allows to store the array internally, in the same way as <a
   * href="http://www.boost.org/libs/utility/shared_container_iterator.html"
   * >boost::shared_container_iterator</a>.
   *
   * The index array is stored within the iterator, by means of a <a
   * href="http://www.boost.org/libs/smart_ptr/shared_ptr.htm"
   * >boost::shared_ptr</a>; thus, all copies of a reorder iterator
   * share the same array. One drawback is that reorder_iterator
   * copy is somewhat slower than ElementIterator copy. Incrementation
   * is still plainly fast, nevertheless.
   *
   * Another difference with boost::permutation_iterator is that while boost::permutation_iterator requires the input range to model <em>Random Access Iterator</em>, reorder_iterator only requires <em>Forward Iterators</em>. Thus, reorder_iterator can be applied to e.g. <tt>std::list</tt> iterators.
   *
   * When the input range is a Random Access Iterator, the internal array contains element indices with a <tt>std::vector<size_t></tt>. Else, it contains iterators of the input range, in a <tt>std::vector<ElementIterator></tt>. Below, indices and iterators contained in reorder_iterator's internal array are referred to as <em>positions</em>.
   *
   * When iterating over a permutation of a population range using a
   * position range, the iteration is actually performed over the position
   * range; the population range is only used when
   * dereferencing. Thus, every trsl::reorder_iterator knows where it
   * begins and where it ends, hence provided begin() and end()
   * methods.
   *
   * TRSL provides several functions that generate reoder iterators
   * for common reorderings. See random_permutation_iterator,
   * sort_iterator, custom_permutation_iterator.
   *
   * @param ElementIterator (template parameter) Corresponds to the type of the iterator through which input elements are referenced to 
   * custom_permutation_iterator::custom_permutation_iterator().
   * It should model <em>Forward Iterator</em>.
   */
  template
  <
    class ElementIterator,
    class OrderTag,
    class DerivedType
  >
  class reorder_iterator :
    public detail::reorder_iterator_base
    <
      ElementIterator,
      OrderTag,
      DerivedType
    >::type
  {
    typedef detail::reorder_iterator_base
    <
      ElementIterator, OrderTag, DerivedType
    >
    base_t;
    typedef typename base_t::type super_t;
    
    friend class boost::iterator_core_access;
    
//    BOOST_STATIC_ASSERT(
//                        (boost::is_base_of
//                         <
//                           reorder_iterator
//                           <
//                             ElementIterator, OrderTag, DerivedType
//                           >,
//                           DerivedType
//                         >::value)
//                        );
    
  public:
    
    typedef ElementIterator element_iterator;
    typedef OrderTag order_tag;
    typedef typename base_t::index_t index_t;
    typedef typename base_t::position_t position_t;
    typedef typename base_t::position_container position_container;
    typedef typename base_t::position_container_ptr position_container_ptr;
    typedef typename base_t::position_iterator position_iterator;
    
    reorder_iterator() :
      m_elt_iter(),
      m_index_collection(new position_container)
    {}
    
    /**
     * @brief Constructs an iterator that will walk through the elements
     * of the range that begins at @p first, follwing the order defined
     * by @p position_collection.
     */
    explicit reorder_iterator(ElementIterator first,
                              const position_container_ptr& position_collection) :
      super_t(position_collection->begin()),
      m_elt_iter(first),
      m_index_collection(position_collection)
    {}
    
    /**
     * @brief Allows conversion from an iterator to
     * a const iterator, won't allow conversion
     * from a const iterator to an iterator.
     *
     * By &ldquo;const iterator&rdquo;, we mean that the @p
     * ElementIterator is const, e.g.
     * <tt>std::vector<Particle>::const_iterator</tt>.
     */
    template<class OtherDerivedType>
    reorder_iterator
    (OtherDerivedType const& r,
     typename boost::enable_if_convertible<OtherDerivedType, DerivedType>::type* = 0) :
      super_t(r.base()), m_elt_iter(r.m_elt_iter),
      m_index_collection(r.m_index_collection)
    {}
    
    
    template<class OtherElementIterator, class OtherDerivedType>
    reorder_iterator
    (reorder_iterator<OtherElementIterator, OrderTag, OtherDerivedType> const& r,
     typename boost::enable_if_convertible<OtherElementIterator, ElementIterator>::type* = 0,
     typename boost::enable_if_convertible<OtherDerivedType, DerivedType>::type* = 0) :
    super_t(r.base()), m_elt_iter(r.m_elt_iter),
    m_index_collection(r.m_index_collection)
      {}
    
    /**
     * @brief Returns an iterator pointing to
     * the begining of the permutation.
     */
    DerivedType begin() const
    {
      DerivedType indexIterator(static_cast<const DerivedType&>(*this));
      indexIterator.base_reference() =
      indexIterator.m_index_collection->begin();
      return indexIterator;
    }
    
    /**
     * @brief Returns an iterator pointing to
     * the end of the permutation.
     */
    DerivedType end() const
    {
      DerivedType indexIterator(static_cast<const DerivedType&>(*this));
      indexIterator.base_reference() =
      indexIterator.m_index_collection->end();
      return indexIterator;
    }
    
    /**
     * @brief Returns the index of the element in the input population that the iterator is
     * currently pointing to.
     *
     * If ElementIterator is not random access, then the complexity of this function is linear in the number of elements in the input range.
     */
    index_t src_index() const
    {
      return src_index(OrderTag());
    }
    
    /**
     * @brief Returns an ElementIterator pointing to the element the iterator is
     * currently pointing to.
     */
    ElementIterator src_iterator() const
    {
      return src_iterator(OrderTag());
    }
    
  protected:
    index_t src_index(index_order_tag) const
    {
      return *this->base();
    }
    ElementIterator src_iterator(index_order_tag) const
    {
      return m_elt_iter + *this->base();
    }
    
    index_t src_index(iterator_order_tag) const
    {
      return std::distance(m_elt_iter, *this->base());
    }
    ElementIterator src_iterator(iterator_order_tag) const
    {
      return *this->base();
    }
    
  private:
    
    typename super_t::reference dereference() const
    { return dereference(OrderTag()); }
    
    typename super_t::reference dereference(index_order_tag) const
    { return *(m_elt_iter + *this->base()); }
    
    typename super_t::reference dereference(iterator_order_tag) const
    { return *(*this->base()); }
    
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
    template <class, class, class> friend class reorder_iterator;
#else
  public:
#endif 
    ElementIterator m_elt_iter;
  protected:
    position_container_ptr m_index_collection;
  };
  
} // namespace trsl

#endif // include guard
