#pragma once

#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>

/**
 * @brief Benchmark class for comparing N-body simulation methods.
 * 
 * This class supports benchmarking the Barnes-Hut approximation and the direct force calculation.
 * It records average timings for various particle counts and saves results to CSV files.
 */
class Benchmark {
public:
    std::vector<std::chrono::duration<double>> run_times;
    std::vector<std::chrono::duration<double>> total_times;

    std::vector<int> n_particles = {
        100, 500, 1000, 2000, 4000, 8000, 10000, 15000,
        20000, 30000, 40000, 50000, 60000, 80000, 100000
    };

    /**
     * @brief Runs the Barnes-Hut benchmark and writes results to file.
     */
    void BarnesHutBenchmarkStart();

    /**
     * @brief Runs the Direct Method benchmark and writes results to file.
     */
    void DirectMethodBenchmarkStart();

private:
    /**
     * @brief Writes the benchmark results to a CSV file.
     * @param filename The path to the file where results will be saved.
     */
    void writeResultsToFile(const std::string& filename);
};
