#pragma once
#include <SDL2/SDL.h>

/*
    * @file Constants.h
    * @brief Contains physical constants, simulation parameters, and SDL parameters.
    * 
    * This header file defines various constants used in the simulation, including physical constants,
    * numerical stability parameters, simulation parameters, Barnes-Hut parameters, and SDL parameters.
    */
namespace Constants {

    // --- Physical Constants (AU, M☉, years) ---
    constexpr double G = 4 * M_PI * M_PI;   // Gravitational constant in AU³ / (M☉ · year²)
    constexpr double AU = 1.0;              // Astronomical Unit (AU)
    constexpr double SOLAR_MASS = 1.0;      // Solar Mass (M☉)
    constexpr double YEAR = 1.0;            // Year as time unit

    // --- Numerical Stability ---
    constexpr double SOFTENING = 0.01;      // Softening parameter (AU²)

    // --- Simulation Parameters ---
    constexpr int    DEFAULT_BODY_COUNT = 20000; // Default number of bodies in the system
    constexpr double DEFAULT_TIMESTEP   = 0.00001;//0.000005;  // Time step (years)

    // --- Barnes-Hut Parameters ---
    constexpr double THETA        = 0.5;    // Barnes-Hut opening angle

    // --- Performance ---
    constexpr int THREAD_COUNT = 8;         // Thread pool size

    // SDL PARAMETERS
    constexpr int WINDOW_WIDTH  = 800;     // Window width
    constexpr int WINDOW_HEIGHT = 800;     // Window height
    constexpr SDL_Color BACKGROUND_COLOUR = {255, 255, 255, 255}; // White background
}
