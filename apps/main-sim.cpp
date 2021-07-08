//////  STL //////
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
//////  LYRA (CMD LINE PARSER) //////
#include <lyra/lyra.hpp>
//////  ROOT HEADERS  //////
#include "TApplication.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TRootCanvas.h"
////// PROJECT HEADERS //////
#include "simulation.hpp"

int main(int argc, char** argv)
{

    /* clang-format off */

    ////////////////////////////////////////////////  INPUT COLLECTION  ////////////////////////////////////////////////

    bool get_help = false;               // evaluates true if there's any lyra-detected input error
    bool def_sim {false};                //should the simulation be performed with default parameters?
    bool default_seir {true};            // are ratios of S,E,I,R individuals among the population(which is specified) default chosen?
    bool clusters_and_locations {false}; // are both clusters and locations values get specified?
    bool default_params {true};
    int people{};
    static int susceptibles{};
    static int exposed{};
    static int infected{};
    static int recovered{};
    static int locations{};
    static int clusters{};
    int side{};
    double alpha{};
    double beta{};
    double gamma{};
    double spread_radius{};

    lyra::cli cli;  //Lyra object: command line input

    cli.add_argument(lyra::help(get_help))
            .add_argument(lyra::opt(def_sim, "default")
                          ["--def"]["--default"]
                                  .help("Perform the simulation with default chosen values"))
            .add_argument(lyra::opt(people, "people")
                          ["-p"]["--people"]
                                  .help("How many people should there be in the simulation?"))
            .add_argument(lyra::group([&](const lyra::group &) {
                default_seir = false;
            })
                                  .add_argument(lyra::opt(susceptibles, "Susceptibles")
                                                ["-S"]
                                                        .required()
                                                        .choices([](int value){ return value > 0 && value < 70000;})
                                                        .help("Susceptible individuals in the simulation."))
                                  .add_argument(lyra::opt(exposed, "Exposed")
                                                ["-E"]
                                                        .required()
                                                        .choices([](int value){ return value >= 0 && value < susceptibles;})
                                                        .help("Exposed individuals in the simulation."))
                                  .add_argument(lyra::opt(infected, "Infected")
                                                ["-I"]
                                                        .required()
                                                        .choices([](int value){ return value >=0 && value < susceptibles;})
                                                        .help("Infected individuals in the simulation."))
                                  .add_argument(lyra::opt(recovered, "Recovered")
                                                ["-R"]
                                                        .required()
                                                        .choices([](int value){ return value >= 0 && value < 70000;})
                                                        .help("Recovered individuals in the simulation."))) //end group
            .add_argument(lyra::opt(locations, "locations")
                          ["-l"]["--loc"]
                                  .choices([](int value){ return value >= 0;})
                                  .help("How many locations should there be on the map?"))
            .add_argument(lyra::opt(clusters, "clusters")
                          ["-c"]["--clust"]
                                  .choices([](int value){ return value > 0 && value < 40;})
                                  .help("How many cluster should the area be divided into?"))
            .add_argument(lyra::group([&](const lyra::group &) {
                clusters_and_locations = true;
            })
                                  .add_argument(lyra::opt(locations, "locations")
                                                ["-l"]["--loc"]
                                                        .required()
                                                        .choices([](int value){ return value / clusters >= MINIMUM_LOC_CLUST_RATIO; })
                                                        .help("How many locations should there be on the map?"))
                                  .add_argument(lyra::opt(clusters, "clusters")
                                                ["-c"]["--cl"]
                                                        .required()
                                                        .choices([](int value){ return locations / value >= MINIMUM_LOC_CLUST_RATIO; })
                                                        .help("How many cluster should the area be divided into?")))//end group
            .add_argument(lyra::group([&](const lyra::group &) {
                default_params = false;
            })
                                  .add_argument(lyra::opt(alpha, "alpha")
                                                ["-a"]["--alpha"]
                                                        .choices([](double value){ return value >= 0.0 && value <= 1.0;})
                                                        .help("Parameter: governs the lag between infectious contact and showing symptoms."))
                                  .add_argument(lyra::opt(beta, "beta")
                                                ["-b"]["--beta"]
                                                        .choices([](double value){ return value >= 0.0 && value <= 1.0;})
                                                        .help("Parameter: number of people an infective person infects each day."))
                                  .add_argument(lyra::opt(gamma, "gamma")
                                                ["-g"]["--gamma"]
                                                        .choices([](double value){ return value >= 0.0 && value <= 1.0;})
                                                        .help("Parameter: cumulative probability for a person to recover or die."))) //end group
            .add_argument(lyra::opt(side, "side")
                          ["--sd"]["--side"]
                                  .choices([](int value){ return value >=locations;})
                                  .help("How big should the simulation area side be"));

    /* clang-format on */

    // Parse the program arguments:
    auto result = cli.parse({argc, argv});

    // Check that the arguments where valid:
    if (!result)
    {
        std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
        std::cerr << cli << "\n";
        return EXIT_FAILURE;
    }

    // Show help indications if needed
    if (get_help)
    {
        std::cout << cli << "\n";
        return 0;
    }

    // Terminate program in case the user chooses to perform the default simulation but also sets some parameters
    if (def_sim && (people != 0 || !default_seir || locations != 0 || clusters != 0 || !default_params || side != 0 ||
                    spread_radius != 0))
    {
        std::cerr << "The simulation mode has been setted as default mode, but some parameters have been specified by "
                     "the user.\n";
        std::cerr << "Use --def 1 (or --default 1) without any other argument to simulate with default values."
                  << std::endl;
        return EXIT_FAILURE;
    }
    //////// Perform simulation with default chosen parameters ////////
    if (def_sim)
    {
        people = DEF_PEOPLE;
        susceptibles = people * DEF_S;
        exposed = people * DEF_E;
        infected = people * DEF_I;
        recovered = people * DEF_R;
        locations = DEF_LOCATIONS;
        clusters = DEF_CLUSTERS;
        alpha = DEF_ALPHA;
        beta = DEF_BETA;
        gamma = DEF_GAMMA;
        side = DEF_SIDE;
        spread_radius = 1;
    }
    //////// The user has chosen to set simulation parameters himself ////////
    else
    {
        // handle missing input cases
        if (default_seir && people == 0)
        {
            std::cerr << "At least one parameter among 'people' and the group 'S,E,I,R' must be specified."
                      << std::endl;
            return EXIT_FAILURE;
        }
        if (!clusters_and_locations && locations == 0 && clusters == 0)
        {
            std::cerr << "At least one parameter among 'clusters' and 'locations' must be specified." << std::endl;
            return EXIT_FAILURE;
        }
        // only simulation people number have been specified: set S,E,I,R default ratio among people
        if (default_seir)
        {
            susceptibles = people * DEF_S;
            exposed = people * DEF_E;
            infected = people * DEF_I;
            recovered = people * DEF_R;
        }
        // only clusters have been specified leaving locations out
        if (!clusters_and_locations && locations == 0 && clusters > 0) { locations = clusters * 150; }
        // only locations have been specified leaving clusters out
        else if (!clusters_and_locations && locations > 0 && clusters == 0)
        {
            clusters = locations / 150;
        }

        // automatically set the chosen epidemic parameters if not specified in the command line
        if (default_params)
        {
            alpha = DEF_ALPHA;
            beta = DEF_BETA;
            gamma = DEF_GAMMA;
        }

        spread_radius = (double)(side) / 1000.0; // TODO determine a smart way to do that
    }

    std::cout << "Clusters == " << clusters << "\t Locations == " << locations << std::endl;
    std::cout << "People == " << people << std::endl;
    std::cout << "S == " << susceptibles << "\nE == " << exposed << "\nI == " << infected << "\nR == " << recovered
              << std::endl;
    std::cout << "Simulation side == " << side << "\t Spread radius == " << spread_radius << std::endl;
    // Everything was ok, width will have a value if supplied on command line.

    ////////////////////////////  Simulation ////////////////////////////
    using namespace smooth_sim;

    auto start = std::chrono::high_resolution_clock::now();

    Simulation prova{susceptibles, exposed, infected, recovered, clusters,     locations,
                     side,         alpha,   beta,     gamma,     spread_radius};
    for (int k = 0; k < 10; ++k)
    {
        prova.move();
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> duration = end - start;
    std::cout << "Generation and first movement : " << duration.count() << " s " << std::endl;

    auto start1 = std::chrono::high_resolution_clock::now();
    auto start2 = std::chrono::high_resolution_clock::now();

    auto end1 = std::chrono::high_resolution_clock::now();
    auto end2 = std::chrono::high_resolution_clock::now();

    double move_count;
    double spread_count;

    std::vector<Data> res{};
    std::vector<Position> positions{};
    std::vector<bool> at_home{};
    for (int i = 0; i < 30; ++i)
    {
        move_count = 0;
        spread_count = 0;
        start = std::chrono::high_resolution_clock::now();
        for (int j = 0; j < UPDATE_ZONES_INTERVAL; ++j)
        {
            start1 = std::chrono::high_resolution_clock::now();
            prova.move();
            end1 = std::chrono::high_resolution_clock::now();
            start2 = std::chrono::high_resolution_clock::now();
            prova.spread();
            end2 = std::chrono::high_resolution_clock::now();
            res.push_back(prova.get_data());
            positions.push_back(prova.get_person_pos(0, 0));
            at_home.push_back(prova.is_person_at_home(0, 0));

            std::chrono::duration<float> duration1 = end1 - start1;
            std::chrono::duration<float> duration2 = end2 - start2;
            move_count += duration1.count();
            spread_count += duration2.count();
        }
        prova.update_zones();
        end = std::chrono::high_resolution_clock::now();
        duration = end - start;
        std::cout << i + 1 << "-nth Cycle : " << duration.count() << " s "
                  << "   Move: " << move_count << " s "
                  << "   Spread: " << spread_count << " s " << std::endl;
    }

    std::ofstream out{"output.txt"};

    for (auto& a : res)
    {
        out << "S = " << a.S << " E = " << a.E << " I = " << a.I << " R = " << a.R << std::endl;
    }

    for (unsigned long i = 0; i < positions.size(); ++i)
    {
        out << "X = " << positions[i].get_x() << " Y = " << positions[i].get_y() << "   at home: " << at_home[i]
            << std::endl;
    }

    // ROOT CODE

    TApplication app("app", &argc, argv);

    auto c0 = new TCanvas("c0", "Evoluzione");
    auto mg = new TMultiGraph();
    auto gS = new TGraph();
    auto gE = new TGraph();
    auto gI = new TGraph();
    auto gR = new TGraph();
    gS->SetLineColor(kBlue);
    gE->SetLineColor(kOrange);
    gI->SetLineColor(kGreen);
    gR->SetLineColor(kRed);
    mg->SetTitle("Evolution; steps; number of people");

    int t2 = 0;
    for (auto& a : res)
    {
        gS->SetPoint(t2, t2, a.S);
        gE->SetPoint(t2, t2, a.E);
        gI->SetPoint(t2, t2, a.I);
        gR->SetPoint(t2, t2, a.R);
        t2++;
    }

    mg->Add(gS);
    gS->SetTitle("S");
    mg->Add(gE);
    gE->SetTitle("E");
    mg->Add(gI);
    gI->SetTitle("I");
    mg->Add(gR);
    gR->SetTitle("R");

    mg->Draw("AL");
    c0->BuildLegend();

    c0->Modified();
    c0->Update();
    TRootCanvas* rc = (TRootCanvas*)c0->GetCanvasImp();
    rc->Connect("CloseWindow()", "TApplication", gApplication, "Terminate()");
    app.Run();
}