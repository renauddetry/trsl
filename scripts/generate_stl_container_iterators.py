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

#include <trsl/detail/stl_container_iterator_factory.hpp>
#include <%s>

namespace trsl
{
  /** @brief Defines convenience types */
  template<class ElementType>
  struct stl_%s : public detail::stl_container_iterator_factory< std::%s<ElementType> >
  {
    typedef detail::stl_container_iterator_factory< std::%s<ElementType> > super_t;
""" % (c.upper(), c.upper(), c.lower().replace("multi", ""),
       c.lower(), c.lower(), c.lower()))
  for i in [ "perm_syst_sample_iter",
             "random_reord_iter",
             "sorted_reord_iter",
             "custom_reord_iter" ]:
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
