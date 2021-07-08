# Epidemic simulation
This project represents the solution which we, Niccolò Zanotti and Filippo Pretolani, two Physics Undegraduates studying at
University of Bologna, gave to our University Programming Course [final assignment][assignment].
We collaborated on writing the source code found in this repository except for, of course, external libraries [code](extern/ExternLibraries.md)

The project is made up of 2 main parts:

1. Using the known SEIR model to determine the spread of an epidemic, given initial conditions(see [here](include/SEIR.md))

2. Simulating an epidemic, given simulation parameters(see [here](include/Simulation.md))


The Simulation of epidemic is based on the SMOOTH* model to simulate how people moves
and implements a division in Clusters which can change color, changing how people move.

--------------------------------------------------------------------------------
## Dependencies
- [Lyra][lyra] (submodule)
- [Doctest][doct] (submodule)
- [SFML](http://www.sfml-dev.org/) (required)
- [Root](https://root.cern) (required)
- [CMake](https://cmake.org/) (required)

--------------------------------------------------------------------------------
## Building
Make sure all the required dependencies are installed on your machine and fully working.

SISTEMA

First of all, clone the repository :
```shell
#clone the released branch of the repository
git clone -b main --single-branch https://gitlab.com/Feyn-23/epidemic.git 
#enter the repository
cd epidemic
```
To facilitate the user, building scripts are available inside `scripts` project subdirectory.

Then you can either run the script in the script folder:
```shell
#from the cloned repository directory
cd scripts
#open the script of the application you want to build
./appname.sh
```
or you can build directly:
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
Instead of building files using a single core, one can take advantage of the other available cores on the machine to
speed up the building process. To do that, it's sufficient to add `-jN` option at build time, N being equal to the number
of available machine cores plus 1: 

```shell
# for a quad-core machine
make sim-graphics -j5
```

The available executable applications(CMake targets) are the following:

| App name     | Purpose                                                       |
| ------------ | ------------------------------------------------------------  |
| seir         | Solve the SEIR differential equation using a numerical method |
| sim          | Simulate an epidemic, without graphical output                |
| sim-graphics | Simulate an pandemic, with graphical output                   |

### Notes
On certain devices, CMake might not be able to detect ROOT installation. If that is the case, the problem can be solved by manually specifying the
path to your ROOT installation ,setting the CMake Variable ROOT_DIR at build time:
```shell
cmake -B path-to-build-dir -S path-to-source -DROOT_DIR="path-to-ROOT-installation"
```
--------------------------------------------------------------------------------
## User Input
The applications built by this project collect user input through command line arguments. 
We chose the command line parser libray [Lyra][lyra] to handle this task.
The value of an input variable can be setted at execution time by referring to one of its names, that is,
a `char` following a short dash(`-`) or a `string` following a long dash(`--`). \
Therefore, the command to pass the value of two generic input variables named respectively `--var1` and `-v`
is the following:
```shell
./appname -var1 value_of_var1 -v value_of_v
```
Clearly, each application has its own input variables. 
The following tables show the available input options
for each application(an empty cell means the variable doesn't have a second name):

### seir

|       Variable       |    First name      |     Second name     |                     Description                                   |
|----------------------|--------------------|---------------------|-------------------------------------------------------------------|
|   default_sim        |        `--def`     |      `--default`    | `bool`:usage of default simulation values                         |
|   numerical_method   |        `-m `       |       `--method`    | `bool`: usage of Euler method(`false`) or RK4 method(`true`)      |
|   people             |        `-N`        |       `--people`    |  `int`: number of individuals in the simulation                   |
|   susceptibles       |        `-S`        |                     |  `int`: number of suceptible individuals                          |
|   exposed            |        `-E`        |                     |  `int`: number of exposed individuals                             |
|   infected           |        `-I`        |                     |  `int`: number of infected individuals                            |
|   recovered          |        `-R`        |                     |  `int`: number of recovered individuals                           |
|   alpha              |        `-a`        |       `--alpha`     |  `double`: lag between infectious contact and symptoms show       |
|   beta               |        `-b`        |        `--beta`     |  `double`: number of people an infective person infects each day  |
|   gamma              |        `-g`        |       `--gamma`     |  `double`: cumulative probability for a person to recover or die  |
|   time               |        `-t`        |        `--time`     |  `int`: duration of the simulation(days)                          |

### sim

|       Variable       |    First name      |     Second name     |                     Description                                   |
|----------------------|--------------------|---------------------|-------------------------------------------------------------------|
|   default_sim        |        `--def`     |      `--default`    | `bool`:usage of default simulation values                         |
|   people             |        `-N`        |       `--people`    |  `int`: number of individuals in the simulation                   |
|   susceptibles       |        `-S`        |                     |  `int`: number of suceptible individuals                          |
|   exposed            |        `-E`        |                     |  `int`: number of exposed individuals                             |
|   infected           |        `-I`        |                     |  `int`: number of infected individuals                            |
|   recovered          |        `-R`        |                     |  `int`: number of recovered individuals                           |
|   locations          |        `-l`        |        `--loc`      |  `int`: number of locations plotted on the simulation area        |
|   clusters           |         `-c`       |     `--clust`      |  `int`: number of clusters of the simulation area                 |
|   alpha              |        `-a`        |       `--alpha`     |  `double`: lag between infectious contact and symptoms show       |
|   beta               |        `-b`        |        `--beta`     |  `double`: number of people an infective person infects each day  |
|   gamma              |        `-g`        |       `--gamma`     |  `double`: cumulative probability for a person to recover or die  |
|   side               |        `--sd`      |        `--side`     |  `int`: side of the simulation area                               |
|   time               |        `-t`        |        `--time`     |  `int`: duration of the simulation(days)                          |

### sim-graphics

|       Variable       |    First name      |     Second name     |                     Description                                   |
|----------------------|--------------------|---------------------|-------------------------------------------------------------------|
|   default_sim        |        `--def`     |      `--default`    | `bool`:usage of default simulation values                         |
|   people             |        `-N`        |       `--people`    |  `int`: number of individuals in the simulation                   |
|   susceptibles       |        `-S`        |                     |  `int`: number of suceptible individuals                          |
|   exposed            |        `-E`        |                     |  `int`: number of exposed individuals                             |
|   infected           |        `-I`        |                     |  `int`: number of infected individuals                            |
|   recovered          |        `-R`        |                     |  `int`: number of recovered individuals                           |
|   locations          |        `-l`        |        `--loc`      |  `int`: number of locations plotted on the simulation area        |
|   clusters           |         `-c`       |     `--clust`       |  `int`: number of clusters of the simulation area                 |
|   alpha              |        `-a`        |       `--alpha`     |  `double`: lag between infectious contact and symptoms show       |
|   beta               |        `-b`        |        `--beta`     |  `double`: number of people an infective person infects each day  |
|   gamma              |        `-g`        |       `--gamma`     |  `double`: cumulative probability for a person to recover or die  |
|   side               |        `--sd`      |        `--side`     |  `int`: side of the simulation area                               |

Lyra offers a help interface consisting of a summary of all input variables with relative names and descriptions
which can be accessed simply by specifying `-h` or `--help` at execution time.

Of course the user has not to pass all these variables to run the program; there are optionals which can be
default chosen, and some which have to be specified. 
The following examples show some of the possible combinations of input variables, choosing,when available, the longer name and omitting
variables values, just for the sake of clarity:
```shell
#app: seir
exec  --default 1                                                #default values
exec --method --people --alpha --beta --gamma --time              
exec --method --alpha --beta --gamma --time -S -E -I -R         
exec --method --people --time
```
```shell
#app: sim
exec  --default 1                                              #default values
exec --people --clust --loc --alpha --beta --gamma --time      #input clusters and locations 
exec --people --clust --alpha --beta --gamma --time            #input only cluster
exec --people --loc --alpha --beta --gamma --time              #input only locations
exec --clust -loc --time -S -E -I -R --time                    #do not use default S,E,I,R ratio
```
```shell
#app: sim-graphics
exec  --default 1                                              #default values
exec --people --clust --loc --alpha --beta --gamma             #input clusters and locations
exec --people --clust --alpha --beta --gamma                   #input only cluster
exec --people --loc --alpha --beta --gamma                     #input only locations
exec --clust -loc --time -S -E -I -R                           #do not use default S,E,I,R ratio
```
Note that , in order for the parsing to occur correctly, the `S`,`E`,`I`,`R` _group_ of parameters need to be specified at 
the end of the parameters list(see [input validation][inp-val]). \
The standard parameters used when  `default_sim` is setted are the ones in [parameters.hpp](include/simulation/parameters.hpp).

### Input validation
All input values are subject to validity conditions, some more restrictive than others.
Our major source of understanding about reasonable parameters values came from the graphical simulation:
testing multiple times in a variety of conditions allowed us to get a good grasp in this sense.
We decided not to put tight constraints on S,E,I,R values since the cases where they are supposed to assume
weird values are not realistic.

World constraints:

- For the combined value of clusters and locations in the simulation area to be valid, the minimum
  number of locations per cluster should be **10**;
- The number of clusters should be less than **40**;

CONTINUA

## Running
All the built apps will be in the epidemic/apps directory.

If you are on WSL make sure the Xserver is running as it is required for every application

TO ADD ALL THE PARTS RELATED TO INPUT

--------------------------------------------------------------------------------

## Tests
The testing strategy adopted for this project was _Unit testing_. To this aim, we made use of the header library
[doctest][doct]. \
Testing is enabled in Debug mode
```shell
#build the test
make testname
#run the test
cd tests && ./testname
#or
./tests/testname
```
The tests are(TO FILL):

| App name     | function    |
| ------------ | ----------- |
| test         | Description |
| test         | Description |
| test         | Description |
| test         | Description | 



--------------------------------------------------------------------------------
## Additional Notes


[lyra]:https://github.com/bfgroup/Lyra
[doct]:https://github.com/onqtam/doctest
[euler]:https://en.wikipedia.org/wiki/Euler_method#Using_step_size_equal_to_1_(h_=_1)
[rk4]:https://en.wikipedia.org/wiki/Runge–Kutta_methods
[assignment]:https://baltig.infn.it/giaco/pf2020/-/blob/master/progetto/progetto.md

[inp-val]:#input-validation

