#include "../include/Benchmark.h"
#include "../include/Window.h"
#include "../include/CLI11.hpp"
#include "../include/Constants.h"
#include "../include/VerletIntegrator.h"
#include "../include/EulerIntegrator.h"
#include "../include/Utils.h"
#include "../include/Quadtree.h"
#include "../include/Forces.h"
#include <omp.h>
#include <thread>
#include <chrono>


int main(int argc, char* argv[]) {
    CLI::App app{"N-Body Simulation"};
    omp_set_num_threads(Constants::THREAD_COUNT);  // Set the number of threads for OpenMP

    int n_bodies = Constants::DEFAULT_BODY_COUNT; // Default number of bodies in the simulation
    float time_step = Constants::DEFAULT_TIMESTEP; // Default time step for simulation
    int init_system = 0;// Flag for initializing two galaxies
    bool draw_nodes = false; // Flag for rendering quadtree nodes
    bool benchmark = false; // Flag for running benchmarks
    bool energy_deviation = false; // Flag for logging energy deviation
    bool write_forces = false; // Flag for writing forces to a file
    std::string integrator = "verlet"; // Default integrator type
    std::string method = "barnes-hut"; // Default method for force calculation


    // Define CLI options
    app.add_option("-N, --n-bodies", n_bodies, "Number of bodies in the simulation")
        ->default_val(Constants::DEFAULT_BODY_COUNT)
        ->check(CLI::Range(1, 100000)); 

    app.add_option("-t, --time-step", time_step, "Time step for simulation")
        ->default_val(Constants::DEFAULT_TIMESTEP)
        ->check(CLI::Range(0.000001f, 0.001f));

    app.add_option("-i, --init-system", init_system, "Initialise system type (0: Uniform Disk, 1: Two Galaxies, 2: Solar System)")
        ->default_val(0)
        ->check(CLI::Range(0, 2));
    
    app.add_flag("--dn", draw_nodes, "Render quadtree nodes for Barnes-Hut method");

    app.add_flag("--b", benchmark, "Run benchmarks for the simulation methods");

    app.add_option("-m, --method", method, "Method for force calculation (barnes-hut, direct)")
        ->default_val("barnes-hut")
        ->check(CLI::IsMember({"barnes-hut", "direct"}));

    app.add_option("-I, --integrator", integrator, "Integrator type (verlet, euler)")
        ->default_val("verlet")
        ->check(CLI::IsMember({"verlet", "euler"}));

    app.add_flag("--energy", energy_deviation, "Log energy deviation to a CSV file");

    app.add_flag("--write-forces", write_forces, "Write forces acting on bodies to a file");

    CLI11_PARSE(app, argc, argv);

    //============== Simulation Setup ==============
    
    // Integrators 
    VerletIntegrator verlet;
    EulerIntegrator euler;

    // Bodies vector
    std::vector<Body*> bodies;

    // Benchmarking
    // If benchmark flag is set, run the benchmarks and exit
    if (benchmark) {
        Benchmark benchmarkRunner;
        benchmarkRunner.BarnesHutBenchmarkStart();
        benchmarkRunner.DirectMethodBenchmarkStart();
        std::cout << "Benchmark completed successfully." << std::endl;
    
        return 0; // Exit after running benchmarks
    }
    
    // Initialize the system based on user input
    switch (init_system){
    case 0: // Uniform Disk
        UniformDisk(bodies, n_bodies);
        break;
    case 1: // Two Galaxies
        TwoGalaxySystem(bodies, n_bodies);
        break;
    case 2: // Solar System
        SolarSystem(bodies);
        break;
    
    default:
        std::cerr << "Invalid system type selected. Exiting." << std::endl;
        return 1;
    }


    // Create a window for rendering
    WindowSDL window(Constants::WINDOW_WIDTH, Constants::WINDOW_HEIGHT);
    Uint32 lastFrameTime = SDL_GetTicks();  // Initialize last frame time for FPS calculation
    int steps = 0; // Step counter for the simulation
    std::string filename;
    std::string force_file;
    if (method == "barnes-hut") {
        filename = "../results/energy_bh.csv";
        force_file = "../results/forces_bh.csv";
    } else {
        filename = "../results/energy_direct.csv";
        force_file = "../results/forces_direct.csv";
    }

    //============== Main Simulation Loop ==============

    // Create a quadtree for Barnes-Hut method
    Quadtree tree(Constants::THETA, Constants::SOFTENING);
  

    while (window.handleEvents()) {

        Uint32 currentFrameTime = SDL_GetTicks();
        float deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f; // Convert milliseconds to seconds
        lastFrameTime = currentFrameTime;
        
        float fps = (deltaTime > 0.0f) ? (1.0f / deltaTime) : 0.0f;


        // If the simulation is not paused, update the system
        if (!window.isPaused()) {
            // Energy logging
            if (energy_deviation) {
                logEnergyDeviation(bodies, steps, filename);
            }

            // Compute forces based on the selected method
            if (method == "barnes-hut") {
                Forces::calculateBarnesHutForces(bodies, tree);
            } else if (method == "direct") {
                Forces::calculateDirectForces(bodies);
            }

            if (integrator == "verlet") {
                if (steps == 0) {
                    verlet.initPrevPos(bodies, time_step);
                } else {
                    verlet.integrateSystem(bodies, time_step);
                } 
            } else if (integrator == "euler") {
                euler.integrateSystem(bodies, time_step);
            }

            if (write_forces && steps == 100){
                WriteForcesToFile(bodies, force_file);
            }
        }

        window.clear(); // Clear the window for the next frame
        // Render bodies
        window.renderBodies(bodies);
        // Render quadtree nodes if the flag is set
        if (draw_nodes && method == "barnes-hut") {
            window.renderQuads(tree.nodes);
        }
        // Render FPS and other information
        std::string infoText = "Bodies: " + std::to_string(bodies.size()) + " | Steps: " + std::to_string(steps);
        window.renderText(infoText, 10, 10, {0, 0, 0, 255});  // Black text with size 20

        // Display FPS
        std::string fpsText = "FPS: " + std::to_string(static_cast<int>(fps));
        window.renderText(fpsText, 10, 30, {0, 0, 0, 255});  // Black text with size 20

        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));  // Approx 60 FPS

        steps++; // Increment the step counter
    }
    // Cleanup bodies
    bodies.clear(); // Clear bodies vector
    return 0;
}
        



