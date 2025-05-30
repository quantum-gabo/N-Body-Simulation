#pragma once
#include "../include/Body.h"
#include <vector>


/**
 * @brief EulerIntegrator class for performing Euler integration on a system of bodies.
 * This class implements the Semi-implicit Euler method for integrating the motion of bodies in a gravitational system.
 * It updates the position and velocity of each body based on its acceleration.
 * The integration is performed in parallel using OpenMP for improved performance.
 */
class EulerIntegrator {
public:
    /**
     * @brief Default constructor for EulerIntegrator.
     * Initializes the integrator with default settings.
     */
    EulerIntegrator() = default;
    /**
     * @brief Default destructor for EulerIntegrator.
     * Cleans up any resources if necessary (not needed in this case).
     */
    ~EulerIntegrator() = default;

    /**
     * @brief Integrates the system of bodies using the Semi-implicit Euler method.
     * This method updates the velocity and position of each body based on its acceleration.
     * The integration is performed in parallel for improved performance.
     * 
     * @param bodies A vector of pointers to Body objects representing the system of bodies.
     * @param dt The time step for the integration, in years.
     */
    void integrateSystem(std::vector<Body*>& bodies, float dt);
};


