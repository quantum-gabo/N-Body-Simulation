#pragma once
#include "../include/Body.h"
#include <vector>


class EulerIntegrator {
public:
    EulerIntegrator() = default;
    ~EulerIntegrator() = default;

    // Integrate function to update one body
    void integrateSystem(std::vector<Body*>& bodies, float dt);
};


