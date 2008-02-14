// (C) Copyright Renaud Detry   2008.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_PPFILTER_ITERATOR_HPP
#define TRSL_PPFILTER_ITERATOR_HPP

#include "trsl/error_handling.hpp"
#include "trsl/common.hpp"

#include "trsl/reorder_iterator.hpp"
#include "trsl/persistent_filter_iterator.hpp"

namespace trsl
{
  template<class Predicate, class ElementIterator>
  class ppfilter_iterator;
  
  namespace detail
  {
    /** @brief Used internally. */
    template<class Predicate, class ElementIterator>
    struct ppfilter_iterator_base
    {
      typedef Predicate predicate_t;
      typedef ElementIterator element_iterator;
    
      typedef reorder_iterator<ElementIterator> upstream_iterator;
      typedef persistent_filter_iterator<Predicate,  upstream_iterator> downstream_iterator;
    
      typedef boost::iterator_adaptor< 
        ppfilter_iterator<Predicate, ElementIterator>,
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
   * This class pipes a random_permutation_iterator (upstream) with a persistent_filter_iterator (downstream).
   *
   * @p ElementIterator should model <em>Random Access Iterator</em>.
   */
  template<class Predicate, class ElementIterator>
  class ppfilter_iterator
    : public detail::ppfilter_iterator_base<Predicate, ElementIterator>::type
  {
    typedef detail::ppfilter_iterator_base<Predicate, ElementIterator> base_t;
    typedef typename base_t::type super_t;

    friend class boost::iterator_core_access;
    
    typedef typename base_t::upstream_iterator upstream_iterator;
    typedef typename base_t::downstream_iterator downstream_iterator;

  public:
    
    typedef typename base_t::element_iterator element_iterator;

    ppfilter_iterator() :
      super_t(), predicate_()
      {}
      
    explicit ppfilter_iterator(Predicate f, ElementIterator first, ElementIterator last)
      : super_t(), predicate_(f)
      {
        upstream_iterator ui = random_permutation_iterator(first, last);
        this->base_reference() = downstream_iterator(f, ui.begin(), ui.end());
      }
    
    /**
     * @brief Allows conversion from a ppfilter_iterator to
     * a const ppfilter_iterator, won't allow conversion
     * from a const ppfilter_iterator to a
     * ppfilter_iterator.
     *
     * By &ldquo;const ppfilter_iterator&rdquo;, we mean that the @p
     * ElementIterator is const, e.g.
     * <tt>std::vector<Particle>::const_iterator</tt>.
     */
    template<class OtherElementIterator>
    ppfilter_iterator
    (ppfilter_iterator<Predicate, OtherElementIterator> const& r,
     typename boost::enable_if_convertible<OtherElementIterator, ElementIterator>::type* = 0) :
      super_t(r.base()), predicate_(r.predicate_)
      {}

    /**
     * @brief Returns a ppfilter_iterator pointing to
     * the begining of the range.
     */
    ppfilter_iterator<Predicate, ElementIterator> begin() const
      {
        ppfilter_iterator<Predicate, ElementIterator> i(*this);
        i.base_reference() = downstream_iterator(predicate_,
                                             this->base_reference().base().begin(),
                                             this->base_reference().base().end());
        return i;
      }
    
    /**
     * @brief Returns a ppfilter_iterator pointing to
     * the end of the range.
     */
    ppfilter_iterator<Predicate, ElementIterator> end() const
      {
        ppfilter_iterator<Predicate, ElementIterator> i(*this);
        i.base_reference() = downstream_iterator(predicate_,
                                             this->base_reference().base().end(),
                                             this->base_reference().base().end());
        return i;
      }
      
  private:
    
#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
    template <class, class> friend class ppfilter_iterator;
#else
  public:
#endif
    // Need to store the predicate to implement begin().
    // Else, a predicate_.reset() method would be required.
    const Predicate predicate_;
  };
  
} // namespace trsl

#endif // include guard
