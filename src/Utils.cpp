#include <algorithm>
#include <cmath>
#include <random>
#include <ctime>
#include <omp.h>
#include <iostream>
#include <fstream>
#include "../include/Utils.h"
#include "../include/Constants.h"


double computeTotalEnergy(const std::vector<Body*>& bodies) {
    double kineticEnergy = 0.0;
    double potentialEnergy = 0.0;

    #pragma omp parallel for reduction(+:kineticEnergy)
    for (size_t i = 0; i < bodies.size(); ++i) {
        const Body* b = bodies[i];
        double v2 = b->vel.x * b->vel.x + b->vel.y * b->vel.y;
        kineticEnergy += 0.5 * b->mass * v2;
    }
    // Potential energy: -G * sum_{i<j} m_i m_j / r_ij
    #pragma omp parallel for reduction(+:potentialEnergy) schedule(dynamic)
    for (size_t i = 0; i < bodies.size(); ++i) {
        for (size_t j = i + 1; j < bodies.size(); ++j) {
            const Body* a = bodies[i];
            const Body* b = bodies[j];
            double dx = a->pos.x - b->pos.x;
            double dy = a->pos.y - b->pos.y;
            double dist = std::sqrt(dx * dx + dy * dy + 1e-10); // avoid div by 0
            potentialEnergy -= (a->mass * b->mass) / dist;
        }
    }

    return kineticEnergy + potentialEnergy;
}

void logEnergyDeviation(const std::vector<Body*>& bodies, int step, const std::string& filename) {
    static bool initialized = false;
    static double initialEnergy = 0.0;

    double currentEnergy = computeTotalEnergy(bodies);

    if (!initialized) {
        initialEnergy = currentEnergy * Constants::DEFAULT_TIMESTEP; // properly store it
        std::ofstream file(filename);
        file << "step,total_energy,delta_energy" << std::endl;
        initialized = true;
    }

    if (step % 100 == 0) {
        double deltaE = (currentEnergy - initialEnergy) / initialEnergy;
        std::ofstream file(filename, std::ios::app);
        file << step << "," << currentEnergy << "," << deltaE << std::endl;
    }
}


void sortByDistance(std::vector<Body*>& bodies) {
    std::sort(bodies.begin(), bodies.end(), [](const Body* a, const Body* b){
        return a->pos.mag() < b->pos.mag();
    });
};

void assignInitVelocities(std::vector<Body*>& bodies) {
    float MASS = 0.0f;
    Vector2D centralPos(0.0f, 0.0f);
    Vector2D centralVel(0.0f, 0.0f);
    for (auto& body : bodies) {
        MASS += body->mass;
        if (body->isCentral) {
            centralPos = body->pos; // Store the position of the central body
            centralVel = body->vel; // Store the velocity of the central body
            continue;
            }
    
    Vector2D position = body->pos - centralPos; // Relative position to the central body
    
    float velocityMagnitude = std::sqrt(Constants::G * MASS / position.mag());
    body->vel *= velocityMagnitude;
    body->vel += centralVel; // Add the central body's velocity to the body's velocity
    }
}

/*
################### Initial Models for N-Body Simulation ###################
*/
void SolarSystem(std::vector<Body*>& bodies) {
    bodies.clear();
    const int n_bodies = 9; // 1 Sun + 8 Planets
    bodies.reserve(n_bodies);

    // Sun — mass in M☉, radius in AU
    float sunMass = 1.0f;  // in solar masses
    float sunRadius = 0.0f; //Not used.

    Body sun(sunMass, sunRadius, Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f), Vector2D(0.0f, 0.0f));
    sun.isCentral = true; // Mark as central body
    bodies.push_back(new Body(sun)); // Add sun to the system

    // Planet data — mass in M☉, radius scaled, distance in AU, speed in AU/year
    struct Planet {
        const char* name;
        float mass;     // in M☉
        float radius;   // in Earth radii (to be converted)
        float distance; // in AU
        float speed;    // in AU/year
    };

    std::vector<Planet> planets = {
        {"Mercury", 1.65e-7f, 0.0f,  0.39f,  2.4f},
        {"Venus",   2.45e-6f, 0.0f,  0.72f,  1.9f},
        {"Earth",   3.00e-6f, 0.0f,  1.00f,  1.0f},
        {"Mars",    3.20e-7f, 0.0f,  1.52f,  0.8f},
        {"Jupiter", 9.50e-4f, 0.0f,  5.20f,  0.43f},
        {"Saturn",  2.85e-4f, 0.0f,  9.58f,  0.32f},
        {"Uranus",  4.40e-5f, 0.0f,  19.2f,  0.23f},
        {"Neptune", 5.10e-5f, 0.0f,  30.1f,  0.18f},
    };

    for (const auto& planet : planets) {
        Vector2D pos(planet.distance, 0.0f); // Position in AU
        Vector2D vel(0.0f, planet.speed);    // Tangential velocity in AU/yr
        bodies.push_back(new Body(planet.mass, planet.radius, pos, vel, Vector2D(0.0f, 0.0f)));
    }
};

