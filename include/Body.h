#pragma once
#include "../include/Vector.h"

/**
 * @brief Body class representing a celestial body in the simulation.
 * This class contains properties such as position, velocity, acceleration,
 * mass, and radius. It also includes methods for initialization and destruction.
 */
class Body {
public:
    float mass;             // Mass of the body
    float radius;           // Radius of the body
    Vector2D pos;           // Position vector
    Vector2D vel;           // Velocity vector
    Vector2D acc;           // Acceleration vector
    Vector2D _prevPos;      // Previous position (for integrators)
    bool isCentral = false; // Flag to indicate if this is the central body


    /**
     * @brief Custom constructor for Body.
     * Initializes the body with specified mass, radius, position, velocity, and acceleration.
     * @param mass Mass of the body
     * @param radius Radius of the body
     * @param pos Initial position of the body
     * @param vel Initial velocity of the body
     * @param acc Initial acceleration of the body
     */
    Body(float mass, float radius, const Vector2D& pos, const Vector2D& vel, const Vector2D& acc);

    /**
     * Default destructor for Body.
     * Cleans up any resources if necessary.
     */
    ~Body() = default;
};
