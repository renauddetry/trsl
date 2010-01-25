// (C) Copyright Renaud Detry   2007-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_SYSTEMATIC_SAMPLE_ITERATOR_HPP
#define TRSL_SYSTEMATIC_SAMPLE_ITERATOR_HPP

#include <trsl/error_handling.hpp>
#include <trsl/common.hpp>
#include <trsl/is_picked_systematic.hpp>
#include <trsl/ppfilter_iterator_prototype.hpp>
#include <boost/type_traits/is_const.hpp>

namespace trsl
{
  
  namespace detail
  {
    template
    <
      class ElementIterator,
      class WeightAccessor,
      class WeightType,
      class Implementor
    >
    struct perm_syst_sample_iter_base
    {
      typedef is_picked_systematic
      <
        WeightAccessor,
        WeightType
      >
      predicate_t;
      
      typedef ppfilter_iterator_prototype
      <
        predicate_t,
        ElementIterator,
        Implementor
      >
      type; 
    };
  }
  
  /**
   * @brief
   */
  template
  <
    class ElementIterator,
    class WeightAccessor,
    class WeightType = double
  >
  class perm_syst_sample_iter :
    public detail::perm_syst_sample_iter_base
    <
      ElementIterator,
      WeightAccessor,
      WeightType,
      perm_syst_sample_iter<ElementIterator, WeightAccessor, WeightType>
    >::type
  {
    typedef detail::perm_syst_sample_iter_base
    <
      ElementIterator,
      WeightAccessor,
      WeightType,
      perm_syst_sample_iter<ElementIterator, WeightAccessor, WeightType>
    >
    base_t;
    typedef typename base_t::type super_t;
    
  public:
    
    typedef ElementIterator element_iterator;
    typedef WeightAccessor weight_accessor;
    typedef WeightType weight_t;
    typedef typename base_t::predicate_t predicate_t;
    
    perm_syst_sample_iter() {}
    
    explicit perm_syst_sample_iter(ElementIterator first,
                                        ElementIterator last,
                                        unsigned sampleSize,
                                        WeightType populationWeight,
                                        WeightAccessor wac) :
      super_t(predicate_t(sampleSize, populationWeight, wac), first, last) {}

    template<class RandomNumberGenerator>
    explicit perm_syst_sample_iter(ElementIterator first,
                                        ElementIterator last,
                                        unsigned sampleSize,
                                        WeightType populationWeight,
                                        WeightAccessor wac,
                                        RandomNumberGenerator& rng,
                                        WeightType uniform01) :
      super_t(predicate_t(sampleSize, populationWeight, uniform01, wac),
              first, last, rng) {}
    
    template<class Container>
    explicit perm_syst_sample_iter(Container& c,
                                        unsigned sampleSize) :
      super_t(predicate_t(sampleSize, c.size()), c.begin(), c.end())
    {
      // This constructor should only be used when WeightAccessor is
      // unit_weight_accessor.
      BOOST_STATIC_ASSERT
      ((boost::is_same<WeightAccessor, unit_weight_accessor<WeightType> >::value));
    }

    template<class Container, class RandomNumberGenerator>
    explicit perm_syst_sample_iter(Container& c,
                                        unsigned sampleSize,
                                        RandomNumberGenerator& rng,
                                        WeightType uniform01) :
      super_t(predicate_t(sampleSize, c.size(), uniform01), c.begin(), c.end(), rng)
    {
      // This constructor should only be used when WeightAccessor is
      // unit_weight_accessor.
      BOOST_STATIC_ASSERT
      ((boost::is_same<WeightAccessor, unit_weight_accessor<WeightType> >::value));
    }
    
    template<class OtherElementIterator>
    perm_syst_sample_iter
    (perm_syst_sample_iter<OtherElementIterator, WeightAccessor, WeightType> const& r,
     typename boost::enable_if_convertible
     <OtherElementIterator, ElementIterator>::type* = 0) :
      super_t(r) {}
  };
  
  template
  <
    class ElementIterator,
    class WeightAccessor,
    class WeightType
  >
  perm_syst_sample_iter<ElementIterator, WeightAccessor, WeightType>
  make_perm_syst_sample_iter
  (ElementIterator first,
   ElementIterator last,
   unsigned sampleSize,
   WeightType populationWeight,
   WeightAccessor wac)
  {
    return
    perm_syst_sample_iter<ElementIterator, WeightAccessor, WeightType>
    (first, last, sampleSize, populationWeight, wac);
  }

  template
  <
    class ElementIterator,
    class WeightAccessor,
    class WeightType,
    class RandomNumberGenerator
  >
  perm_syst_sample_iter<ElementIterator, WeightAccessor, WeightType>
  make_perm_syst_sample_iter
  (ElementIterator first,
   ElementIterator last,
   unsigned sampleSize,
   WeightType populationWeight,
   WeightAccessor wac,
   RandomNumberGenerator& rng,
   WeightType uniform01)
  {
    return
    perm_syst_sample_iter<ElementIterator, WeightAccessor, WeightType>
    (first, last, sampleSize, populationWeight, wac, rng, uniform01);
  }

  template
  <class Container>
  perm_syst_sample_iter
  <
    typename Container::iterator,
    unit_weight_accessor<>,
    double
  >
  make_perm_syst_sample_iter
  (Container& c,
   unsigned sampleSize)
  {
    return
    perm_syst_sample_iter
    <
      typename Container::iterator,
      unit_weight_accessor<>,
      double
    >
    (c, sampleSize);
  }

  template
  <class Container>
  perm_syst_sample_iter
  <
    typename Container::const_iterator,
    unit_weight_accessor<>,
    double
  >
  make_perm_syst_sample_iter
  (Container const& c,
   unsigned sampleSize)
  {
    return
    perm_syst_sample_iter
    <
      typename Container::const_iterator,
      unit_weight_accessor<>,
      double
    >
    (c, sampleSize);
  }
  
  template
  <class Container, class RandomNumberGenerator>
  perm_syst_sample_iter
  <
    typename Container::iterator,
    unit_weight_accessor<>,
    double
  >
  make_perm_syst_sample_iter
  (Container& c,
   unsigned sampleSize,
   RandomNumberGenerator& rng,
   double uniform01)
  {
    return
    perm_syst_sample_iter
    <
      typename Container::iterator,
      unit_weight_accessor<>,
      double
    >
    (c, sampleSize, rng, uniform01);
  }

  template
  <class Container, class RandomNumberGenerator>
  perm_syst_sample_iter
  <
    typename Container::const_iterator,
    unit_weight_accessor<>,
    double
  >
  make_perm_syst_sample_iter
  (Container const& c,
   unsigned sampleSize,
   RandomNumberGenerator& rng,
   double uniform01)
  {
    return
    perm_syst_sample_iter
    <
      typename Container::const_iterator,
      unit_weight_accessor<>,
      double
    >
    (c, sampleSize, rng, uniform01);
  }
  
} // namespace trsl

#endif // include guard
