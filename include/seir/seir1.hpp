#ifndef SEIR_HPP
#define SEIR_HPP

#include <vector>

namespace Euler
{

struct State
{
    double S;
    double E;
    double I;
    double R;
    int N;
};

class SEIR
{
  private:
    State state0; // initial State
    double alpha; // inverse of incubation period
    double beta;  // average number of people infected by an infected in a day
    double gamma; // probability of recovery(or death) of an infected
  public:
    SEIR(State const& initial_state, double a, double b, double c);
    SEIR();
    bool is_valid();
    void evolve(std::vector<State>& states, int time_in_days);
};

} // namespace Euler

#endif