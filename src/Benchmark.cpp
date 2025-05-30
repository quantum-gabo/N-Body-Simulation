#include "../include/Benchmark.h"
#include "../include/Utils.h"
#include "../include/Quadtree.h"
#include "../include/Forces.h"

void Benchmark::BarnesHutBenchmarkStart() {
    std::cout << "Starting Barnes-Hut Benchmark..." << std::endl;
    total_times.clear();
    for (int n : n_particles) {
        run_times.clear();
        std::vector<Body*> bodies;
        UniformDisk(bodies, n);

        for (int i = 0; i < 100; ++i) {
            Quadtree tree(Constants::THETA, Constants::SOFTENING);

            auto start = std::chrono::high_resolution_clock::now();
            Forces::calculateBarnesHutForces(bodies, tree);
            auto end = std::chrono::high_resolution_clock::now();

            run_times.push_back(end - start);
        }

        std::chrono::duration<double> total_time = std::chrono::duration<double>::zero();
        for (const auto& duration : run_times) {
            total_time += duration;
        }

        total_times.push_back(total_time / run_times.size());
        for (Body* b : bodies) delete b; // Delete bodies after all runs for this particle count
    }

    writeResultsToFile("../results/barnes_hut_benchmark_results.csv");
    std::cout << "Benchmark completed." << std::endl;
}

void Benchmark::DirectMethodBenchmarkStart() {
    std::cout << "Starting Direct Method Benchmark..." << std::endl;
    total_times.clear();
    for (int n : n_particles) {
        run_times.clear();
        std::vector<Body*> bodies;
        UniformDisk(bodies, n);

        for (int i = 0; i < 100; ++i) {
            auto start = std::chrono::high_resolution_clock::now();
            Forces::calculateDirectForces(bodies);
            auto end = std::chrono::high_resolution_clock::now();

            run_times.push_back(end - start);
        }

        std::chrono::duration<double> total_time = std::chrono::duration<double>::zero();
        for (const auto& duration : run_times) {
            total_time += duration;
        }

        total_times.push_back(total_time / run_times.size());
        for (Body* b : bodies) delete b; // Delete bodies after all runs for this particle count

    }

    writeResultsToFile("../results/direct_method_benchmark_results.csv");
    std::cout << "Benchmark completed." << std::endl;
}

void Benchmark::writeResultsToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    file << "Number of Particles, Duration (seconds)\n";
    for (size_t i = 0; i < n_particles.size(); ++i) {
        file << n_particles[i] << ", "
             << std::fixed << std::setprecision(6)
             << total_times[i].count() << "\n";
    }

    file.close(); // Optional, explicit is good practice
}
