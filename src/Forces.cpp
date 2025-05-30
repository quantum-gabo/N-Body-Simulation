#include "../include/Forces.h"
#include "../include/Constants.h"
#include <cmath>
#include <omp.h>
#include <iostream>

// Cache the gravitational constant and softening factor
const float G = Constants::G;
const float SOFTENING = Constants::SOFTENING;

void Forces::calculateDirectForces(std::vector<Body*>& bodies) {
    const size_t n = bodies.size();

    // Reset accelerations in parallel (static schedule for uniform work)
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < n; ++i) {
        bodies[i]->acc.Zero();
    }

    // Calculate forces in parallel (static schedule, better for uniform workload)
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < n; ++i) {
        Vector2D localAcc(0.f, 0.f);
        const Vector2D& pos_i = bodies[i]->pos;

        // Use a local reference to avoid repeated access in loop
        #pragma omp simd
        for (size_t j = 0; j < n; ++j) {
            if (i == j) continue;

            const Vector2D& pos_j = bodies[j]->pos;
            const float mass_j = bodies[j]->mass;

            Vector2D direction = pos_j - pos_i;

            const float distSqr = direction.magSqr() + SOFTENING;
            const float invDist = 1.0f / std::sqrt(distSqr);
            const float invDist3 = invDist * invDist * invDist;

            localAcc += direction * (G * mass_j * invDist3);
        }
        bodies[i]->acc = localAcc;
    }
}

void Forces::calculateBarnesHutForces(std::vector<Body*>& bodies,Quadtree& tree) {
    if (bodies.empty()) return;

    // Build root bounding quad from all bodies
    const Quad rootQuad = Quad::containing(bodies);

    tree.clear(rootQuad);

    for (Body* body : bodies) {
        tree.insert(body->pos, body->mass);
    }
    tree.propagate();

    // Reset accelerations in parallel (static schedule)
    #pragma omp parallel for schedule(static)
    for (size_t i = 0; i < bodies.size(); ++i) {
        bodies[i]->acc.Zero();
    }

    // Compute accelerations in parallel (static schedule)
    #pragma omp parallel for schedule(dynamic)
    for (size_t i = 0; i < bodies.size(); ++i) {
        bodies[i]->acc = tree.computeAcceleration(bodies[i]->pos);
    }
}
