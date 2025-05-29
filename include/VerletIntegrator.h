#pragma once
#include <vector>
#include "../include/Body.h"

class VerletIntegrator {
public:
    VerletIntegrator() = default;
    ~VerletIntegrator() = default;

    // Integrate function to update all bodies in parallel
    void integrateSystem(std::vector<Body*>& bodies, float dt);


    // Initialize the prev_pos of all bodies in parallel
    void initPrevPos(std::vector<Body*>& bodies, float dt);
};
