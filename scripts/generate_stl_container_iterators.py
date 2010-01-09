#!/usr/bin/env python


for c in [ "deque", "list", "vector",
           "set", "multiset" ]:
  f = open("trsl/stl_%s.hpp" % c.lower(), 'w')
  f.write("""// (C) Copyright Renaud Detry   2008-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_STL_%s_HPP
#define TRSL_STL_%s_HPP

#include <trsl/stl_container_iterator_factory.hpp>
#include <%s>

namespace trsl
{
  template<class ElementType>
  struct stl_%s : public stl_container_iterator_factory< std::%s<ElementType> >
  {
    typedef stl_container_iterator_factory< std::%s<ElementType> > super_t;
""" % (c.upper(), c.upper(), c.lower().replace("multi", ""),
       c.lower(), c.lower(), c.lower()))
  for i in [ "systematic_sample_iterator",
             "random_permutation_iterator",
             "sort_iterator",
             "custom_permutation_iterator" ]:
    f.write("""
    // %s
    
    typedef
    typename super_t::%s
    %s;
    
    typedef
    typename super_t::const_%s
    const_%s;
""" % (i, i, i, i, i))
  f.write("""  };
    
} // namespace trsl

#endif // include guard
""")
