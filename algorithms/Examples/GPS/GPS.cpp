/**
 * @file GPS.cpp
 *
 * @brief This is an oversimplified "model" of GPS data reporting to a satellite.
 * The satellite houses an alpha-beta filter and the GPS is just moving in a circular
 * pattern. The GPS also provides some uncertainty factor and uses that to add noise
 * to the raw measurement. The satellite reads in the uncertain data and applies the
 * filter to it.
 */

#include <array>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>

#include "Filters/AlphaBetaFilter.h"

using namespace nesan;

static const double PI = 4.0 * std::atan(1.0);
static const double RAD = 1.0 / (2.0 * PI);

class Satellite
{
public:
   void initialize(const std::array<double, 2> &initial)
   {
      ab.setState(initial);
   }

   void filterState(const std::array<double, 2> &signal)
   {
      ab.update(1.0, signal);
   }

   std::array<double, 2> getState() const
   {
      std::array<double, 2> state;
      ab.getState(state);
      return state;
   }

private:
   AlphaBetaFilter<double, 2> ab {0.45, 0.05};
};

class GPS
{
public:
   void update(const double dt)
   {
      time += dt;
      state[0] = std::cos(time * RAD / 5.0);
      state[1] = std::sin(time * RAD / 5.0);
   }

   std::array<double, 2> getRawState() const
   {
      return state;
   }

   std::array<double, 2> getState()
   {
      std::array<double, 2> s = state;
      double r = rdis(eng);
      double t = tdis(eng);
      s[0] += uncertainty * std::sqrt(r) * std::cos(t);
      s[1] += uncertainty * std::sqrt(r) * std::sin(t);
      return s;
   }

private:
   double uncertainty = 0.05;
   double time = 0.0;
   std::array<double, 2> state {1.0, 0.0};

   std::mt19937_64 eng;
   std::uniform_real_distribution<double> rdis {0.0, 1.0};
   std::uniform_real_distribution<double> tdis {0.0, 2.0 * PI};
};

int main()
{
   std::ofstream rawStateFile("raw_state.dat");
   std::ofstream uncertainStateFile("uncertain_state.dat");
   std::ofstream filteredStateFile("filtered_state.dat");

   rawStateFile << std::fixed << std::setprecision(8);
   uncertainStateFile << std::fixed << std::setprecision(8);
   filteredStateFile << std::fixed << std::setprecision(8);

   Satellite satellite;
   GPS gps;

   satellite.initialize(gps.getRawState());

   double time = 0.0;
   while (time < 200.0)
   {
      gps.update(1.0);

      std::array<double, 2> rawState = gps.getRawState();
      std::array<double, 2> uncertainState = gps.getState();

      satellite.filterState(uncertainState);

      std::array<double, 2> filteredState = satellite.getState();

      rawStateFile << rawState[0] << " " << rawState[1] << std::endl;
      uncertainStateFile << uncertainState[0] << " " << uncertainState[1] << std::endl;
      filteredStateFile << filteredState[0] << " " << filteredState[1] << std::endl;

      time += 1.0;
   }
}
