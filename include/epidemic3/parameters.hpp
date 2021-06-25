//////////////////// PARAMETERS OF THE SIMULATION ////////////////

#include "configuration.h" //get the CMAKE VARIABLES parameters

int constexpr CLUSTERS_SIZE = CLUSTERS;
int constexpr WAYPOINTS_SIZE = WAYPOINTS;
int constexpr POPULATION_SIZE = POPULATION;
double constexpr TRANSMISSION_RANGE = 2.0;
double constexpr MAXIMUM_GROUP_PROBABILITY = 0.5;
double constexpr HOME_RADIUS = 0.5;
double constexpr STARTING_LATP_PARAMETER = 0.5;
double constexpr TIME_STEP = 0.5;
int constexpr CHECK_ZONES_INTERVAL = 5;
double constexpr VISITING_PERCENTAGE_WHITE = 0.4;
double constexpr VISITING_PERCENTAGE_YELLOW = 0.3;
double constexpr VISITING_PERCENTAGE_ORANGE = 0.2;
double constexpr VISITING_PERCENTAGE_RED = 0.1;
double constexpr AVERAGE_FAMILY_SIZE = 2.5;
double constexpr PAUSE_EXPONENT = 0.5;
double constexpr MAX_PAUSE = 60.0;
double constexpr MIN_PAUSE = 5.0;
double constexpr STARTING_VELOCITY[2]{0.5, 0.5}; // units/step
double constexpr MEAN_SPEED = 2.0;
double constexpr MAXIMUM_VELOCITY_STDDEV = 1.0;
double constexpr OTHER_CLUSTERS_PROBABILITY = 0.30;