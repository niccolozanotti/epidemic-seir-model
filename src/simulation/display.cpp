#include "display.hpp"

using namespace smooth_sim;

Display::Display(Simulation& simulation, sf::RenderWindow &window, unsigned Graph_width) : sim{&simulation}, Window{window}, Graph_width{Graph_width}
{
    // fill cluster VertexArray and borders VertexArray
    Clusters.setPrimitiveType(sf::Quads);
    Borders.setPrimitiveType(sf::Lines);
    std::array<float, 4> X{}, Y{};
    for (unsigned i = 0; i < simulation.world().size(); ++i)
    {
        auto& cl_a = sim->world().clusters()[i].area();
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
        Clusters.append(sf::Vector2f(X[0], Y[0]));
        Clusters.append(sf::Vector2f(X[1], Y[1]));
        Clusters.append(sf::Vector2f(X[2], Y[2]));
        Clusters.append(sf::Vector2f(X[3], Y[3]));
        // Fill borders
        Borders.append(sf::Vertex(sf::Vector2f(X[0], Y[0]), sf::Color::Black));
        Borders.append(sf::Vertex(sf::Vector2f(X[1], Y[1]), sf::Color::Black));
        Borders.append(sf::Vertex(sf::Vector2f(X[1], Y[1]), sf::Color::Black));
        Borders.append(sf::Vertex(sf::Vector2f(X[2], Y[2]), sf::Color::Black));
        Borders.append(sf::Vertex(sf::Vector2f(X[2], Y[2]), sf::Color::Black));
        Borders.append(sf::Vertex(sf::Vector2f(X[3], Y[3]), sf::Color::Black));
        Borders.append(sf::Vertex(sf::Vector2f(X[3], Y[3]), sf::Color::Black));
        Borders.append(sf::Vertex(sf::Vector2f(X[0], Y[0]), sf::Color::Black));
    }

    // Fill locations VertexArray
    Locations.setPrimitiveType(sf::Triangles);
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
                    Locations.append(sf::Vertex(sf::Vector2f(x[0], y[0]), sf::Color::Blue));
                    Locations.append(sf::Vertex(sf::Vector2f(x[j + 1], y[j + 1]), sf::Color::Blue));
                    Locations.append(sf::Vertex(sf::Vector2f(x[j + 2], y[j + 2]), sf::Color::Blue));
                }
                // Set the vertices and color of the eight triangle
                Locations.append(sf::Vertex(sf::Vector2f(x[0], y[0]), sf::Color::Blue));
                Locations.append(sf::Vertex(sf::Vector2f(x[8], y[8]), sf::Color::Blue));
                Locations.append(sf::Vertex(sf::Vector2f(x[1], y[1]), sf::Color::Blue));
            }
        }
    }
    //Window.close();
    Window.create(sf::VideoMode(simulation.world().get_side() + Graph_width,simulation.world().get_side()),"Simulation");

    //Set primitive type for the graphs
    Susceptible.setPrimitiveType(sf::LineStrip);
    Exposed.setPrimitiveType(sf::LineStrip);
    Infected.setPrimitiveType(sf::LineStrip);
    Recovered.setPrimitiveType(sf::LineStrip);
    //set the starting value of dx and coeff
    coeff = static_cast<double>(simulation.world().get_side()) / static_cast<double>(simulation.world().people_num());
    dx = static_cast<double>(Graph_width) / 100.0;
    offset = static_cast<double>(simulation.world().get_side());
    //set fisrt point for every graphs
    Data data = simulation.get_data();
    Susceptible.append(sf::Vertex(sf::Vector2f(offset,offset - coeff*data.S),sf::Color::White));
    Exposed.append(sf::Vertex(sf::Vector2f(offset,offset - coeff*data.E),sf::Color::Cyan));
    Infected.append(sf::Vertex(sf::Vector2f(offset,offset - coeff*data.I),sf::Color::Magenta));
    Recovered.append(sf::Vertex(sf::Vector2f(offset,offset - coeff*data.R),sf::Color::Red));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PRIVATE METHODS           /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////    POPULATION    //////////
sf::VertexArray Display::population()
{
    sf::VertexArray people(sf::Quads, sim->world().people_num() * 4);
    double x_0, y_0;
    double r = 1; // half diagonal of the square that represent the person
    int count = 0;
    for (auto& a : sim->world().clusters())
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
void Display::Color_clusters()
{
    for (unsigned i = 0; i < sim->world().clusters().size(); ++i)
    {
        auto& cl = sim->world().clusters()[i];
        sf::Color color;
        if (cl.get_zone() == Zone::Green)
        {
            color = sf::Color::Green;
        }
        else if (cl.get_zone() == Zone::Yellow)
        {
            color = sf::Color::Yellow;
        }
        else
        {
            color = sf::Color::Red;
        }
        Clusters[4 * i].color = color;
        Clusters[4 * i + 1].color = color;
        Clusters[4 * i + 2].color = color;
        Clusters[4 * i + 3].color = color;
    }
}
///////////    UPDATE THE GRAPHS    //////////
void Display::update_graphs()
{
    Data data = sim->get_data();
    unsigned vertex_count = Susceptible.getVertexCount();
    float Sim_side = static_cast<float>(sim->world().get_side());

    //Check if the Graphs are over 4/5 of the Graph width, and if neccessary resize
    if (Susceptible[vertex_count - 1].position.x - Sim_side >= 4 * Graph_width / 5)
    { // if last graph point x is >= of 4/5 of Graph_width, adapt the graph so that it stay in half Graph_width
        double k = Graph_width / (2 * (Susceptible[vertex_count - 1].position.x - Sim_side));
        for (unsigned i = 1; i < vertex_count; ++i)
        { // adapt the x axis
            Susceptible[i].position.x = Sim_side * (1 - k) + k * Susceptible[i].position.x;
            Exposed[i].position.x = Susceptible[i].position.x;
            Infected[i].position.x = Susceptible[i].position.x;
            Recovered[i].position.x = Susceptible[i].position.x;
        }
        dx *= k; // adapt the delta_x
    }

    //append new points to the graph
    float previous_x = Susceptible[vertex_count - 1].position.x;
    Susceptible.append(sf::Vertex(sf::Vector2f(previous_x + dx, offset - coeff*data.S),sf::Color::White));
    Exposed.append(sf::Vertex(sf::Vector2f(previous_x + dx, offset - coeff*data.E),sf::Color::Cyan));
    Infected.append(sf::Vertex(sf::Vector2f(previous_x + dx, offset - coeff*data.I),sf::Color::Magenta));
    Recovered.append(sf::Vertex(sf::Vector2f(previous_x + dx, offset - coeff*data.R),sf::Color::Red));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////           PRIVATE METHODS           /////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////    DISPLAY    //////////
void Display::Draw()
{
    //call teh function to update the arrays
    Color_clusters();
    update_graphs();
    //Draw the arrays
    Window.draw(Clusters);
    Window.draw(Borders);
    Window.draw(Locations);
    Window.draw(population());
    Window.draw(Susceptible);
    Window.draw(Exposed);
    Window.draw(Infected);
    Window.draw(Recovered);

}