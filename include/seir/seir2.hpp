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
    double beta;  // average number of people infected by an infected in a day
    double alpha; // parameter: inverse of disease incubation period
    double gamma; // parameter: probability of recovery(or death) of an infected

  public:
    // constructor
    SEIR(int population, int time, State S0, double beta, double alpha, double gamma);

    // default constructor
    SEIR();

  private:
    // returns newly calculated State using Runge Kutta method(4th order numerical approximation)
    State RungeKuttaSolver(const State& oldState);

    // check for the validity of a SEIR object
    bool is_valid(SEIR obj);

  public:
    // performs the actual SEIRulation over the setted time interval
    void evolve(std::vector<State>& states);
};

} // namespace RK_4

#endif
