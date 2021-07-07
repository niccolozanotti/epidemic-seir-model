# Epidemic simulation
This project represents the solution to our University Programming Course [final assignment][assignment].
We, the authors(Niccolò Zanotti and Filippo Pretolani), are two Physics Undegraduates studying at
University of Bologna.

The project is made up of 2 main parts:

1. Using the known SEIR model to determine the spread of an epidemic, given initial conditions and
   parameters

2. Simulating an epidemic

We solved the SEIR ordinary differential equations using both a first-order numerical method([Euler][euler] method) and
with a local error of

```math
\frac{dS}{dt} = -\beta \frac{S}{N} I \\[3mm]
\frac{dI}{dt} = \beta \frac{S}{N} I - \gamma I \\[3mm]
\frac{dR}{dt} = \gamma I
```

a fourth-order one ([Runge-Kutta][rk4] method).
The point was t

The Simulation of epidemic is based on the SMOOTH* model to simulate how people moves
and implements a division in Clusters which can change color, changing how people move.

--------------------------------------------------------------------------------
## Dependencies
- [Lyra](https://github.com/bfgroup/Lyra) (bundled)
- [Doctest](https://github.com/onqtam/doctest) (bundled)
- [SFML](http://www.sfml-dev.org/) (required)
- [Root](https://root.cern) (required)
- [CMake](https://cmake.org/) (required)

--------------------------------------------------------------------------------
## Building
Make sure that all the required dependencies are installed.

The first step is to clone the repository:
```shell
#clone the final branch of the repository
git clone -b final --single-branch https://gitlab.com/Feyn-23/epidemic.git 
#enter the repository
cd epidemic
```
Then you can either run the script in the script folder:
```shell
#from the cloned repository directory
cd scripts
#open the script of the application you want to build
./appname.sh
```
Or you can build directly:
```shell
#create and enter a build directory
mkdir build && cd build
#prepare to build files
cmake ..
#if you want to build every app
make
#if you want to build a specific app
make appname
```
The possible apps to build are:

| App name     | function                                                     |
| ------------ | ------------------------------------------------------------ |
| seir         | Solve the SEIR differential equation with Euler Method       |
| seir2        | Solve the SEIR differential equation with Runge-Kutta Method |
| sim          | Simulate an epidemic, without graphical output               |
| sim-graphics | Simulate an pandemic, with graphical output                  |

### Notes
On certain devices, CMake might not be able to detect ROOT installation. If that is the case, the problem can be solved by manually specifying the
path to your ROOT installation ,setting the CMake Variable ROOT_DIR at build time:
```shell
cmake -B path-to-build-dir -S path-to-source -DROOT_DIR="path-to-ROOT-installation"
```

--------------------------------------------------------------------------------
## Running
All the built apps will be in the epidemic/apps directory.

If you are on WSL make sure the Xserver is running as it is required for every application

TO ADD ALL THE PARTS RELATED TO INPUT

--------------------------------------------------------------------------------
# Implementation
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
The Person class represent, as the name suggest, the person in the simulation, it is composed of 5 private members:
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

### Random
This class implements the random generation features critical for this project. It  making use of the header-only library
`randutil`. This small library enhances c++11 random-number facilities found in <random> supplying a simple and easy to use
class. The main purpose of our use of it in our random implementation is the high quality seeding given by the use of multiple
sources of entropy(see [here][seed_entropy]) which sometimes may not be achieved trough `std::random_device`.

[randutil][randutils_git]
which guarantees high entropy seeding which sometimes may not be
achieved through std::random_device. Additionally it implements some random operations useful for our Simulation.

--------------------------------------------------------------------------------
## Tests
Testing is enabled by default in cmake; so if you want to run a test you just need to build and run it.
```shell
#build the test
make testname
#run the test
cd tests && ./testname
#or
./tests/testname
```
The test are(TO FILL):

| App name     | function    |
| ------------ | ----------- |
| test         | Description |
| test         | Description |
| test         | Description |
| test         | Description | 



--------------------------------------------------------------------------------
## Additional Notes

[euler]:https://en.wikipedia.org/wiki/Euler_method#Using_step_size_equal_to_1_(h_=_1)
[rk4]:https://en.wikipedia.org/wiki/Runge–Kutta_methods
[assignment]:https://baltig.infn.it/giaco/pf2020/-/blob/master/progetto/progetto.md
[randutils_web]:https://gist.github.com/imneme/540829265469e673d045
[randutils_git]:https://gist.github.com/imneme/540829265469e673d045
[seeding]:https://www.pcg-random.org/posts/simple-portable-cpp-seed-entropy.html