void UniformDisk(std::vector<Body*>& bodies, int count) {
    // Random engine and distributions
    bodies.clear();
    bodies.reserve(count);
    // std::random_device rd;
    std::mt19937 gen(42);
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * M_PI);
    std::uniform_real_distribution<float> unitDist(0.0f, 1.0f);
    std::uniform_real_distribution<float> massDist(0.5f, 50.0f);

    // Central massive body
    float centralMass = 4e6f;
    float centralRadius = std::cbrt(centralMass) * 1e3f;
    Vector2D centralPos(0.0f, 0.0f);
    Vector2D centralVel(0.0f, 0.0f);
    Vector2D centralAcc(0.0f, 0.0f);
    Body centralBody(centralMass, centralRadius, centralPos, centralVel, centralAcc);
    centralBody.isCentral = true; // Mark as central body
    bodies.push_back(new Body(centralBody)); // Add central body to the system

    // Disk configuration
    float innerRadius = 1.0f;
    float outerRadius = 10.0f; //std::sqrt(static_cast<float>(count)) * 0.5f;
    float t = innerRadius / outerRadius;

    for (int i = 0; i < count - 1; ++i) {
        // Sample position in polar coordinates (annular disk)
        float angle = angleDist(gen);
        float r = outerRadius * std::sqrt(t * t + (1.0f - t * t) * unitDist(gen));
        float x_unit = std::cos(angle);
        float y_unit = std::sin(angle);
        Vector2D position(r * x_unit, r * y_unit);

        // Sample mass and compute radius
        float mass = massDist(gen);
        float radius = std::cbrt(mass);

        // Stable circular orbit velocity
        //float orbitalSpeed = std::sqrt(Constants::G * centralMass / r);
        Vector2D velocity(-y_unit, x_unit); // Perpendicular to position vector
        bodies.push_back(new Body(mass, radius, position, velocity, Vector2D(0.0f, 0.0f)));
    }

    // Sort bodies by distance from the central body
    sortByDistance(bodies);

    // Assign initial velocities based on the Mass(<r)
    assignInitVelocities(bodies);
}

void TwoGalaxySystem(std::vector<Body*>& bodies, int totalCount) {
    const int countPerGalaxy = totalCount / 2;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * M_PI);
    std::uniform_real_distribution<float> unitDist(0.0f, 1.0f);
    std::uniform_real_distribution<float> massDist(0.01f, 0.09f);

    float innerRadius = 0.1f;
    float outerRadius = 5.0f;
    float t = innerRadius / outerRadius;
    float centralMassA = 1e6f;
    float centralMassB = 1e6f;

    struct Galaxy {
        Vector2D centerPos;
        Vector2D bulkVel;
        float centralMass = 0.0f; // Central mass for each galaxy
    };

    Galaxy galaxy1 = { Vector2D(-5.0f, 0.0f), Vector2D(0.0f, 100.0f), centralMassA};
    Galaxy galaxy2 = { Vector2D(5.0f, 0.0f), Vector2D(100.0f, -100.0f), centralMassB};

    auto createGalaxy = [&](const Galaxy& g) -> std::vector<Body*> {
        std::vector<Body*> galaxyBodies;

        float centralRadius = 0.0f;
        Body centralBody(g.centralMass, centralRadius, g.centerPos, g.bulkVel, Vector2D(0.0f, 0.0f));
        centralBody.isCentral = true; // Mark as central body
        galaxyBodies.push_back(new Body(centralBody)); // Add central body to the galaxy

        for (int i = 0; i < countPerGalaxy - 1; ++i) {
            float angle = angleDist(gen);
            float r = outerRadius * std::sqrt(t * t + (1.0f - t * t) * unitDist(gen));
            float x_unit = std::cos(angle);
            float y_unit = std::sin(angle);
            Vector2D relPos(r * x_unit, r * y_unit);
            Vector2D position = g.centerPos + relPos;

            float mass = massDist(gen);
            float radius = 0.0f;

            Vector2D velDir(-y_unit, x_unit); // Perpendicular to position vector
            galaxyBodies.push_back(new Body(mass, radius, position, velDir, Vector2D(0.0f, 0.0f)));
        }

        sortByDistance(galaxyBodies);
        assignInitVelocities(galaxyBodies); // Now mass(<r) only accounts for this galaxy

        return galaxyBodies;
    };

    std::vector<Body*> g1 = createGalaxy(galaxy1);
    std::vector<Body*> g2 = createGalaxy(galaxy2);

    bodies.clear();
    bodies.insert(bodies.end(), g1.begin(), g1.end());
    bodies.insert(bodies.end(), g2.begin(), g2.end());
}

void WriteForcesToFile(const std::vector<Body*>& bodies, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    file << "body_index,force_x,force_y\n";
    for (size_t i = 0; i < bodies.size(); ++i) {
        const Body* body = bodies[i];
        file << i << "," << body->acc.x << "," << body->acc.y << "\n";
    }
    file.close();
    std::cout << "Forces written to " << filename << std::endl;
}