/**
 * @file VectorUtilities.h
 *
 * @brief Contains utility functions for working with vectors.
 */

#pragma once

#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <type_traits>
#include <vector>

namespace nesan
{
template <typename T>
bool printVector(std::ostream &stream, const std::vector<T> &vec)
{
   for (auto element : vec)
   {
      stream << element << "\n";
   }
   return !stream.fail();
}

template <typename T,
          typename D = typename std::conditional<std::is_floating_point<T>::value,
                                                 std::uniform_real_distribution<T>,
                                                 std::uniform_int_distribution<T>>::type>
bool makeRandomVector(const size_t n, const T min, const T max, std::vector<T> &vec, bool isBitonic = false)
{
   if (n < 1)
   {
      std::cerr << n << " is not a valid number of elements!\n";
      return false;
   }

   if (min > max)
   {
      std::cerr << "The minimum value " << min << " is greater then the maximum " << max << "\n";
      return false;
   }

   std::mt19937 eng;
   D dis(min, max);
   vec.clear();
   vec.resize(n, 0.0);

   auto start = static_cast<size_t>(isBitonic);

   for (size_t i = start; i < n; ++i)
   {
      if (isBitonic)
      {
         T step = 4 * (max - min) / n;
         T back = vec[i-1];
         if (i < n/2 && back < max)
         {
            dis.param(typename D::param_type(back, std::min(back + step, max)));
         }
         else
         {
            dis.param(typename D::param_type(std::max(static_cast<T>(0), back - step), back));
         }
      }

      vec[i] = dis(eng);
   }

   return true;
}
}
