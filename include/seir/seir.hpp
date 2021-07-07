#ifndef SEIRODE_HPP
#define SEIRODE_HPP

#include <iostream>

namespace RK_4
{

int constexpr TIME_STEP = 1; // days

struct State
{
    double S;
    double E;
    double I;
    double R;

    // susceptibles change rate
    double dS_dt(double beta, int N) const;

    // exposed change rate
    double dE_dt(double beta, double alpha, int N) const;

    // infected change rate
    double dI_dt(double alpha, double gamma) const;

    // Recovered change rate
    double dR_dt(double gamma) const;
};

class SEIR
{
  private:
    int N;        // total number of individuals
    int t;        // duration time of the simualtion(days)
    State S_0;    // initial state
    double beta;  // number of people an infective person infects each day
    double alpha; // governs the lag between infectious contact and showing symptoms
    double gamma; // probability to recover or die (accounts for two parameters)

  public:
    // constructor
    SEIR(int population, int time, State S0, double beta, double alpha, double gamma);

    // default constructor
    SEIR();

  private:
    // returns newly calculated State using Euler method(1th order numerical approximation)
    State EulerSolver(const State& current_state);

    // returns newly calculated State using Runge Kutta method(4th order numerical approximation)
    State RungeKuttaSolver(const State& current_state);

    // check for the validity of a SEIR object
    bool is_valid(SEIR obj);

  public:
    // performs the actual simulation:determines the various states by solving the ode system
    // over the setted time interval either with Euler method (method == 0) or with RK4 (method == 1)
    void evolve(std::vector<State>& states, bool method = 0);
};

} // namespace RK_4

#endif
