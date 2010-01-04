// (C) Copyright Renaud Detry   2008-2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_STL_CONTAINER_ITERATOR_FACTORY_HPP
#define TRSL_STL_CONTAINER_ITERATOR_FACTORY_HPP

#include <trsl/systematic_sample_iterator.hpp>
#include <trsl/random_permutation_iterator.hpp>
#include <trsl/sort_iterator.hpp>
#include <trsl/custom_permutation_iterator.hpp>

namespace trsl
{
  template<class Container>
  struct stl_container_iterator_factory
  {    
    // systematic_sample_iterator
    
    typedef
    ::trsl::systematic_sample_iterator
    <
      typename Container::iterator,
      unit_weight_accessor<double>
    >
    systematic_sample_iterator;
    
    typedef
    ::trsl::systematic_sample_iterator
    <
      typename Container::const_iterator,
      unit_weight_accessor<double>
    >
    const_systematic_sample_iterator;
    
    // random_permutation_iterator
    
    typedef
    ::trsl::random_permutation_iterator
    <typename Container::iterator>
    random_permutation_iterator;
    
    typedef
    ::trsl::random_permutation_iterator
    <typename Container::const_iterator>
    const_random_permutation_iterator;
    
    // sort_iterator
    
    typedef
    ::trsl::sort_iterator
    <typename Container::iterator>
    sort_iterator;
    
    typedef
    ::trsl::sort_iterator
    <typename Container::const_iterator>
    const_sort_iterator;

    // custom_permutation_iterator
    
    typedef
    ::trsl::custom_permutation_iterator
    <typename Container::iterator>
    custom_permutation_iterator;
    
    typedef
    ::trsl::custom_permutation_iterator
    <typename Container::const_iterator>
    const_custom_permutation_iterator;
  };
} // namespace trsl

#endif // include guard
