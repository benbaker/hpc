#if ! defined rvec
#define rvec 1

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

std::vector<float> generateRandomVector(unsigned int size) {
   std::vector<float> rv(size, 0);
   for (std::vector<float>::iterator i=rv.begin(); i!=rv.end(); ++i)
      *i =  ((static_cast<float>(rand()) / RAND_MAX) * 2.0) - 1.0;

   return rv;
}

#endif