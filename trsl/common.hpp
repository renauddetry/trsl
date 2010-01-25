// (C) Copyright Renaud Detry   2007-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_COMMON_HPP
#define TRSL_COMMON_HPP

#include <trsl/version.hpp> // version macros used to be defined here
#include <cstdlib>
#include <algorithm> //iter_swap

namespace trsl {
  
  namespace detail {
        
    template<typename RandomAccessIterator, typename RandomNumberGenerator>
    void partial_random_shuffle(RandomAccessIterator first,
                                RandomAccessIterator middle,
                                RandomAccessIterator last,
                                RandomNumberGenerator &rg)
    {
      if (first == middle)
        return;
        
      for (RandomAccessIterator i = first; i != middle; ++i)
      {
        std::iter_swap(i, i + rg(last - i));
      }
    }
    
  }
  
  /**
   * @brief Identity operator().
   *
   * This class implements a template operator() such that its instances
   * can be used as functors which return a const reference to their argument.
   */
  class identity
  {
  public:
    template<typename T>
    const T& operator() (const T& t)
    {
      return t;
    }
  };
  
  /** @brief Random number wrapper functions. */
  namespace rand_gen {

    /**
     * @brief Returns an integer in <tt>[0,n[</tt>.
     */
    inline unsigned int uniform_int(unsigned int n)
    {
#ifdef TRSL_USE_BSD_BETTER_RANDOM_GENERATOR
      return ::random()%n;
#else
      return std::rand()%n;
#endif
    }
  
    /**
     * @brief Returns a float in <tt>[0,1[</tt>.
     */
    template<typename Real>
    inline Real uniform_01()
    {
#ifdef TRSL_USE_BSD_BETTER_RANDOM_GENERATOR
      return (::random() / (RAND_MAX+Real(1.0)));
#else
      return (std::rand() / (RAND_MAX+Real(1.0)));
#endif
    }
    
  }
}

#endif // include guard
