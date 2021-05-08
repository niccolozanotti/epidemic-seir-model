#include <vector>
#include "person.hpp"
namespace sim
{
class Simulation
{
  private:
    std::vector<Person> Population;
    double alpha;
    double beta;
    double gamma;
    double spread_radius;
    int time_in_days;

  public:
    Simulation(int S, int E, int I, int R, double alpha, double gamma, double beta, double spread_radius,
               int time_in_days);
    Simulation();
    std::vector<Person *> Close_People(Person &current_person); // function that put in a vector pointers to all
                                                                // other inRadius Susceptible People
    void spread(); // Function that, if *this is I, check close People and try
                   // to spread virus to close S if *this is E or I check if the
                   // State evolve
};

bool try_event(double probability);
}
