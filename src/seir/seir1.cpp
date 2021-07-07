#include "seir1.hpp"

namespace Euler
{

//////////////////////////////////////////////
///////        SEIR CONSTRUCTOR         //////
//////////////////////////////////////////////
SEIR::SEIR(const State& initial_state, double alpha, double beta, double gamma)
    : state0{initial_state},
      alpha{alpha},
      beta{beta},
      gamma{gamma}
{
}

// default values
const SEIR& default_SEIR()
{
    State df{99999, 0, 1, 0, 100000};
    static SEIR def{df, 0.5, 0.7, 0.2};
    return def;
}

//////////////////////////////////////////////
///////        SEIR CONSTRUCTOR         //////
//////////////////////////////////////////////
SEIR::SEIR()
    : state0{default_SEIR().state0},
      alpha{default_SEIR().alpha},
      beta{default_SEIR().beta},
      gamma{default_SEIR().gamma}
{
}

///////////////// EVOLVE /////////////////
void SEIR::evolve(std::vector<State>& states, int time_in_days)
{
    assert(states.empty());
    states.reserve(time_in_days);

    states.push_back(state0); // set the first state according to initial conditions
    for (int i = 1; i < time_in_days; ++i)
    {
        State prev = states.back(); // previous State
        State new_state{};
        new_state.S = prev.S - (beta * (prev.I) * (prev.S)) / (prev.N);
        new_state.E = prev.E + (beta * (prev.I) * (prev.S)) / (prev.N) - alpha * (prev.E);
        new_state.I = prev.I + alpha * (prev.E) - gamma * (prev.I);
        new_state.R = prev.R + gamma * (prev.I);
        new_state.N = prev.N;
        states.push_back(new_state);
    }
}

bool SEIR::is_valid()
{
}

} // namespace Euler