#pragma once
#include "../include/Body.h"
#include "../include/Quadtree.h"
#include <vector>

/**
 * @brief Forces class for calculating gravitational forces between bodies.
 * This class provides methods for direct N^2 force calculations and Barnes-Hut O(n log n) force calculations
 * using a quadtree structure.
 */
class Forces {
public:
    
    /**
     * @brief Calculates direct N^2 gravitational forces between bodies.
     * This method computes the gravitational forces between all pairs of bodies using a direct N^2 approach.
     * It uses OpenMP for parallel processing to improve performance.
     * @param bodies Vector of pointers to Body objects representing the bodies in the system.
     * Each body will have its acceleration vector updated based on the gravitational forces exerted by all other bodies.
     * The gravitational constant and softening factor are used to avoid singularities.
     * @note This method assumes that the bodies vector is not empty.
     * It resets the acceleration of each body before calculating the forces.
     * @note The gravitational constant and softening factor are defined in Constants.h.
     * @see Constants::G for the gravitational constant.
     * @see Constants::SOFTENING for the softening factor to avoid singularities.
     * @see Body for the structure of the Body class.
     */
    static void calculateDirectForces(std::vector<Body*>& bodies);
    
    /**
     * @brief Calculates gravitational forces using the Barnes-Hut algorithm.
     * This method computes the gravitational forces between bodies using a quadtree structure
     * for efficient O(n log n) force calculations.
     * @param bodies Vector of pointers to Body objects representing the bodies in the system.
     * @param tree Quadtree used for Barnes-Hut force calculations.
     * @param theta Barnes-Hut opening angle parameter.
     * @param epsilon Softening parameter to avoid singularities.
     * @note This method assumes that the bodies vector is not empty and that the quadtree is properly initialized.
     */
    static void calculateBarnesHutForces(std::vector<Body*>& bodies,Quadtree& tree);
};
