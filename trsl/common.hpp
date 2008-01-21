// (C) Copyright Renaud Detry   2007.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_COMMON_HPP
#define TRSL_COMMON_HPP

#include <cstdlib>
#include <algorithm> //iter_swap

#define TRSL_VERSION "0.1.1 (dev)"

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
