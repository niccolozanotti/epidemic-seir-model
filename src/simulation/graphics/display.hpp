#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include "simulation.hpp"
#include <SFML/Graphics.hpp>


namespace smooth_sim{

class Display{
  private:
    Simulation* sim; // pointer to the simulation to display
    double Ratio; // Ratio of the wanted window height divided by the sim_side
    sf::RenderWindow &Window; // reference to the window where all will be rendered
    sf::VertexArray Clusters; // vertex array that represent the clusters
    sf::VertexArray Borders; // vertex array that represent the clusters borders
    sf::VertexArray Locations; // vertex array that represent the locations
    // vertex arrays that are used for the graph
    unsigned Graph_width;
    double dx; //increment for every step of the graph
    double coeff; // coefficient that make sure the graphs stay in the window
    double offset; // offset to plot the graphs, it's equivalent to the side of the simulation
    sf::VertexArray Susceptible;
    sf::VertexArray Exposed;
    sf::VertexArray Infected;
    sf::VertexArray Recovered;
  public:
    Display(Simulation& simulation, sf::RenderWindow &window, unsigned Window_height);

  private:
    //generate the vertex array population
    sf::VertexArray population();

    //color the clusters
    void Color_clusters();

    //update the Graphs
    void update_graphs();

  public:
    // update arrays and draw
    void Draw();
};

}



#endif //DISPLAY_HPP