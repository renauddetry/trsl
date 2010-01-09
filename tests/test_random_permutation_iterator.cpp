// (C) Copyright Renaud Detry   2007-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <trsl/random_permutation_iterator.hpp>
#include <tests/common.hpp>

using namespace trsl::test;

int main()
{
  // BSD has two different random generators
  unsigned long random_seed = time(NULL)*getpid();
  srandom(random_seed);
  srand(random_seed);
  
  typedef std::vector<PickCountParticle> ParticleArray;
  
  // ---------------------------------------------------- //
  // Test 1: large population --------------------------- //
  // ---------------------------------------------------- //
  {
    const size_t POPULATION_SIZE = 1000000;
    const size_t SAMPLE_SIZE = 1000;
    
    // Type definitions, once and for all.

    typedef trsl::random_permutation_iterator
      <ParticleArray::const_iterator> permutation_iterator;

    //-----------------------//
    // Generate a population //
    //-----------------------//
    
    ParticleArray population;
    generatePopulation(POPULATION_SIZE, population);
    ParticleArray const& const_pop = population;
    
    //-------------------------------------------------//
    // Test 1a: correct sample size, first constructor //
    //-------------------------------------------------//
    {
      ParticleArray sample;
      
      permutation_iterator sb(const_pop.begin(), const_pop.end());
      for (permutation_iterator si = sb,
             se = sb.end(); si != se; ++si)
      {
        sample.push_back(*si);
      }
      if (! (sample.size() == POPULATION_SIZE) )
      {
        TRSL_TEST_FAILURE;
        std::cout << TRSL_NVP(sample.size()) << "\n" << TRSL_NVP(POPULATION_SIZE) << std::endl;
      }
    }
    //--------------------------------------------------//
    // Test 1b: correct sample size, second constructor //
    //--------------------------------------------------//
    {
      ParticleArray sample;
      
      permutation_iterator sb(const_pop.begin(), const_pop.end(), SAMPLE_SIZE);
      for (permutation_iterator si = sb,
             se = sb.end(); si != se; ++si)
      {
        sample.push_back(*si);
      }
      if (! (sample.size() == SAMPLE_SIZE) )
      {
        TRSL_TEST_FAILURE;
        std::cout << TRSL_NVP(sample.size()) << "\n" << TRSL_NVP(SAMPLE_SIZE) << std::endl;
      }
    }
    //------------------------//
    // Test 1c: random access //
    //------------------------//
    {
      ParticleArray sample;
      
      permutation_iterator sb(const_pop.begin(), const_pop.end());
      for (permutation_iterator si = sb,
             se = sb.end(); si != se; ++si)
      {
        sample.push_back(*si);
      }
      if (! (sample.size() == POPULATION_SIZE) )
      {
        TRSL_TEST_FAILURE;
        std::cout << TRSL_NVP(sample.size()) << "\n" << TRSL_NVP(POPULATION_SIZE) << std::endl;
      }
      ParticleArray::iterator iter = sample.begin();
      for (unsigned i = 0; i < POPULATION_SIZE; i++)
      {
        if (! (*(sb + i) == *iter) )
        {
          TRSL_TEST_FAILURE;
        }
        iter++;
      }
    }
    
  }
  
  // ---------------------------------------------------- //
  // Test 2: small population --------------------------- //
  // ---------------------------------------------------- //
  {
    const size_t POPULATION_SIZE = 100;
    const size_t SAMPLE_SIZE = 5;
    
    // Type definitions, once and for all.

    typedef trsl::random_permutation_iterator
      <ParticleArray::iterator> permutation_iterator;

    //-----------------------//
    // Generate a population //
    //-----------------------//
    
    std::vector<PickCountParticle> population;
    generatePopulation(POPULATION_SIZE, population);
    
    //------------------------------------------------//
    // Test 2a: sampling coherency with probabilities //
    //------------------------------------------------//
    {
      // Test 2a checks that, after repeating sampling many times,
      // element pick proportions correspond to element weights.
      const unsigned N_ROUNDS = 1000000;
      unsigned pickCount = 0;
            
      //clock_t start = clock();
      for (unsigned round = 0; round < N_ROUNDS; round++)
      {        
        permutation_iterator sb(population.begin(),
                                population.end(),
                                SAMPLE_SIZE);
        permutation_iterator se = sb.end();
        for (permutation_iterator si = sb; si != se; ++si)
        {
          si->pick();
          pickCount++;
        }
      }
      //clock_t end = clock(); std::cerr << end-start << std::endl;
      if (! (pickCount == N_ROUNDS * SAMPLE_SIZE) )
      {
        TRSL_TEST_FAILURE;
        std::cout << TRSL_NVP(N_ROUNDS) << std::endl
                  << TRSL_NVP(SAMPLE_SIZE) << std::endl
                  << TRSL_NVP(pickCount) << std::endl;
      }
      double div = 0;
      for (std::vector<PickCountParticle>::iterator e = population.begin();
           e != population.end(); e++)
      {
        double pickProp = double(e->getPickCount()) / (N_ROUNDS * SAMPLE_SIZE);
        div += std::fabs(e->getWeight() - pickProp);
        if (! ( std::fabs(1 -
                          POPULATION_SIZE * pickProp) <= 1e-1) )
        {
          TRSL_TEST_FAILURE;
        }
        if (! ( std::fabs(1 -
                          POPULATION_SIZE * pickProp) <= 1e-1) ||
            TEST_VERBOSE > 0)
          std::cout << "Element " << std::distance(population.begin(), e)
                    << ": weight = " << int(100 * POPULATION_SIZE *
                                            e->getWeight()) << "%"
                    << ", pickp = " << int(100 * POPULATION_SIZE *
                                           pickProp) << "%"
                    << std::endl;
      }
      if (TEST_VERBOSE > 0)
        std::cout << TRSL_NVP(div) << std::endl;
    }
  }
  // ---------------------------------------------------- //
  // Test 3: types -------------------------------------- //
  // ---------------------------------------------------- //
  {
    // Test conversion to const
    trsl::random_permutation_iterator
    <int const *> i1 = trsl::random_permutation_iterator
    <int*>();
    
    // Test that begin/end are the same iterator type.
    // (Also tests empty range.)
    if (std::distance(i1.begin(), i1.end()) +
        std::distance(i1, i1.end()) != 0)
      TRSL_TEST_FAILURE;
  }
  return 0;
}
