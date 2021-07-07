#include "display.hpp"

using namespace smooth_sim;

Display::Display(Simulation& simulation, double Graph_width){
    //fill cluster VertexArray and borders VertexArray
    clusters.setPrimitiveType(sf::Quads);
    borders.setPrimitiveType(sf::Lines);
    std::array<float,4> X{}, Y{};
    for (unsigned i = 0; i < simulation.world().size(); ++i)
    {
        auto& cl_a = simulation.world().clusters()[i].area();
        // Cordinates of the Cluster's corners
        X[0] = cl_a.get_blh_corner().get_x();
        Y[0] = cl_a.get_blh_corner().get_y();
        X[1] = cl_a.get_blh_corner().get_x();
        Y[1] = cl_a.get_trh_corner().get_y();
        X[2] = cl_a.get_trh_corner().get_x();
        Y[2] = cl_a.get_trh_corner().get_y();
        X[3] = cl_a.get_trh_corner().get_x();
        Y[3] = cl_a.get_blh_corner().get_y();
        // Fill clusters
        clusters.append(sf::Vector2f(X[0], Y[0]));
        clusters.append(sf::Vector2f(X[1], Y[1]));
        clusters.append(sf::Vector2f(X[2], Y[2]));
        clusters.append(sf::Vector2f(X[3], Y[3]));
        // Fill borders
        borders.append(sf::Vertex(sf::Vector2f(X[0], X[0]), sf::Color::Black));
        borders.append(sf::Vertex(sf::Vector2f(X[1], X[1]), sf::Color::Black));
        borders.append(sf::Vertex(sf::Vector2f(X[1], X[1]), sf::Color::Black));
        borders.append(sf::Vertex(sf::Vector2f(X[2], X[2]), sf::Color::Black));
        borders.append(sf::Vertex(sf::Vector2f(X[2], X[2]), sf::Color::Black));
        borders.append(sf::Vertex(sf::Vector2f(X[3], X[3]), sf::Color::Black));
        borders.append(sf::Vertex(sf::Vector2f(X[3], X[3]), sf::Color::Black));
        borders.append(sf::Vertex(sf::Vector2f(X[0], X[0]), sf::Color::Black));
    }

    //Fill locations VertexArray
    locations.setPrimitiveType(sf::Triangles);
    std::array<double, 9> x{}, y{};
    double r;
    for (auto& cl : simulation.world().clusters())
    {
        for (auto& gr : cl.groups())
        {
            for (auto& l : gr.locations())
            {
                r = l.get_radius();
                // Fill the points
                x[0] = l.get_position().get_x();
                y[0] = l.get_position().get_y();
                for (int j = 1; j < 9; ++j)
                {
                    x[j] = x[0] + r * std::cos(j * PI / 4);
                    y[j] = y[0] + r * std::sin(j * PI / 4);
                }
                // Assign points so that you make octagons(with 8 triangles)
                for (int j = 0; j < 7; ++j)
                { // The first 7 triangles
                    // Set the vertices and the color of the triangles
                    locations.append(sf::Vertex(sf::Vector2f(x[0], y[0]),sf::Color::Blue));
                    locations.append(sf::Vertex(sf::Vector2f(x[j + 1], y[j + 1]),sf::Color::Blue));
                    locations.append(sf::Vertex(sf::Vector2f(x[j + 2], y[j + 2]),sf::Color::Blue));
                }
                // Set the vertices and color of the eight triangle
                locations.append(sf::Vertex(sf::Vector2f(x[0], y[0]),sf::Color::Blue));
                locations.append(sf::Vertex(sf::Vector2f(x[8], y[8]),sf::Color::Blue));
                locations.append(sf::Vertex(sf::Vector2f(x[1], y[1]),sf::Color::Blue));
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PRIVATE METHODS           /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////    POPULATION    //////////
sf::VertexArray Display::population(Simulation& simulation)
{
    sf::VertexArray people(sf::Quads, simulation.world().people_num() * 4);
    double x_0, y_0;
    double r = 1; //half diagonal of the square that represent the person
    int count = 0;
    for (auto& a : simulation.world().clusters())
    {
        for (auto& b : a.people())
        {
            if (!b.is_at_home())
            {
                if (b.person().get_current_status() == Status::Susceptible)
                {
                    people[4 * count].color = sf::Color::White;
                    people[4 * count + 1].color = sf::Color::White;
                    people[4 * count + 2].color = sf::Color::White;
                    people[4 * count + 3].color = sf::Color::White;
                }
                else if (b.person().get_current_status() == Status::Exposed)
                {
                    people[4 * count].color = sf::Color::Cyan;
                    people[4 * count + 1].color = sf::Color::Cyan;
                    people[4 * count + 2].color = sf::Color::Cyan;
                    people[4 * count + 3].color = sf::Color::Cyan;
                }
                else if (b.person().get_current_status() == Status::Infected)
                {
                    people[4 * count].color = sf::Color::Magenta;
                    people[4 * count + 1].color = sf::Color::Magenta;
                    people[4 * count + 2].color = sf::Color::Magenta;
                    people[4 * count + 3].color = sf::Color::Magenta;
                }
                else
                {
                    people[4 * count].color = sf::Color::Black;
                    people[4 * count + 1].color = sf::Color::Black;
                    people[4 * count + 2].color = sf::Color::Black;
                    people[4 * count + 3].color = sf::Color::Black;
                }
                x_0 = b.person().get_position().get_x();
                y_0 = b.person().get_position().get_y();
                people[4 * count].position = sf::Vector2f(x_0 - r, y_0 - r);
                people[4 * count + 1].position = sf::Vector2f(x_0 + r, y_0 - r);
                people[4 * count + 2].position = sf::Vector2f(x_0 + r, y_0 + r);
                people[4 * count + 3].position = sf::Vector2f(x_0 - r, y_0 + r);
            }
            ++count;
        }
    }
    return people;
}

///////////    COLOR THE CLUSTERS    //////////
void Display::Color_clusters(Simulation& simulation)
{
    for (unsigned i = 0; i < simulation.world().clusters().size(); ++i)
    {
        auto& cl = simulation.world().clusters()[i];
        sf::Color color;
        if (cl.get_zone() == Zone::Green) { color = sf::Color::Green; }
        else if (cl.get_zone() == Zone::Yellow)
        {
            color = sf::Color::Yellow;
        }
        else
        {
            color = sf::Color::Red;
        }
        clusters[4 * i].color = color;
        clusters[4 * i + 1].color = color;
        clusters[4 * i + 2].color = color;
        clusters[4 * i + 3].color = color;
    }
}