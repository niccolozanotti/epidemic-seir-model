#include "seir2.hpp"
#include <array>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace RK_4
{

void error(std::string s)
{
    throw std::runtime_error(s);
}

//////////////////////////////////////////////
///////        SEIR CONSTRUCTOR         //////
//////////////////////////////////////////////
SEIR::SEIR(int population, int time, State initial_state, double beta, double alpha, double gamma)
    : N{population},
      t{time},
      S_0{initial_state},
      beta{beta},
      alpha{alpha},
      gamma{gamma}
{
    is_valid(*this);
}
/////////////////////////////////////////////////////////////////
///  DEFAULT CONDITIONS : POPULATION: 100000,TIME: 80 DAYS, 1 ///
///   INFECTED, BETA = 0.7, ALPHA = 0.5 , GAMMA = 0.2        ////
/////////////////////////////////////////////////////////////////
const SEIR& default_SEIR()
{
    State df{99999, 0, 1, 0};
    static SEIR def{100000, 80, df, 0.7, 0.5, 0.2};
    return def;
}
///////////////////////////////////////////////////////////////
//                   DEFAULT CONSTRUCTOR                     //
///////////////////////////////////////////////////////////////
SEIR::SEIR()
    // default constructor:CHECK IF IT MAKES SENSE
    : N{default_SEIR().N},
      t{default_SEIR().t},
      S_0{default_SEIR().S_0},
      beta{default_SEIR().beta},
      alpha{default_SEIR().alpha},
      gamma{default_SEIR().gamma}
{
}

// argument checking in object construction
bool SEIR::is_valid(SEIR obj)
{
    if (obj.t <= 0)
    {
        error("The SEIRulation duration have a non-positive value!");
        return false;
    }
    else if (obj.t < 10)
    {
        error("The simulation period is too small for the SEIRulation to be "
              "accurate!");
        return false;
    }
    if (obj.N <= 0)
    {
        error("The population has a non-positive value!");
        return false;
    }
    else if (obj.N < 100)
    {
        error("The population sample is too small for the SEIRulation to be "
              "accurate!");
        return false;
    }
    if (obj.S_0.E == 0 && obj.S_0.I == 0)
    { // 0 people with, at least, virus in latent phase
        error("The disease will not spread if nobody has taken the virus in "
              "the initial state!");
        return false;
    }
    if (obj.beta <= 0.0 || obj.gamma <= 0.0 || obj.alpha <= 0.0)
    {
        error("The parameters can't be negative or 0!");
        return false;
        // add some more constraints
    }

    if (obj.S_0.S + obj.S_0.E + obj.S_0.I + obj.S_0.R != obj.N)
    {
        error("The sum of susceptibles, latent, infected and removed individuals "
              "must equal the total population!");
    }

    return true;
}
///////////////// DETERMINE NEW STATE WITH RUNGE KUTTA METHOD/////////////////
State SEIR::RungeKuttaSolver(State const& oldState)
{
    State newState;
    State updatedState;

    // 1st order term in formula for S,E,I,R
    // k1 = f(S,E,I,R)
    std::array<double, 4> k1{};
    k1[0] = TIME_STEP * oldState.dS_dt(beta, N);
    k1[1] = TIME_STEP * oldState.dE_dt(beta, alpha, N);
    k1[2] = TIME_STEP * oldState.dI_dt(alpha, gamma);
    k1[3] = TIME_STEP * oldState.dR_dt(gamma);

    // update the state by half a TIME_STEP
    updatedState.S = oldState.S + k1[0] / 2.0;
    updatedState.E = oldState.E + k1[1] / 2.0;
    updatedState.I = oldState.I + k1[2] / 2.0;
    updatedState.R = oldState.R + k1[3] / 2.0;

    // 2nd order term in formula for S,E,I,R
    // k2 = f(S+h/2*k1 ,E+h/2*k1 ,I+h/2*k1 ,R+h/2*k1)
    std::array<double, 4> k2{};
    k2[0] = TIME_STEP * updatedState.dS_dt(beta, N);
    k2[1] = TIME_STEP * updatedState.dE_dt(beta, alpha, N);
    k2[2] = TIME_STEP * updatedState.dI_dt(alpha, gamma);
    k2[3] = TIME_STEP * updatedState.dR_dt(gamma);

    // update the state by half a TIME_STEP
    updatedState.S = oldState.S + k2[0] / 2.0;
    updatedState.E = oldState.E + k2[1] / 2.0;
    updatedState.I = oldState.I + k2[2] / 2.0;
    updatedState.R = oldState.R + k2[3] / 2.0;

    // 3rd order term in formula for S,E,I,R
    // k3 = f(S+h/2*k ,E+h/2*k2 ,I+h/2*k2 ,R+h/2*k2)
    std::array<double, 4> k3{};
    k3[0] = TIME_STEP * updatedState.dS_dt(beta, N);
    k3[1] = TIME_STEP * updatedState.dE_dt(beta, alpha, N);
    k3[2] = TIME_STEP * updatedState.dI_dt(alpha, gamma);
    k3[3] = TIME_STEP * updatedState.dR_dt(gamma);

    // update the state by the whole TIME_STEP
    updatedState.S = oldState.S + k3[0];
    updatedState.E = oldState.E + k3[1];
    updatedState.I = oldState.I + k3[2];
    updatedState.R = oldState.R + k3[3];

    // 4th order term in formula for S,E,I,R
    // k4 = f(S+h ,E+h ,I+h ,R+h)
    std::array<double, 4> k4{};
    k4[0] = TIME_STEP * updatedState.dS_dt(beta, N);
    k4[1] = TIME_STEP * updatedState.dE_dt(beta, alpha, N);
    k4[2] = TIME_STEP * updatedState.dI_dt(alpha, gamma);
    k4[3] = TIME_STEP * updatedState.dR_dt(gamma);

    // calculating the values of S,E,I,R for the new state
    // yn+1= yn + 1/6(k1+2k2+2k3+k4)
    newState.S = oldState.S + (k1[0] + 2.0 * k2[0] + 2.0 * k3[0] + k4[0]) / 6.0;
    newState.E = oldState.E + (k1[1] + 2.0 * k2[1] + 2.0 * k3[1] + k4[1]) / 6.0;
    newState.I = oldState.I + (k1[2] + 2.0 * k2[2] + 2.0 * k3[2] + k4[2]) / 6.0;
    newState.R = oldState.R + (k1[3] + 2.0 * k2[3] + 2.0 * k3[3] + k4[3]) / 6.0;

    // if one of SEIR is <0 set it to 0
    return newState;
}

///////////////// EVOLVE /////////////////
void SEIR::evolve(std::vector<State>& states)
{
    assert(states.empty());
    states.reserve(t);

    State current_state = S_0;
    State new_state;

    states.push_back(current_state); // the first state corresponds to initial conditions

    for (int i = 0; i < t; ++i)
    {
        new_state = RungeKuttaSolver(current_state); // calculate new state with Runge Kutta
        states.push_back(new_state);                 // push into the SEIRulation
        current_state = new_state;
    }
}
///////////////// RATE OF CHANGE OF SUSCEPTIBLES /////////////////
double State::dS_dt(double beta, int N) const
{
    return -(beta / N) * S * I;
}
///////////////// RATE OF CHANGE OF EXPOSED /////////////////
double State::dE_dt(double beta, double alpha, int N) const
{
    return +beta * S * I / N - alpha * E;
}
///////////////// RATE OF CHANGE OF INFECTED /////////////////
double State::dI_dt(double alpha, double gamma) const
{
    return alpha * E - gamma * I;
}
///////////////// RATE OF CHANGE OF RECOVERED /////////////////
double State::dR_dt(double gamma) const
{
    return +gamma * I;
}
} // namespace RK_4
