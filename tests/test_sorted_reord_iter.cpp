// (C) Copyright Renaud Detry   2007-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <trsl/sorted_reord_iter.hpp>
#include <tests/common.hpp>

using namespace trsl::test;

class ParticleXComparator
{
public:
  bool operator() (const PickCountParticle& p1, const PickCountParticle& p2) const
    {
      return p1.getX() < p2.getX();
    }
};

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
    
    typedef trsl::sorted_reord_iter
      <ParticleArray::const_iterator> permutation_iterator;
    
    //-----------------------//
    // Generate a population //
    //-----------------------//
    
    ParticleArray population;
    generatePopulation(POPULATION_SIZE, population);
    ParticleArray const& const_pop = population;
    
    //----------------------------//
    // Test 1a: first constructor //
    //----------------------------//
    {
      ParticleArray sample;
      
      permutation_iterator sb(const_pop.begin(),
                              const_pop.end(),
                              trsl::same_size,
                              std::less<PickCountParticle>());
      for (permutation_iterator si = sb,
             se = sb.end(); si != se; ++si)
      {
        if (!sample.empty())
        {
          if (sample.back().getWeight() > si->getWeight())
          {
            TRSL_TEST_FAILURE;
          }
        }

        sample.push_back(*si);
      }
      if (! (sample.size() == POPULATION_SIZE) )
      {
        TRSL_TEST_FAILURE;
        std::cout << TRSL_NVP(sample.size()) << "\n" << TRSL_NVP(POPULATION_SIZE) << std::endl;
      }
    }
    //-----------------------------//
    // Test 1b: second constructor //
    //-----------------------------//
    {
      ParticleArray sample;
      
      permutation_iterator sb(const_pop.begin(),
                              const_pop.end(),
                              SAMPLE_SIZE,
                              std::less<PickCountParticle>());
      for (permutation_iterator si = sb,
             se = sb.end(); si != se; ++si)
      {
        if (!sample.empty())
        {
          if (sample.back().getWeight() > si->getWeight())
          {
            TRSL_TEST_FAILURE;
          }
        }

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
      for (unsigned i = 0; i < POPULATION_SIZE; i++)
      {
        if (! (*(sb + i) == sample.at(i)) )
        {
          TRSL_TEST_FAILURE;
        }
      }
    }
    //----------------------------//
    // Test 1d: custom comparator //
    //----------------------------//
    {
      ParticleArray sample;
      
      permutation_iterator sb(const_pop.begin(),
                              const_pop.end(),
                              trsl::same_size,
                              ParticleXComparator());
      for (permutation_iterator si = sb,
             se = sb.end(); si != se; ++si)
      {
        if (!sample.empty())
        {
          if (sample.back().getX() > si->getX())
          {
            TRSL_TEST_FAILURE;
          }
        }

        sample.push_back(*si);
      }
      if (! (sample.size() == POPULATION_SIZE) )
      {
        TRSL_TEST_FAILURE;
        std::cout << TRSL_NVP(sample.size()) << "\n" << TRSL_NVP(POPULATION_SIZE) << std::endl;
      }
    }
    
  }
  // ---------------------------------------------------- //
  // Test 2: types -------------------------------------- //
  // ---------------------------------------------------- //
  {
    // Test conversion to const
    trsl::sorted_reord_iter
    <int const *> i1 = trsl::sorted_reord_iter
    <int*>();

    // Test that begin/end are the same iterator type.
    // (Also tests empty range.)
    if (std::distance(i1.begin(), i1.end()) +
        std::distance(i1, i1.end()) != 0)
      TRSL_TEST_FAILURE;
  }
  return 0;
}
