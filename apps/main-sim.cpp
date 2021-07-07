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

    ///////////////////////  Simulation input parameters ///////////////////////

    bool show_help = false;
    bool get_help = false;
    int people {25000};
    int susceptibles {};
    int exposed {};
    int infected {};
    int recovered {};

    // if true,sets the default chosen ratio of S,E,I,R individuals among the population(which is specified)
    // it can assume false value if all S,E,I,R are specified
    bool default_seir {true};
    double def_S {0.95};
    double def_E {0.02};
    double def_I {0.02};
    double def_R {0.01};


    int clusters_num {5};
    int side {1000};
    double alpha {0.1};
    double beta {0.2};
    double gamma {0.02};
    double spread_radius {1};
    double time_in_days {10};
    int choice {0};

    /* clang-format off */

    lyra::cli cli;  //command line input
    cli.add_argument(lyra::help(get_help))
            .add_argument(lyra::opt(people, "people")
                                  ["-p"]["--people"]
                                  .help("How many people should there be in the simulation?"))
            .add_argument(lyra::group([&](const lyra::group &) {
                default_seir = false;
            })
                                  .add_argument(lyra::opt(susceptibles, "Susceptibles")
                                                        ["-S"]
                                                        .required()
                                                        .choices([](int value){ return value >= 0;})
                                                        .help("Susceptible individuals in the simulation."))
                                  .add_argument(lyra::opt(exposed, "Exposed")
                                                        ["-E"]
                                                        .required()
                                                        .choices([](int value){ return value >= 0;})
                                                        .help("Exposed individuals in the simulation."))
                                  .add_argument(lyra::opt(infected, "Infected")
                                                        ["-I"]
                                                        .required()
                                                        .choices([](int value){ return value >= 0;})
                                                        .help("Infected individuals in the simulation."))
                                  .add_argument(lyra::opt(recovered, "Recovered")
                                                         ["-R"]
                                                        .required()
                                                        .choices([](int value){ return value >= 0;})
                                                        .help("Recovered individuals in the simulation."))) //end group
            .add_argument(lyra::opt(side, "Side")
                                   ["-sd"]["--side"]
                                   .choices([](int value){ return value >= 700 && value <= 1500;})
                                  .help("Side of the simulation area."))
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
                                  .help("Parameter: cumulative probability for a person to recover or die."));
    /* clang-format on */

    // Parse the program arguments:
    auto result = cli.parse({ argc, argv });

    // Check that the arguments where valid:
    if (!result)
    {
        std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
        std::cerr << cli << "\n";
        return 1;
    }

    // Show the help when asked for.
    if (show_help)
    {
        std::cout << cli << "\n";
        return 0;
    }



    // Everything was ok, width will have a value if supplied on command line.
        using namespace smooth_sim;


    auto start = std::chrono::high_resolution_clock::now();

    Simulation prova{25000, 3, 200, 4, 5, 1500, 1000, 0.1, 0.02, 0.2, 1, 10};
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