// (C) Copyright Renaud Detry   2007.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "trsl/is_picked_systematic.hpp"
#include "examples/Particle.hpp"

#include <list>
#include <iostream>
#include <numeric> // accumulate
#include <cassert>

// Type definitions, once and for all.

using namespace trsl::example;

typedef trsl::is_picked_systematic<Particle> is_picked;

typedef trsl::persistent_filter_iterator<
  is_picked, std::list<Particle>::const_iterator
> sample_iterator;

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
  // Create the systemtatic sampling functor.
  is_picked predicate(SAMPLE_SIZE, 1.0, &Particle::getWeight);

  std::cout << "Mean weight: " << 1.0/POPULATION_SIZE << std::endl;
  for (sample_iterator
         sb = sample_iterator(predicate, const_pop.begin(), const_pop.end()),
         si = sb,
         se = sample_iterator(predicate, const_pop.end(),   const_pop.end());
       si != se; ++si)
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
