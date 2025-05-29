#include "../include/EulerIntegrator.h"
#include <omp.h>

void EulerIntegrator::integrateSystem(std::vector<Body*>& bodies, float dt) {
    // Parallel loop to update the position and velocity of each body
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < static_cast<int>(bodies.size()); ++i) {
        // Semi-implicit Euler integration
        bodies[i]->vel += bodies[i]->acc * dt; // Update velocity
        bodies[i]->pos += bodies[i]->vel * dt; // Update position
    }
}