#pragma once
#include <vector>
#include "../include/Body.h"
#include "../include/Constants.h"

/**
 * @brief Computes the total energy of the system.
 * This function calculates the total energy of a system of bodies, including both kinetic and potential energy.
 * The kinetic energy is computed as 0.5 * m * v² for each body, and the potential energy is computed as -G * sum_{i<j} (m_i * m_j / r_ij).
 * The function uses OpenMP for parallel computation to improve performance.
 */
double totEnergy(const std::vector<Body*>& bodies);


/**
 * @brief Logs the energy deviation of the system to a CSV file.
 * This function computes the total energy of the system and logs the deviation from a reference energy value
 * to a CSV file. The reference energy is computed at the initial time (t=0).
 * @param bodies A vector of pointers to Body objects representing the system of bodies.
 * @param time The current simulation time.
 * @param filename The name of the file to log the energy deviation. Default is "energy_log.csv".
 */
void logEnergyDeviation(const std::vector<Body*>& bodies, int step, const std::string& filename);

/**
 * @brief Writes the forces acting on each body to a file.
 * This function computes the forces acting on each body in the system and writes them to a specified file.
 * The forces are written in a CSV format with columns for body index, force_x, and force_y.
 * @param bodies A vector of pointers to Body objects representing the system of bodies.
 * @param filename The name of the file to write the forces to. Default is "forces_log.csv".
 */
void WriteForcesToFile(const std::vector<Body*>& bodies, const std::string& filename);

/**
 * @brief Sorts bodies by their distance from the central body.
 * This function sorts the bodies in ascending order based on their distance
 * from the origin (assumed to be the position of the central body).
 * It uses the squared magnitude to avoid unnecessary square root calculations.
 * @param bodies A vector of pointers to Body objects representing the system of bodies.
 */
void sortByDistance(std::vector<Body*>& bodies);

/**
 * @brief Assigns initial velocities to bodies based on their mass and position.
 * This function calculates the initial velocity of each body in the system
 * based on its distance from the central body and the total mass of all bodies up
 * to that distance. The velocity is computed using the formula v = sqrt(G * M (<r0)/ r),
 * This method assumes an ordered system and checks for a central body. 
 * @param bodies A vector of pointers to Body objects representing the system of bodies.
 */
void assignInitVelocities(std::vector<Body*>& bodies);

// ============ Simulation Initializers ============

/**
 * @brief Initializes a solar system with a central star and several planets.
 * This function creates a solar system with a central star (the Sun) and several planets
 * with specified masses, radii, distances, and speeds. The planets are initialized in circular orbits.
 * @param bodies A vector of pointers to Body objects representing the system of bodies.
 */
void SolarSystem(std::vector<Body*>& bodies);

/**
 * @brief Initializes a uniform disk of bodies around a central massive body.
 * This function creates a disk of bodies with random positions and velocities,
 * simulating a uniform distribution in an annular region around a central body.
 * The bodies are assigned masses and radii, and their velocities are calculated
 * to maintain stable circular orbits.
 * @param bodies A vector of pointers to Body objects representing the system of bodies.
 * @param count The number of bodies to create in the disk.
 */
void UniformDisk(std::vector<Body*>& bodies, int count);

/**
 * @brief Initializes a two-galaxy system with a specified number of bodies.
 * This function creates two galaxies, each with a central massive body and several smaller bodies.
 * The galaxies are positioned on opposite sides of the origin, and the bodies are assigned random positions
 * and velocities to simulate a realistic galaxy structure.
 * @param bodies A vector of pointers to Body objects representing the system of bodies.
 * @param totalCount The total number of bodies to create in both galaxies.
 */
void TwoGalaxySystem(std::vector<Body*>& bodies, int count);
