// (C) Copyright Renaud Detry   2008-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_STL_DEQUE_HPP
#define TRSL_STL_DEQUE_HPP

#include <trsl/detail/stl_container_iterator_factory.hpp>
#include <deque>

namespace trsl
{
  /** @brief Defines convenience types */
  template<class ElementType>
  struct stl_deque : public detail::stl_container_iterator_factory< std::deque<ElementType> >
  {
    typedef detail::stl_container_iterator_factory< std::deque<ElementType> > super_t;

    // perm_syst_sample_iter
    
    typedef
    typename super_t::perm_syst_sample_iter
    perm_syst_sample_iter;
    
    typedef
    typename super_t::const_perm_syst_sample_iter
    const_perm_syst_sample_iter;

    // random_reord_iter
    
    typedef
    typename super_t::random_reord_iter
    random_reord_iter;
    
    typedef
    typename super_t::const_random_reord_iter
    const_random_reord_iter;

    // sorted_reord_iter
    
    typedef
    typename super_t::sorted_reord_iter
    sorted_reord_iter;
    
    typedef
    typename super_t::const_sorted_reord_iter
    const_sorted_reord_iter;

    // custom_reord_iter
    
    typedef
    typename super_t::custom_reord_iter
    custom_reord_iter;
    
    typedef
    typename super_t::const_custom_reord_iter
    const_custom_reord_iter;
  };
    
} // namespace trsl

#endif // include guard
