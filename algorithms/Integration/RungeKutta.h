/**
 * @file RungeKutta.h
 *
 * @brief Implements a 4th-order Runge Kutta integrator class.
 */

#pragma once

namespace nesan
{
template <typename FuncType>
class RungeKutta
{
public:
   RungeKutta(FuncType function, double initial_condition, double step_size) :
      f(function),
      w(initial_condition),
      h(step_size),
      h02(h * 0.5),
      t(0.0)
   {}

   double integrate()
   {
      double k1 = h * f(t, w);
      double k2 = h * f(t + h02, w + k1 * 0.5);
      double k3 = h * f(t + h02, w + k2 * 0.5);
      double k4 = h * f(t + h, w + k3);
      w += (k1 + 2.0 * k2 + 2.0 * k3 + k4) / 6.0;
      t += h;
      return w;
   }

private:
   FuncType f;
   double w;
   double h;
   double h02;
   double t;
};

template <typename FuncType>
RungeKutta<FuncType> make_integrator(FuncType t, double initial_condition, double step_size)
{
   return RungeKutta<FuncType>(t, initial_condition, step_size);
}
}
