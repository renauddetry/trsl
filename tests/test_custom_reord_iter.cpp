// (C) Copyright Renaud Detry   2007-2010.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <trsl/custom_reord_iter.hpp>
#include <tests/common.hpp>

using namespace trsl::test;

int main()
{
  // BSD has two different random generators
  unsigned long random_seed = time(NULL)*getpid();
  srandom(random_seed);
  srand(random_seed);

  typedef
  trsl::custom_reord_iter< std::vector<unsigned>::const_iterator >
  permutation_iterator;
  
  std::vector<unsigned> inputRange;
  for (unsigned i = 0; i < 100; ++i) inputRange.push_back(i);
  
  permutation_iterator::position_container_ptr
    pcp(new permutation_iterator::position_container);
  
  pcp->push_back(3);
  pcp->push_back(10);
  pcp->push_back(35);
  pcp->push_back(57);
  pcp->push_back(60);
  pcp->push_back(99);
  
  permutation_iterator pi(inputRange.begin(), pcp);
  permutation_iterator::position_container::const_iterator ci(pcp->begin());
  for (; pi != pi.end(); ++pi, ++ci)
  {
    if (ci == pcp->end())
      TRSL_TEST_FAILURE;
    if (*pi != *ci)
      TRSL_TEST_FAILURE;
  }
  // ---------------------------------------------------- //
  // Test 1: types -------------------------------------- //
  // ---------------------------------------------------- //
  {
    // Test conversion to const
    trsl::custom_reord_iter
    <int const *> i1 = trsl::custom_reord_iter
    <int*>();
    
    // Test that begin/end are the same iterator type.
    // (Also tests empty range.)
    if (std::distance(i1.begin(), i1.end()) +
        std::distance(i1, i1.end()) != 0)
      TRSL_TEST_FAILURE;
  }
  return 0;
}
