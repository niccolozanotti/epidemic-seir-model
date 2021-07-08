# Epidemic simulation
This project represents the solution which we, Niccolò Zanotti and Filippo Pretolani, two Physics Undegraduates studying at
University of Bologna, gave to our University Programming Course [final assignment][assignment].
<!-- put relative path to extern -->
We collaborated on writing the source code found in this repository except for, of course, external libraries [code](##Building)

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
The executable targets availabele to build are the following:

| App name     | Purpose                                                      |
| ------------ | ------------------------------------------------------------ |
| seir         | Solve the SEIR differential equation with Euler Method       |
| sim          | Simulate an epidemic, without graphical output               |
| sim-graphics | Simulate an pandemic, with graphical output                  |

### Notes
On certain devices, CMake might not be able to detect ROOT installation. If that is the case, the problem can be solved by manually specifying the
path to your ROOT installation ,setting the CMake Variable ROOT_DIR at build time:
```shell
cmake -B path-to-build-dir -S path-to-source -DROOT_DIR="path-to-ROOT-installation"
```
--------------------------------------------------------------------------------
## Input
The applications 

--------------------------------------------------------------------------------
## Running
All the built apps will be in the epidemic/apps directory.

If you are on WSL make sure the Xserver is running as it is required for every application

TO ADD ALL THE PARTS RELATED TO INPUT

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

