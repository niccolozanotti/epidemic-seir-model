
#include "seir1.hpp"
#include "seir2.hpp"
#include "doctest.h"

TEST_CASE("Differential equation solving test")
{
    Euler::State starting_cond1 {800, 100, 80, 20, 1000};
    Euler::SEIR trial1{starting_cond1, 0.15, 0.4, 0.24};

    RK_4::State starting_cond2 {800,100,80,20};
    RK_4::SEIR trial2{1000,100,starting_cond2,0.4,0.15,0.24};

    auto const result1 = trial1.generate_all_points(100);
    auto it1 = result1.end();
    --it1;

    std::vector<RK_4::State> result2;
    trial2.evolve(result2);
    auto it2 = result2.end();
    --it2;

    CHECK(doctest::Approx(it1->S + it1->E + it1->I + it1->R) == (it1->N));
    CHECK(doctest::Approx(it2->S + it2->E + it2->I + it2->R) == 1000.0);
}