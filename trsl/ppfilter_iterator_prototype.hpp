// (C) Copyright Renaud Detry   2008-2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_PPFILTER_ITERATOR_PROTOTYPE_HPP
#define TRSL_PPFILTER_ITERATOR_PROTOTYPE_HPP

#include <trsl/error_handling.hpp>
#include <trsl/common.hpp>

#include <trsl/random_permutation_iterator.hpp>
#include <trsl/persistent_filter_iterator.hpp>

namespace trsl
{
  namespace detail
  {
    template <class Predicate, class Iterator>
    persistent_filter_iterator<Predicate,Iterator>
    make_persistent_filter_iterator(Predicate f, Iterator x)
    {
      return persistent_filter_iterator<Predicate,Iterator>(f,x.begin(), x.end());
    }
    
    
    /** @brief Used internally. */
    template<class Predicate, class ElementIterator, class DerivedType>
    struct ppfilter_iterator_base_prototype
    {
      typedef Predicate predicate_t;
      typedef ElementIterator element_iterator;
    
      typedef random_permutation_iterator<ElementIterator> upstream_iterator;
      typedef persistent_filter_iterator<
        Predicate, upstream_iterator> downstream_iterator;
    
      typedef boost::iterator_adaptor< 
        DerivedType,
        downstream_iterator,
        typename boost::detail::iterator_traits<ElementIterator>::value_type,
        boost::forward_traversal_tag,
        typename boost::detail::iterator_traits<ElementIterator>::reference
      > type;
    };
  }
  
  
  /**
   * @brief Random permutation, persistent filter iterator.
   *
   * This class pipes a random_permutation_iterator (upstream) with
   * a persistent_filter_iterator (downstream).  It is intended to be
   * used with is_picked_systematic. Systematic sampling of a random
   * permutation achieves <em>probability sampling</em>.
   *
   * @p ElementIterator should model <em>Random Access Iterator</em>.
   */
  template<class Predicate, class ElementIterator, class DerivedType>
  class ppfilter_iterator_prototype :
    public detail::ppfilter_iterator_base_prototype
    <Predicate, ElementIterator, DerivedType>::type
  {
    typedef detail::ppfilter_iterator_base_prototype
    <Predicate, ElementIterator, DerivedType> base_t;
    typedef typename base_t::type super_t;

    friend class boost::iterator_core_access;
    
  protected:
    typedef typename base_t::upstream_iterator upstream_iterator;
    typedef typename base_t::downstream_iterator downstream_iterator;
    
  public:
    
    typedef ElementIterator element_iterator;
    typedef Predicate predicate_t;

    ppfilter_iterator_prototype() :
      super_t(), predicate_()
      {}
      
    /**
     * @brief Constructor.
     */
    explicit ppfilter_iterator_prototype(Predicate f,
                                         ElementIterator first,
                                         ElementIterator last) :
      super_t(detail::make_persistent_filter_iterator
              (f, make_random_permutation_iterator(first, last))),
      predicate_(f)
    {}

    /**
     * @brief Constructor.
     */
    template<class RandomNumberGenerator>
    explicit ppfilter_iterator_prototype(Predicate f,
                                         ElementIterator first,
                                         ElementIterator last,
                                         RandomNumberGenerator& rng) :
      super_t(detail::make_persistent_filter_iterator
              (f, make_random_permutation_iterator(first, last,
                                                   same_size,
                                                   rng))),
      predicate_(f)
    {}
    
    /**
     * @brief Allows conversion from a ppfilter_iterator_prototype to a const
     * ppfilter_iterator_prototype, won't allow conversion from a const
     * ppfilter_iterator_prototype to a ppfilter_iterator_prototype.
     *
     * By &ldquo;const ppfilter_iterator_prototype&rdquo;, we mean that the @p
     * ElementIterator is const, e.g.
     * <tt>std::vector<Particle>::const_iterator</tt>.
     */
    template<class OtherElementIterator, class OtherDerivedType>
    ppfilter_iterator_prototype
    (ppfilter_iterator_prototype
     <
       Predicate,
       OtherElementIterator,
       OtherDerivedType
     > const& r,
     typename boost::enable_if_convertible
     <OtherElementIterator, ElementIterator>::type* = 0,
     typename boost::enable_if_convertible
     <OtherDerivedType, DerivedType>::type* = 0) :
      super_t(r.base()), predicate_(r.predicate_) {}

    /**
     * @brief Returns a ppfilter_iterator_prototype pointing to the begining of
     * the range.
     */
    DerivedType begin() const
      {
        DerivedType i
        (static_cast<const DerivedType&>(*this));
        i.base_reference() =
          downstream_iterator(predicate_,
                              this->base_reference().base().begin(),
                              this->base_reference().base().end());
        return i;
      }
    
    /**
     * @brief Returns a ppfilter_iterator_prototype pointing to
     * the end of the range.
     */
    DerivedType end() const
      {
        DerivedType i
        (static_cast<const DerivedType&>(*this));
        i.base_reference() =
          downstream_iterator(predicate_,
                              this->base_reference().base().end(),
                              this->base_reference().base().end());
        return i;
      }
    
    /**
     * @brief Returns the index of the element that the iterator is
     * currently pointing to.
     */
    typename upstream_iterator::index_t src_index() const
    { return this->base().base().src_index(); }

    /**
     * @brief Returns an ElementIterator pointing to the element the pp_filter_iterator is
     * currently pointing to.
     */
    typename upstream_iterator::element_iterator src_iterator() const
    { return this->base().base().src_iterator(); }
    
    /**
     * @brief Returns the persistent_filter_iterator predicate.
     */
    Predicate predicate() const { return this->base_reference().predicate(); }

  private:
    
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
    template <class, class, class> friend class ppfilter_iterator_prototype;
#else
  public:
#endif
    // Need to store the predicate to implement begin().
    // Else, a predicate_.reset() method would be required.
    Predicate predicate_;
  };
  
} // namespace trsl

#endif // include guard
