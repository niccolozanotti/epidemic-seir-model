////// STL //////
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
#include "../src/simulation/graphics/display.hpp"
#include "simulation.hpp"

using namespace smooth_sim;

int main(int argc,char** argv)
{

    ///////////////////////  Simulation input parameters ///////////////////////
    bool get_help = false;

    bool def_sim {false};
    // are ratios of S,E,I,R individuals among the population(which is specified) default chosen?
    bool default_seir {true};
    bool clusters_and_locations {false}; // are both clusters and locations values get specified?
    bool default_params {true};
    int people {25000};
    int susceptibles {};
    int exposed {};
    int infected {};
    int recovered {};
    int locations {};
    int clusters {};
    int side {};
    double alpha {0.1};
    double beta {0.2};
    double gamma {0.02};
    double spread_radius {1};
    double time_in_days {10};
    int choice {0};

    /* clang-format off */

    lyra::cli cli;  //command line input

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
            .add_argument(lyra::opt(locations, "locations")
                          ["-l"]["--loc"]
                                  .choices([](int value){ return value >= 0;})
                                  .help("How many locations should there be on the map?"))
            .add_argument(lyra::opt(clusters, "clusters")
                          ["-c"]["--clust"]
                                  .choices([](int value){ return value >= 0;})
                                  .help("How many cluster should the area be divided into?"))
            .add_argument(lyra::group([&](const lyra::group &) {
                clusters_and_locations = true;
            })
                                  .add_argument(lyra::opt(locations, "locations")
                                                ["-l"]["--loc"]
                                                        .required()
                                                        .choices([](int value){ return value >= 0;})
                                                        .help("How many locations should there be on the map?"))
                                  .add_argument(lyra::opt(clusters, "clusters")
                                                ["-c"]["--cl"]
                                                        .required()
                                                        .choices([](int value){ return value >= 0;})
                                                        .help("How many cluster should the area be divided into?")))//end group
            .add_argument(lyra::group([&](const lyra::group &) {
                default_params = true;
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
                                                        .help("Parameter: cumulative probability for a person to recover or die."))); //end group
    /* clang-format on */

    // Parse the program arguments:
    auto result = cli.parse({ argc, argv });

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

    //////// Perform simulation with default chosen parameters ////////
    if (def_sim)
    {
        people = DEF_PEOPLE;
        susceptibles = people * DEF_S;
        exposed = people * DEF_E;
        infected= people * DEF_I;
        recovered = people * DEF_R;
        locations = DEF_LOCATIONS;
        clusters = DEF_CLUSTERS;
        alpha = DEF_ALPHA;
        beta = DEF_BETA;
        gamma = DEF_GAMMA;
        spread_radius = 1;
    }
        //////// The user has chosen to set the parameters himself ////////
    else
    {
        // only clusters have been specified leaving locations out
        if (!clusters_and_locations && locations == 0 && clusters > 0)
        {
            locations = clusters * 150;
        }
            // only locations have been specified leaving clusters out
        else if (!clusters_and_locations && locations > 0 && clusters == 0)
        {
            clusters = locations / 150;
        }
        else if (!clusters_and_locations && locations == 0 && clusters == 0) {
            std::cerr << "At least one parameter among 'clusters' and 'locations' must be specified." << std::endl;
            return EXIT_FAILURE;
        }

    }

    std::cout << "Clusters == " <<clusters<<"\t Locations == "<<locations<<std::endl;
    std::cout << "People == " <<people<<std::endl;
    std::cout << "S == " <<susceptibles<<"\nE == "<<exposed<<"\nI == "<<infected <<"\nR == "<<recovered<<std::endl;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    std::cout << "Width and height of your screen "<< std::endl;
    std::cout << sf::VideoMode::getDesktopMode().width <<std::endl;
    std::cout << sf::VideoMode::getDesktopMode().height <<std::endl;
    unsigned Graph_width = 800;
    Simulation prova {susceptibles,exposed,infected,recovered,clusters,locations,side,alpha,beta,gamma,spread_radius};
    //Simulation prova{23750,500,375,125,10,1000,1000,0.3,0.1,0.05,1};
    std::vector<Data> Result = {prova.get_data()};
    sf::RenderWindow window;
    std::cout << "I'm here";
    Display Window{prova,window,Graph_width};
    Window.Draw();
    window.display();
    /*for(auto& a: prova.world_ref().Clusters())
    {
        std::cout << "nth cluster: "
                  << " base: " << a.base() << " height: " << a.height() << " X: " << a.area().get_blh_corner().get_x()
                  << " Y: " << a.area().get_trh_corner().get_y() << std::endl;
    }*/
    int counter{};
    while (window.isOpen())
    {
        sf::Event event{};
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) window.close();
        }
        prova.move();
        prova.spread();
        Result.push_back(prova.get_data());
        if (counter % 10 == 0)
        {
            prova.update_zones();
            std::cout << counter / 10 << "nth cycle" << std::endl;
        }
        ++counter;
        window.clear(sf::Color::Black);
        Window.Draw();
        window.display();
    }

    // ROOT CODE

   /* TApplication app("app", &argc, argv);

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
    for (auto& a : Result)
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
    app.Run();*/
}