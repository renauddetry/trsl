// (C) Copyright Renaud Detry   2007-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//#define TRSL_USE_BSD_BETTER_RANDOM_GENERATOR
//#define TRSL_USE_SYSTEMATIC_INTUITIVE_ALGORITHM

#include <tests/common.hpp>
#include <trsl/systematic_sample_iterator.hpp>
#include <trsl/stl_list.hpp>
#include <trsl/stl_vector.hpp>
#include <trsl/stl_deque.hpp>
#include <trsl/stl_set.hpp>
#include <trsl/stl_multiset.hpp>

using namespace trsl::test;

int main()
{
  // BSD has two different random generators
  unsigned long random_seed = time(NULL)*getpid();
  srandom(random_seed);
  srand(random_seed);
  
  typedef std::list<PickCountParticle> ParticleArray;
  
  // ---------------------------------------------------- //
  // Test 1: large population --------------------------- //
  // ---------------------------------------------------- //
  {
    const size_t POPULATION_SIZE = 1000000;
    const size_t SAMPLE_SIZE = 1000;
    
    // Type definitions, once and for all.

    typedef trsl::systematic_sample_iterator
    <
      ParticleArray::const_iterator,
      std::const_mem_fun_ref_t<double, Particle>
    >
    sample_iterator;

    //-----------------------//
    // Generate a population //
    //-----------------------//
    
    ParticleArray population;
    generatePopulation(POPULATION_SIZE, population);
    ParticleArray const& const_pop = population;
    
    //------------------------------//
    // Test 1a: correct sample size //
    //------------------------------//
    {
      ParticleArray sample;
      
      sample_iterator si(const_pop.begin(), const_pop.end(),
                         SAMPLE_SIZE, 1.0,
                         std::mem_fun_ref(&Particle::getWeight));
      clock_t start = clock();
      for (; si != si.end(); ++si)
      {
        sample.push_back(*si);
      }
      clock_t end = clock(); std::cerr << end-start << std::endl;
      if (! (sample.size() == SAMPLE_SIZE) )
      {
        TRSL_TEST_FAILURE;
        std::cout << TRSL_NVP(sample.size()) << "\n" << TRSL_NVP(SAMPLE_SIZE)
          << std::endl;
      }
    }
    
    //-------------------------------//
    // Test 1b: deteministic picking //
    //-------------------------------//
    {
      ParticleArray sample;
      // Create the systemtatic sampling functor.

      boost::mt19937 rng((unsigned)random_seed);
      // Use a *copy* of rng
      boost::variate_generator<boost::mt19937, boost::uniform_int<> >
        uniform_int1(rng, boost::uniform_int<>()),
        uniform_int2(rng, boost::uniform_int<>());
      
      if ( !(&*sample_iterator(const_pop.begin(), const_pop.end(),
                               SAMPLE_SIZE, 1.0,
                               std::mem_fun_ref(&Particle::getWeight),
                               uniform_int1, .3) ==
             &*sample_iterator(const_pop.begin(), const_pop.end(),
                               SAMPLE_SIZE, 1.0,
                               std::mem_fun_ref(&Particle::getWeight),
                               uniform_int2, .3)) )
      {
        TRSL_TEST_FAILURE;
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

    typedef trsl::systematic_sample_iterator
    <
      std::vector<PickCountParticle>::iterator,
      std::const_mem_fun_ref_t<double, Particle>
    >
    sample_iterator;

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
      
      boost::mt19937 rng((unsigned)random_seed);
      boost::variate_generator<boost::mt19937&, boost::uniform_int<> >
        uniform_int(rng, boost::uniform_int<>());
      boost::variate_generator<boost::mt19937&, boost::uniform_real<> >
        uniform_01(rng, boost::uniform_real<>());
      
      clock_t start = clock();
      for (unsigned round = 0; round < N_ROUNDS; round++)
      {        
        for (sample_iterator si(population.begin(), population.end(),
                                SAMPLE_SIZE, 1.0,
                                std::mem_fun_ref(&Particle::getWeight),
                                uniform_int,
                                uniform_01());
             si != si.end(); ++si)
        {
          si->pick();
          pickCount++;
        }
      }
      clock_t end = clock(); std::cerr << end-start << std::endl;
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
        if (! ( std::fabs(POPULATION_SIZE * e->getWeight() -
                          POPULATION_SIZE * pickProp) <= 1e-1) )
        {
          TRSL_TEST_FAILURE;
        }
        if (! ( std::fabs(POPULATION_SIZE * e->getWeight() -
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
  // Test 3: empty sample ------------------------------- //
  // ---------------------------------------------------- //
  {
    const size_t POPULATION_SIZE = 1000;
    const size_t SAMPLE_SIZE = 0;
    
    // Type definitions, once and for all.

    typedef trsl::systematic_sample_iterator
    <
      ParticleArray::const_iterator,
      std::const_mem_fun_ref_t<double, Particle>
    >
    sample_iterator;
    
    //-----------------------//
    // Generate a population //
    //-----------------------//
    
    ParticleArray population;
    generatePopulation(POPULATION_SIZE, population);
    ParticleArray const& const_pop = population;
    
    //------------------------------//
    // Test 3a: correct sample size //
    //------------------------------//
    {
      ParticleArray sample;
      
      sample_iterator sb =
        sample_iterator(const_pop.begin(), const_pop.end(),
                        SAMPLE_SIZE, 1.0,
                        std::mem_fun_ref(&Particle::getWeight));
      if (sb != sb.end())
      {
        TRSL_TEST_FAILURE;
      }
    }
  }

  // ---------------------------------------------------- //
  // Test 4: sample larger than population -------------- //
  // ---------------------------------------------------- //
  {
    const size_t POPULATION_SIZE = 8000;
    const size_t SAMPLE_SIZE = 10000;
    
    // Type definitions, once and for all.

    typedef trsl::systematic_sample_iterator
    <
      ParticleArray::const_iterator,
      std::const_mem_fun_ref_t<double, Particle>
    >
    sample_iterator;
    
    //-----------------------//
    // Generate a population //
    //-----------------------//
    
    ParticleArray population;
    generatePopulation(POPULATION_SIZE, population);
    ParticleArray const& const_pop = population;
    
    //----------------------------//
    // Test 4a: iterator equality //
    //----------------------------//
    {
      ParticleArray sample;
      
      sample_iterator sb =
        sample_iterator(const_pop.begin(), const_pop.end(),
                        SAMPLE_SIZE, 1.0,
                        std::mem_fun_ref(&Particle::getWeight));
      sample_iterator se = sb.end();
      sample_iterator sp = sb;
      
      if (sb == se)
      {
        std::cout << "Sample of at least one element needed for this test." << std::endl;
        TRSL_TEST_FAILURE;
      }
      
      int duplicates = 0;
      for (sample_iterator si = ++sb; si != se; ++si)
      {
        if (si == sp)
          TRSL_TEST_FAILURE;
        
        if (&*si == &*sp)
        {
          duplicates++;
          if (si.base().base() != sp.base().base())
            TRSL_TEST_FAILURE;
        }
        else
        {
          if (si.base().base() == sp.base().base())
            TRSL_TEST_FAILURE;
        }
        
        sp = si;
      }
      if (duplicates == 0)
        TRSL_TEST_FAILURE;
    }

    //------------------------//
    // Test 4b: is_first_pick //
    //------------------------//
    {
      ParticleArray sample;
      
      sample_iterator sb =
        sample_iterator(const_pop.begin(), const_pop.end(),
                        SAMPLE_SIZE, 1.0,
                        std::mem_fun_ref(&Particle::getWeight));
      sample_iterator previous = sb;
      sample_iterator se = sb.end();
      for (sample_iterator
             si = sb,
             previous = sb; si != se; previous = si++)
      {
        if ( (si != previous && si.base().base() == previous.base().base()) ==
              is_first_pick(si) )
          TRSL_TEST_FAILURE;
      }
    }
  }
  // ---------------------------------------------------- //
  // Test 5: types -------------------------------------- //
  // ---------------------------------------------------- //
  {
    // Test conversion to const
    trsl::systematic_sample_iterator
    <int const *, trsl::unit_weight_accessor<double> > i1 = trsl::systematic_sample_iterator
    <int*, trsl::unit_weight_accessor<double> >();
    
    // Test that begin/end are the same iterator type.
    // (Also tests empty range.)
    if (std::distance(i1.begin(), i1.end()) +
        std::distance(i1, i1.end()) != 0)
      TRSL_TEST_FAILURE;
  }
  return 0;
}
