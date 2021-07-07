#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "simulation.hpp"
#include <SFML/Graphics.hpp>

namespace smooth_sim{

class Display{
  private:
    sf::RenderWindow window; // window where all will be rendered
    sf::VertexArray clusters; // vertex array that represent the clusters
    sf::VertexArray borders; // vertex array that represent the clusters borders
    sf::VertexArray locations; // vertex array that represent the locations
    // vertex arrays that are used for the graph
    sf::VertexArray Susceptible;
    sf::VertexArray Exposed;
    sf::VertexArray Infected;
    sf::VertexArray Recovered;
  public:
    Display(Simulation& simulation, double Graph_width);

  private:
    //generate the vertex array population
    sf::VertexArray population(Simulation& simulation);

    //color the clusters
    void Color_clusters(Simulation& simulation);
};

}



#endif