// (C) Copyright Renaud Detry   2007-2011.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_COMMON_HPP
#define TRSL_COMMON_HPP

#include <cstdlib>
#include <algorithm> //iter_swap

/**
 * @brief Code version string.
 *
 * Defined as <em>major</em>.<em>minor</em>.<em>bugfix</em>.
 */
#define TRSL_VERSION "0.2.2"

/**
 * @brief Code version number.
 *
 * Defined as 1<em>MMmmbbii</em>, where <em>MM</em> is the major release number, <em>mm</em> is the minor
 * release number, <em>bb</em> is the bug-fix release number, and <em>ii</em> is the internal release number.
 */
#define TRSL_VERSION_NR 100020200

namespace trsl {
  
  /** @brief Implementation details. */
  namespace detail {
    
    template<typename T>
    class identity
    {
    public:
      T operator() (const T& t)
        {
          return t;
        }
    };
    
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
  
  /** @brief Random number wrapper functions. */
  namespace rand_gen {

    /**
     * @brief Returns an integer in <tt>[0,n[</tt>.
     * Used internally.
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
     * Used internally.
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
