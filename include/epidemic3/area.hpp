#ifndef AREA_HPP
#define AREA_HPP

#include "cluster.hpp"
#include "configuration.h"
#include "person.hpp"
#include <array> //using STL array (provides optional bound checking, iterators, ecc to low performance loss)
#include <stdexcept>

namespace SMOOTH
{

// TODO try-catch for checking
// getting some program inputs from Cmake Variables and doing type check
// so that the program won't compile in case of wrong input
int constexpr population_size = POPULATION;
int constexpr clusters_size = CLUSTERS;
int constexpr waypoints_size = WAYPOINTS;

class Area
{
  private:
    double sd; // side of the simulation area
    double R;  // transmission Range
  public:
    std::array<Location, waypoints_size> Waypoints; // array with waypoints
    std::array<Person, population_size> People;     // People in the simulation
    std::array<Cluster, clusters_size> Clusters;    // array with Clusters in the simulation
  private:
    void partition_in_clusters();
    void partition_in_groups(int label);
    Location first_group_step(int label);
    Location other_groups_step(int cl_label, int gr_label, Location const &prev_waypoint) const;
    Location plot_nearby_waypoints(int cluster_label, int group_label, Location const &starting_waypoint);

  public:
    Area(double side, double transmission_range); // constructor

    int side() const
    {
        return sd;
    }

    void partition();
    void plot_waypoints();

}; // class Area

void plot_waypoints(Cluster const &current, Area &world); // setting the waypoints on the map based on the algorithm

} // namespace SMOOTH

#endif // AREA_HPP
