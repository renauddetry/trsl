// (C) Copyright Renaud Detry   2007-2008.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/** @file */

#ifndef TRSL_PARTICLE_HPP
#define TRSL_PARTICLE_HPP

namespace trsl {
  namespace example {
  
    // Example element class.

    class Particle
    {
    public:
      Particle(double weight, double x, double y) :
        x_(x), y_(y), weight_(weight) {}
  
      void setWeight(const double weight) { weight_ = weight; }
      double getWeight() const { return weight_; };
  
      bool operator==(const Particle& p) const
        { return x_ == p.x_ && y_ == p.y_ && weight_ == p.weight_; }

      bool operator<(const Particle& p) const
        { return weight_ < p.weight_; }

      double getX() const { return x_; }
      double getY() const { return y_; }
      
    private:
      double x_;
      double y_;
      double weight_;
    };

  }
}

#endif // include guard
