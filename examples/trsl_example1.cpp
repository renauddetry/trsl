// (C) Copyright Renaud Detry   2007-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <trsl/systematic_sample_iterator.hpp>
#include <examples/Particle.hpp>

#include <vector>
#include <list>
#include <iostream>
#include <numeric> // accumulate
#include <cassert>

// Type definitions, once and for all.

using namespace trsl::example;

typedef trsl::systematic_sample_iterator
<
  std::list<Particle>::const_iterator,
  std::const_mem_fun_ref_t<double, Particle>
>
sample_iterator;

int main()
{
  const size_t POPULATION_SIZE = 100;
  const size_t SAMPLE_SIZE = 10;

  //-----------------------//
  // Generate a population //
  //-----------------------//
  
  std::list<Particle> population;
  double totalWeight = 0;
  for (size_t i = 0; i < POPULATION_SIZE; ++i)
  {
    Particle p(double(rand())/RAND_MAX,  // weight
               double(rand())/RAND_MAX,  // position (x)
               double(rand())/RAND_MAX); // position (y)
    totalWeight += p.getWeight();
    population.push_back(p);
  }
  // Normalize total weight.
  for (std::list<Particle>::iterator i = population.begin();
       i != population.end(); ++i)
    i->setWeight(i->getWeight()/totalWeight);
  
  std::list<Particle> const& const_pop = population;
  
  //----------------------------//
  // Sample from the population //
  //----------------------------//
  
  std::list<Particle> sample;

  std::cout << "Mean weight: " << 1.0/POPULATION_SIZE << std::endl;
  for (sample_iterator
         sb = sample_iterator(const_pop.begin(), const_pop.end(),
                              SAMPLE_SIZE, 1.0,
                              std::mem_fun_ref(&Particle::getWeight)),
         si = sb;
       si != si.end(); ++si)
  {
    std::cout << "sample_" << std::distance(sb, si) << "'s weight = " <<
      si->getWeight() << std::endl;

    Particle p = *si;
    p.setWeight(1);
    sample.push_back(p);

    // ... or do something else with *si ...
  }
  assert(sample.size() == SAMPLE_SIZE);
  return 0;
}
