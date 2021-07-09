# Simulation


## SMOOTH Paper 


This is our implementation of the second part of the assignment.

It simulates how an epidemic spread in a World divided in various part(Clusters), where the people move between
various location.

## Classes

### Position
Position is the class that handle the coordinate of the simulated world. It is composed of 2 double private
members which represent respectively the `x` and the `y` coordinates, and by function to check if two position
are close and to move a position closer to another

### Location
The Location class represent a place that a person can visit and where a person will stay for a randomized
time, it's where usually the virus spread.  
The class is composed of 3 private members:
1. `position` an object of the Position class which represent the Location center position in the simulated world
2. `radius` a double type member which represent the size of the location
3. `cluster_index` Index of the cluster where the location is located(See Cluster)

### Status
it's an enum class composed of 4 possible values:
1. `Susceptible` Can contract the disease if in contact with an infected individual.
2. `Exposed` It has the disease but cannot spread the disease to other people.
3. `Infected` It has the disease and can spread the disease to other people.
4. `Recovered` It has either recovered or died from the disease, it will not be reinfected.

### Person
The Person class represents, as the name suggests, the person in the simulation, it is composed of 5 private members:
1. `position` that represent the person coordinate
2. `current_status` that represent the current Status of the person
3. `next_status` that represent the status that the person will have in the next step
4. `home` an object of the Location class, represent the person's home.
5. `label` it represent the index of the cluster in which the person's home is located

### Mobility Model
This class is used to implement movement in the simulation, it is composed of 7 private members:
1. `person` an object of the Person class which represent the person that is going to be moved
2. `path` a vector of pointers to Location, which represent the Locations that the person is going to
   visit before returning home
3. `target_location` the current location the person is moving to
4. `stay` the number of step the person will stay at target_location
5. `home_probability` the probability that the person will remain home
6. `at_home` a boolean object which represent if the person is at_home or not
7. `going_home` a boolean object which represent if the person is going_home

It is then composed of various function, the most important are:
* `move()` which move the person closer to the **target_location**.
* `next_location()` which follows the Least Action Trip Planning algorithm(LATP) to select the next
  location to visit from the **path** vector.

### Rectangle
The Rectangle class represent a rectangle. The simulation world is in fact represented by a square divided in
rectangles as it is a very simple shape to handle. It is composed of 2 private members:
1. `blh_corner` the bottom left-hand corner of the rectangle
2. `trh_corner` the top right-hand corner of the rectangle

It is then composed of various function, the most important are:
* `Split()` which split a rectangle in 2 rectangles of different sizes.
* `Divide(int n)` which divide the rectangle in n rectangles using the Split function.

### Group
The Group class represent a group of Location object that are generated close to each other; it is composed
of three private members:
1. `Locations` a vector of Location object.
2. `center` the coordinate of the centre around which the Locations are generated.
3. `grp_engine` an object of the Random class, used to generate random numbers.

### Zone
It's an enum class that represent the color of the cluster, which is dependant on the number of infected;
there are 3 possible values, which have different restrictions:
1. `Green` people can go to every other green cluster, big number of location to visit per person.
2. `Yellow` people can stay only in their cluster, smaller number of location to visit per person.
3. `Red` people ca stay only in their cluster, very small number of location to visit per person.

### Cluster
The Cluster class represent the various part in which the world is divided, it has 7 private members:
1. `people` vector of Mobility_Model objects, which represent the population that reside in the cluster.
2. `groups` vector of Group objects.
3. `Area` an object of the Rectangle class that represent the area of the cluster.
4. `zone` represent the cluster color.
5. `index` index of the cluster.
6. `LATP_alpha` represent the value of the alpha parameter for the LATP algorithm.
7. `cl_engine` an object of the Random class, used to generate random numbers.

It is then composed of various function, the most important are:
* `generate_path(int n, vector<Location*>&)` generate a path of n locations selected them from the cluster
  and put them in the referenced vector

### World
The World class represent the world of the Simulation, it has 3 private members:
1. `Area` an object of the Rectangle class, that represent the Area of the world.
2. `Clusters` a vector of Cluster object in which the world is divided.
3. `wrld_engine` an object of the Random class, used to generate random numbers.

It is then composed of various function, the most important are:
* `generate_path(int n, vector<double> wehights, vector<Location*>& path)` generate a path of n random
  locations selected from the clusters, based on the weights vector.
### Simulation
The Simulation class handle the evolution of the epidemic, it is composed of 7 private members:
1. `sim_engine` an object of the Random class, used to generate random numbers.
2. `wrld` an object of the World class, it represent the world where the epidemic is simulated.
3. `alpha` represent the probability that a person pass from Exosed to Infected status.
4. `beta` represent the probability that a person pass from Susceptible to Exposed status when in contact
   with an Infected person
5. `gamma` represent the probability that a person pass from the Infected to Recovered status
6. `spread_radius` it's the radius that represent when a person is in contact with another
7. `time` represent the duration of the simulation NEED TO PROBABLY REMOVE

It is then composed of various function, the most important are:
* `move()` move all the person in the wrld object.
* `spread()` spread the epidemic.
* `update_zone()` update the color of every cluster based on the number of Infected.

The simulation is divided in cycles, which is composed of 10(TO BE POSSIBLY CHANGED) step:  
Every step consist in calling the `move()`, the `spread()` and the `update_status()` functions.  
Every cycle consist in calling 10 steps and the calling the `update_zone()` function to, if necessary,
change the cluster color.

