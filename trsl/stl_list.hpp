// (C) Copyright Renaud Detry   2008-2009.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_STL_LIST_HPP
#define TRSL_STL_LIST_HPP

#include <trsl/stl_container_iterator_factory.hpp>
#include <list>

namespace trsl
{
  template<class ElementType>
  struct stl_list : public stl_container_iterator_factory< std::list<ElementType> >
  {
    typedef stl_container_iterator_factory< std::list<ElementType> > super_t;

    // systematic_sample_iterator
    
    typedef
    typename super_t::systematic_sample_iterator
    systematic_sample_iterator;
    
    typedef
    typename super_t::const_systematic_sample_iterator
    const_systematic_sample_iterator;

    // random_permutation_iterator
    
    typedef
    typename super_t::random_permutation_iterator
    random_permutation_iterator;
    
    typedef
    typename super_t::const_random_permutation_iterator
    const_random_permutation_iterator;

    // sort_iterator
    
    typedef
    typename super_t::sort_iterator
    sort_iterator;
    
    typedef
    typename super_t::const_sort_iterator
    const_sort_iterator;

    // custom_permutation_iterator
    
    typedef
    typename super_t::custom_permutation_iterator
    custom_permutation_iterator;
    
    typedef
    typename super_t::const_custom_permutation_iterator
    const_custom_permutation_iterator;
  };
    
} // namespace trsl

#endif // include guard
