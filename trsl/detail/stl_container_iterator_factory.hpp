// (C) Copyright Renaud Detry   2008-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef TRSL_STL_CONTAINER_ITERATOR_FACTORY_HPP
#define TRSL_STL_CONTAINER_ITERATOR_FACTORY_HPP

#include <trsl/perm_syst_sample_iter.hpp>
#include <trsl/random_reord_iter.hpp>
#include <trsl/sorted_reord_iter.hpp>
#include <trsl/custom_reord_iter.hpp>

namespace trsl
{
  namespace detail {
    
    template<class Container>
    struct stl_container_iterator_factory
    {    
      // perm_syst_sample_iter
      
      typedef
      ::trsl::perm_syst_sample_iter
      <
      typename Container::iterator,
      unit_weight_accessor<>
      >
      perm_syst_sample_iter;
      
      typedef
      ::trsl::perm_syst_sample_iter
      <
      typename Container::const_iterator,
      unit_weight_accessor<>
      >
      const_perm_syst_sample_iter;
      
      // random_reord_iter
      
      typedef
      ::trsl::random_reord_iter
      <typename Container::iterator>
      random_reord_iter;
      
      typedef
      ::trsl::random_reord_iter
      <typename Container::const_iterator>
      const_random_reord_iter;
      
      // sorted_reord_iter
      
      typedef
      ::trsl::sorted_reord_iter
      <typename Container::iterator>
      sorted_reord_iter;
      
      typedef
      ::trsl::sorted_reord_iter
      <typename Container::const_iterator>
      const_sorted_reord_iter;
      
      // custom_reord_iter
      
      typedef
      ::trsl::custom_reord_iter
      <typename Container::iterator>
      custom_reord_iter;
      
      typedef
      ::trsl::custom_reord_iter
      <typename Container::const_iterator>
      const_custom_reord_iter;
    };
  }
} // namespace trsl

#endif // include guard
