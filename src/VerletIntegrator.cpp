#include <omp.h>
#include "../include/VerletIntegrator.h"
#include "../include/Body.h"

void VerletIntegrator::integrateSystem(std::vector<Body*>& bodies, float dt) {
    // Precompute dt^2
    float dt2 = dt * dt;
    // Parallel loop to update the position and velocity of each body
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < static_cast<int>(bodies.size()); ++i) {
        // Compute the velocity based on the current and previous positions
        bodies[i]->vel = (bodies[i]->pos - bodies[i]->_prevPos);

        // Reset the previous position to the current position
        bodies[i]->_prevPos = bodies[i]->pos;

        // Update Body's position
        bodies[i]->pos += bodies[i]->vel + bodies[i]->acc * dt2; // + O(dt^4)
    }
}


void VerletIntegrator::initPrevPos(std::vector<Body*>& bodies, float dt) {
    // Precompute dt^2
    float dt2 = dt * dt;

    // Verlet is not self-starting, so we need to initialise it
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < static_cast<int>(bodies.size()); ++i) {
        bodies[i]->_prevPos = bodies[i]->pos;

        bodies[i]->pos = bodies[i]->pos + bodies[i]->vel * dt + 0.5f * bodies[i]->acc * dt2; // + O(dt^3)
    }
}