#### move() function
The move function is an adaptation of the random waypoint mobility model seen in the SMOOTH paper.  
It checks every cluster and, based on the zone color, it calls the correspondent move function from:
* `move_white(Cluster&)`
* `move_yellow(Cluster&)`
* `move_red(Cluster&)`

The cluster color decide the number of location to visit and from which cluster they will be chosen.
If the cluster is *Green* then about half of the location will be chosen from the person's cluster, while the
other half from other green cluster; if the cluster is *Red* or *Yellow* then they will all be chosen from
the person's cluster.

Every move function checks every people in the cluster:
* If they are home it checks if they leave it based on `home_probability`, and if they do it fill the path
  vector, following the rules decided by the cluster color
* If they are not at home:
    + If it has arrived at target location:
        - If `stay` is higher than 0 than it remain at the target location
        - if `stay` is 0 then it selects a new target location from the path with `mobility_model::next_location()`
    + If it has not arrived at the target location it moves closer to it with `mobility_model::move()`

When the path is empty it returns home.

#### spread() function
The spread function check every *Exposed* and *Infected* person in the world.
* For every *Exposed* person it check if it becomes *Infected* using `alpha` probability.
* For every *Infected* person:
    * It checks the color of the cluster in which the person reside
        * If it is *Green* it checks all person to see which are *Susceptible*, inside the spread radius and not
          at home, to see if they become *Exposed* using `beta` probability.
        * If it is *Yellow* or *Red* it only checks all person in the same cluster of the person.
    * It checks if it becomes *Recovered*  using `gamma` probabilty.
    
### Display
The display class is used to display the simulation. We used SFML for everything.
The class has 13 private members:
1. `sim` Pointer to the Simulation object that will be displayed.
2. `Window` a reference to the window in which everything will be displayed.
3. `Cluster` a Vertex Array of *sf::Quads* primitive to draw the Clusters.
4. `Borders` a Vertex Array of *sf::Lines* primitive to draw the Clusters Borders.
5. `Locations` a Vertex Array of *sf::Triangles* primitive to draw octagon that represents the various locations
and their radius.
6. `Graph_width` The width of the Graph.
7. `coeff` a coefficient to adapt the values of S to the Height of the windows so that the plotted graphs use
the entire window.
8. `dx` the increment used to plot the graphs.
9. `offset` offset needed to draw the graphs, it ha the same value of the world side in sim.
10. `Susceptible` a Vertex Array of *sf::LineStrip* primitive to plot the graph of susceptible people.
11. `Exposed` a Vertex Array of *sf::LineStrip* primitive to plot the graph of exposed people.
12. `Infected` a Vertex Array of *sf::LineStrip* primitive to plot the graph of infected people.
13. `Recovered` a Vertex Array of *sf::LineStrip* primitive to plot the graph of recovered people.

It's functions are:
* `Color_clusters()` which checks the cluster color and color the vertexes of the cluster to color
  it when it is drawn;
* `population()` which checks every person and create a Vertex Array of *sf::Quads* primitive to draw every
person.
* `update_graphs()` add the next point to all graphs.
* `Draw()` draw all Vertex arrays using *sf::Window::draw()*.

The colors of the objects are chosen as follows:

Clusters:

| Color     | meaning                                |
| --------- | -------------------------------------- |
| ![green](https://via.placeholder.com/25/0ADC00/000000?text=+)  | When *Cluster::zone* is `Zone::Green`  |
| ![yellow](https://via.placeholder.com/25/EBF216/000000?text=+) | When *Cluster::zone* is `Zone::Yellow` |
| ![red](https://via.placeholder.com/25/F04646/000000?text=+)    | When *Cluster::zone* is `Zone::Red`    |

People and graphs:

| Color     | meaning                                |
| --------- | -------------------------------------- |
| ![S_color](https://via.placeholder.com/25/ffffff/000000?text=+)    | When *Person::Status* is `Status::S`   |
| ![E_color](https://via.placeholder.com/25/006E6E/000000?text=+)    | When *Person::Status* is `Status::E`   |
| ![I_color](https://via.placeholder.com/25/5A006E/000000?text=+)    | When *Person::Status* is `Status::I`   |
| ![R_color](https://via.placeholder.com/25/7D7D7D/000000?text=+)    | When *Person::Status* is `Status::R`   |

The end result is like that:

![Sim-graph](assets/Images/Sim-graph.PNG)

At the left we have the simulation. At the right the graphs of the 4 statuses of the people.  
The colored rectangles represent the various clusters, the Blue points represent group of locations, and the colored small squares represent the people that
are moving, all the people that are in their home are not displayed.

### Random
This class implements the random generation features critical for this project. It  making use of the header-only library
`randutil`. This small library enhances c++11 random-number facilities found in <random> supplying a simple and easy to use
class. The main purpose of our use of it in our random implementation is the high quality seeding given by the use of multiple
sources of entropy(see [here][seed_entropy]) which sometimes may not be achieved through `std::random_device`.

[randutil][randutils_git]
which guarantees high entropy seeding which sometimes may not be
achieved through std::random_device. Additionally it implements some random operations useful for our Simulation.



[randutils_web]:https://gist.github.com/imneme/540829265469e673d045
[randutils_git]:https://gist.github.com/imneme/540829265469e673d045
[seeding]:https://www.pcg-random.org/posts/simple-portable-cpp-seed-entropy.html
[latp]:https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.530.4886&rep=rep1&type=pdf
