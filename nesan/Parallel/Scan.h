/**
 * @file Scan.h
 *
 * @brief Implements a scan routine
 */

#include <algorithm>
#include <functional>
#include <thread>
#include <vector>

namespace nesan
{
template <typename T, typename Op>
bool scan(Op op, std::vector<T> &vec)
{
   size_t n = vec.size();
   if (n == 1)
   {
      return true;
   }

   for (size_t i = 0; i < n / 2; i++)
   {
      vec[i*2 + 1] = op(vec[i*2], vec[i*2 + 1]);
   }

   bool toggle = false;
   std::vector<T> e;
   std::vector<T> o;

   std::partition_copy(std::begin(vec), std::end(vec),
                       std::back_inserter(e),
                       std::back_inserter(o),
                       [&toggle](int){return toggle = !toggle;});

   auto spawn = [&]()
   {
      scan<T,Op>(op, o);
      for (size_t i = 0; i < o.size(); i++)
      {
         vec[2*i + 1] = o[i];
      }
   };

   std::thread par(spawn);

   for (size_t i = 1; i < n / 2; i++)
   {
      vec[i*2] = op(o[i-1], e[i]);
   }

   par.join();

   return true;
}
}
