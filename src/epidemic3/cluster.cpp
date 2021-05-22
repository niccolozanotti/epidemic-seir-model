
#include "cluster.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>

namespace SMOOTH
{

Cluster::Cluster(int size) : sz{size}
{
    //        partition_in_groups();
}
// default constructor
Cluster::Cluster() : sz{0}, lbl{0}, w{0.0}, zone{Zone::white}
{
}

/////////////////////////////////////////////////////
////////      GROUP SIZE DETERMINATION        ///////
/////////////////////////////////////////////////////
void Cluster::determine_groups_sizes()
{
    const double maximum_group_size = sz / 2; // TODO the groups can be as big as 66% of the cluster size(makesense???)
    int all_groups_size = 0;
    int current = 0;
    int group_index = 0;

    std::random_device rd; // set the seed
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> rand(1, nearbyint(maximum_group_size));

    assert(Groups.size() == 0);

    Groups.reserve(sz / 2); // allocate space for half of cluster size-->considering the uniform distribution
                            // should avoid reallocation in the majority of the cases
    for (int i = 0; i < sz; ++i)
    {

        current = rand(gen); // pick a rand size

        if (all_groups_size + current == sz)
        {
            Group g{current, group_index};
            Groups.push_back(g);
            break;
        }
        if (all_groups_size + current < sz)
        {
            Group g{current, group_index};
            Groups.push_back(g);
            ++group_index;
            all_groups_size += current;
        }

    } // end for

    // Fix the size if 1 waypoint is left out either from the 1st of from last group
    int total_size = 0;
    for (unsigned long int j = 0; j < Groups.size(); ++j)
    {
        total_size += Groups[j].size();
    }
    int difference = sz - total_size;
    Groups[Groups.size() - 1].size() += difference;
}

} // namespace SMOOTH
