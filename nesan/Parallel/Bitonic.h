/**
 * @file Bitonic.h
 *
 * @brief Implements bitonic splitting and merging routines
 */

#pragma once

#include <algorithm>
#include <thread>
#include <vector>

namespace nesan
{
template <typename T>
void bitonicSplit(const std::vector<T> &data, std::vector<T> &min, std::vector<T> &max)
{
   size_t n = data.size();

   min = std::vector<T>(data.begin(), data.begin() + n/2);
   max = std::vector<T>(data.begin() + n/2, data.end());

   for (size_t i = 0; i < n/2; ++i)
   {
      auto minmax = std::minmax(min[i], max[i]);
      min[i] = minmax.first;
      max[i] = minmax.second;
   }
}

template <typename T>
std::vector<T> bitonicMerge(const std::vector<T> data)
{
   size_t n = data.size();

   if (n >= 2)
   {
      std::vector<T> a;
      std::vector<T> b;
      bitonicSplit(data, a, b);

      auto spawn = [&]()
      {
         a = bitonicMerge(a);
      };
      std::thread par(spawn);

      b = bitonicMerge(b);

      par.join();

      a.insert(a.end(), b.begin(), b.end());

      return a;
   }
   return data;
}
}
