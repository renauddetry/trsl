// (C) Copyright Renaud Detry   2007.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "trsl/is_picked_systematic.hpp"
#include "trsl/random_permutation_iterator.hpp"

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
  
//#define C_ARRAY_ITERATOR_SAMPLING
#define STD_VECTOR_ITERATOR_SAMPLING

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

  std::vector<float> population(100);
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
                      /* here, you may use a fancy Mersenne Twister
                         generated number, e.g. from boost or GSL */
                      rand()/(RAND_MAX+1.0),
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

  //-------------------------------------//
  // Get a permutation of the population //
  //-------------------------------------//

  typedef trsl::random_permutation_iterator
    <population_iterator> permutation_iterator;
  
  {
    permutation_iterator pi(populationIteratorBegin,
                            populationIteratorEnd);
    
    std::cout << "Permutation:" << std::endl;
    std::copy(pi,
              pi.end(),
              std::ostream_iterator<float>(std::cout, " "));
    std::cout << std::endl;
  }
  {
    permutation_iterator pi(populationIteratorBegin,
                            populationIteratorEnd,
                            SAMPLE_SIZE);
    
    std::cout << "Permutation of a subset of " << SAMPLE_SIZE
              << " elements:" << std::endl;
    std::copy(pi,
              pi.end(),
              std::ostream_iterator<float>(std::cout, " "));
    std::cout << std::endl;
  }
  return 0;
}
