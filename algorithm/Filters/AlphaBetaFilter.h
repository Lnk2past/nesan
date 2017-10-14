/**
 * @file AlphaBetaFilter.h
 *
 * @brief Implements a simple alpha-beta filter.
 */

#pragma once

#include <array>

namespace nesan
{
template <typename T, int N>
class AlphaBetaFilter
{
public:
   AlphaBetaFilter(const T a, const T b):
      alpha(a),
      beta(b),
      position(),
      velocity()
   {
      position.fill(0);
      velocity.fill(0);
   }

   template<typename Container>
   void update(const T dt, Container &signal)
   {
      for (int d = 0; d < N; ++d)
      {
         position[d] += velocity[d] * dt;
         T r = signal[d] - position[d];
         position[d] += alpha * r;
         velocity[d] += beta * r / dt;
      }
   }

   void getState(std::array<T, N> &position_out,
                 std::array<T, N> &velocity_out) const
   {
      position_out = position;
      velocity_out = velocity;
   }

private:
   T alpha = 0.0;
   T beta = 0.0;

   std::array<T, N> position;
   std::array<T, N> velocity;
};
}
