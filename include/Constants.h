#pragma once

#include <SDL2/SDL.h>  // Added to define SDL_Color
// #include <cmath>

namespace Constants {

    // --- Physical Constants (AU, M☉, years) ---
    constexpr double PI = 3.14159265358979323846;
    constexpr double G = 4 * PI * PI;       // Gravitational constant in AU³ / (M☉ · year²)
    constexpr double AU = 1.0;              // Astronomical Unit (AU)
    constexpr double SOLAR_MASS = 1.0;      // Solar Mass (M☉)
    constexpr double YEAR = 1.0;            // Year as time unit

    // --- Numerical Stability ---
    constexpr double SOFTENING = 0.05;      // Softening parameter (AU²)
    constexpr double MIN_DISTANCE = 0.5;   // Min distance to avoid singularities (AU)

    // --- Simulation Parameters ---
    constexpr int    DEFAULT_BODY_COUNT = 20000; // Default number of bodies in the system
    constexpr double DEFAULT_TIMESTEP   = 0.00001;//0.000005;  // Time step (years)
    constexpr int    DEFAULT_STEPS      = 1000;

    // --- Barnes-Hut Parameters ---
    constexpr double THETA        = 0.5;    // Barnes-Hut opening angle
    constexpr SDL_Color TREE_COLOUR = {180, 180, 180, 255}; // Light gray for tree nodes
    // constexpr int    MAX_DEPTH    = 20;     // Quadtree depth limit
    // constexpr int    MAX_CAPACITY = 1;      // Max bodies before subdivision
    // constexpr int    MIN_BODIES   = 8;      // Threading threshold
    // constexpr int    POOL_SIZE    = 1024;   // Default size for node memory pool

    // --- Spatial Domain ---
    //constexpr double SYSTEM_SIZE = 3.0;    // Domain size (AU)

    // --- Performance ---
    constexpr int THREAD_COUNT = 8;         // Thread pool size

    // SDL PARAMETERS
    constexpr int WINDOW_WIDTH  = 800;     // Window width
    constexpr int WINDOW_HEIGHT = 800;     // Window height
    constexpr SDL_Color BACKGROUND_COLOUR = {255, 255, 255, 255}; // White background
    constexpr SDL_Color DEFAULT_COLOUR = {0, 255, 255, 255}; // Cyan
    constexpr SDL_Color SUN_COLOUR = {255, 165, 0, 255}; // Orange    
}
