// (C) Copyright Renaud Detry   2007-2011.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//#define TRSL_USE_BSD_BETTER_RANDOM_GENERATOR
//#define TRSL_USE_SYSTEMATIC_INTUITIVE_ALGORITHM

#include <trsl/sort_iterator.hpp>
#include <trsl/is_picked_systematic.hpp>
#include <trsl/random_permutation_iterator.hpp>
#include <trsl/ppfilter_iterator.hpp>

#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <vector>
#include <iostream>
#include <numeric> // accumulate

// In this example, population elements are floats, and an element's
// weight is the element itself. The weight accessor is:
double wac(float x)
{
  return x;
}

int main()
{

  //-----------------------//
  // Generate a population //
  //-----------------------//
  
#define C_ARRAY_ITERATOR_SAMPLING
//#define STD_VECTOR_ITERATOR_SAMPLING

#if defined(C_ARRAY_ITERATOR_SAMPLING)
  float population[] = { 0, 1, 4, 3, 5, 8, 2 };
  const size_t POPULATION_SIZE = sizeof(population)/sizeof(float);
  const size_t SAMPLE_SIZE = 3;
  
  typedef float* population_iterator;
  population_iterator populationIteratorBegin(population),
    populationIteratorEnd(population + POPULATION_SIZE);
#elif defined(STD_VECTOR_ITERATOR_SAMPLING)
  boost::mt19937 rng;
  boost::uniform_real<float> dist(0, 100);
  boost::variate_generator<
    boost::mt19937&,
    boost::uniform_real<float>
    > uni(rng, dist);

  const size_t POPULATION_SIZE = 100;
  std::vector<float> population(POPULATION_SIZE);
  std::generate(population.begin(), population.end(), uni);
  const size_t SAMPLE_SIZE = 10;
  
  typedef std::vector<float>::iterator population_iterator;
  population_iterator populationIteratorBegin = population.begin(),
    populationIteratorEnd = population.end();
#else 
#  error Please choose an example.
#endif

  std::cout << "Population ("
            << std::distance(populationIteratorBegin,
                             populationIteratorEnd)
            << " elements):" << std::endl;
  std::copy(populationIteratorBegin,
            populationIteratorEnd,
            std::ostream_iterator<float>(std::cout, " "));
  std::cout << std::endl;

  {
    //----------------------------//
    // Sample from the population //
    //----------------------------//

    typedef trsl::is_picked_systematic<
      float,
      double,
      std::pointer_to_unary_function<float, double>
      > is_picked;
  
    typedef trsl::persistent_filter_iterator
      <is_picked, population_iterator> sample_iterator;
  
    is_picked predicate(SAMPLE_SIZE,
                        std::accumulate(populationIteratorBegin,
                                        populationIteratorEnd,
                                        float(0)),
                        std::ptr_fun(wac));
  
    sample_iterator sampleIteratorBegin(predicate,
                                        populationIteratorBegin,
                                        populationIteratorEnd);
    sample_iterator sampleIteratorEnd(predicate,
                                      populationIteratorEnd,
                                      populationIteratorEnd);
  
    std::cout << "Sample of " << SAMPLE_SIZE << " elements:" << std::endl;
    std::copy(sampleIteratorBegin,
              sampleIteratorEnd,
              std::ostream_iterator<float>(std::cout, " "));
    std::cout << std::endl;
  }

  {
    //-------------------------------------------------------------------//
    // Sample from the population (robust to patterns in the population) //
    //-------------------------------------------------------------------//
    
    typedef trsl::is_picked_systematic<
    float,
    double,
    std::pointer_to_unary_function<float, double>
    > is_picked;
    
    typedef trsl::ppfilter_iterator
    <is_picked, population_iterator> sample_iterator;
    
    is_picked predicate(SAMPLE_SIZE,
                        std::accumulate(populationIteratorBegin,
                                        populationIteratorEnd,
                                        float(0)),
                        std::ptr_fun(wac));
    
    sample_iterator sampleIteratorBegin(predicate,
                                        populationIteratorBegin,
                                        populationIteratorEnd);
    sample_iterator sampleIteratorEnd = sampleIteratorBegin.end();
    
    std::cout << "Probabilistic sample of " << SAMPLE_SIZE << " elements:" << std::endl;
    std::copy(sampleIteratorBegin,
              sampleIteratorEnd,
              std::ostream_iterator<float>(std::cout, " "));
    std::cout << std::endl;
  }
  
  {
    //-------------------------------------//
    // Get a permutation of the population //
    //-------------------------------------//

    typedef trsl::reorder_iterator
      <population_iterator> permutation_iterator;
  
    {
      permutation_iterator pi =
        trsl::random_permutation_iterator(populationIteratorBegin,
                                          populationIteratorEnd);
      
      std::cout << "Permutation:" << std::endl;
      std::copy(pi,
                pi.end(),
                std::ostream_iterator<float>(std::cout, " "));
      std::cout << std::endl;
    }
    {
      permutation_iterator pi =
        trsl::random_permutation_iterator(populationIteratorBegin,
                                          populationIteratorEnd,
                                          SAMPLE_SIZE);
      
      std::cout << "Permutation of a subset of " << SAMPLE_SIZE
                << " elements:" << std::endl;
      std::copy(pi,
                pi.end(),
                std::ostream_iterator<float>(std::cout, " "));
      std::cout << std::endl;
    }
  }

  {
    //---------------------//
    // Sort the population //
    //---------------------//

    typedef trsl::reorder_iterator
      <population_iterator> permutation_iterator;
  
    {
      permutation_iterator pi =
        trsl::sort_iterator(populationIteratorBegin,
                            populationIteratorEnd);
      
      std::cout << "Sorted population:" << std::endl;
      std::copy(pi,
                pi.end(),
                std::ostream_iterator<float>(std::cout, " "));
      std::cout << std::endl;
    }
    {
      permutation_iterator pi =
        trsl::sort_iterator(populationIteratorBegin,
                            populationIteratorEnd,
                            std::less<float>(), SAMPLE_SIZE);
      
      std::cout << "Sorted " << SAMPLE_SIZE
                << " smallest elements:" << std::endl;
      std::copy(pi,
                pi.end(),
                std::ostream_iterator<float>(std::cout, " "));
      std::cout << std::endl;
    }
  }

  return 0;
}
