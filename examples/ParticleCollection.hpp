// (C) Copyright Renaud Detry   2007-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_PARTICLECOLLECTION_HPP
#define TRSL_PARTICLECOLLECTION_HPP

#include <vector>
#include <trsl/systematic_sample_iterator.hpp>
#include <examples/Particle.hpp>

namespace trsl {
  namespace example {
  

    // Example population class

    class ParticleCollection
    {
    public:
      typedef trsl::systematic_sample_iterator<
        std::vector<Particle>::iterator,
        std::const_mem_fun_ref_t<double, Particle>
      > sample_iterator;
      typedef trsl::systematic_sample_iterator<
        std::vector<Particle>::const_iterator,
        std::const_mem_fun_ref_t<double, Particle>
      > const_sample_iterator;

      ParticleCollection(): totalWeight_(0) {}
  
      void add(const Particle& p)
        {
          totalWeight_ += p.getWeight();
          particles_.push_back(p);
        }
  
      size_t size() const { return particles_.size(); }
  
      sample_iterator sample_begin(size_t sampleSize)
        {
          return sample_iterator(particles_.begin(), particles_.end(),
                                 sampleSize, totalWeight_,
                                 std::mem_fun_ref(&Particle::getWeight));
        }

      const_sample_iterator sample_begin(size_t sampleSize) const
        {
          return const_sample_iterator(particles_.begin(), particles_.end(),
                                       sampleSize, totalWeight_,
                                       std::mem_fun_ref(&Particle::getWeight));
        }

    private:
      std::vector<Particle> particles_;
      double totalWeight_;
    };

  }
}

#endif // include guard
