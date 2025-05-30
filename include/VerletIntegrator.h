#pragma once
#include <vector>
#include "../include/Body.h"

/**
 * @brief VerletIntegrator class for performing Verlet integration on a system of bodies.
 * Verlet integration is a numerical method used to simulate the motion of bodies under the influence of forces.
 * It is a symplectic integrator, which means it preserves the Hamiltonian structure of the system, making it suitable for long-term simulations of conservative systems.
 * This class updates the position and velocity of each body based on its acceleration and previous position.
 * The integration is performed in parallel using OpenMP for improved performance.
 */
class VerletIntegrator {
public:

    /**
     * @brief Default constructor for VerletIntegrator.
     * Initializes the integrator with default settings.
     */
    VerletIntegrator() = default;
    /**
     * @brief Default destructor for VerletIntegrator.
     * Cleans up any resources if necessary (not needed in this case).
     */
    ~VerletIntegrator() = default;

    /**
     * @brief Integrates the system of bodies using the Verlet method.
     * This method updates the position and velocity of each body based on its acceleration and previous position.
     * The integration is performed in parallel for improved performance.
     * 
     * @param bodies A vector of pointers to Body objects representing the system of bodies.
     * @param dt The time step for the integration, in years.
     */
    void integrateSystem(std::vector<Body*>& bodies, float dt);


    /**
     * @brief Initializes the previous position for each body in the system.
     * This is necessary for Verlet integration, as it requires the previous position to compute the new position.
     * The method updates each body's previous position and initial position based on its current velocity and acceleration.
     * 
     * @param bodies A vector of pointers to Body objects representing the system of bodies.
     * @param dt The time step for the integration, in years.
     */
    void initPrevPos(std::vector<Body*>& bodies, float dt);
};
