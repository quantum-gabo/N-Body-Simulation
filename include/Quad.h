#pragma once
#include "../include/Vector.h"
#include "../include/Body.h"
#include <vector>
#include <array>

/**
 * @brief Quad class for representing a quadrilateral region in 2D space. 
 * used in Barnes-Hut algorithm for spatial partitioning.
 * This class defines a quad with a center point and size, and provides methods
 * to find quadrants, subdivide the quad, and create a quad that contains a set of bodies.
 */
class Quad {
public:
    Vector2D center; // Center point of the quad
    float size;      // Size of the quad (length of one side)

    /**
     * @brief Default constructor for Quad.
     * Initializes center to (0, 0) and size to 1.0.
     * @param c Center point of the quad.
     * @param s Size of the quad.
     */
    Quad(const Vector2D& c, float s) : center(c), size(s) {}

    /**
     * @brief Static method to create a Quad that contains all bodies in the vector.
     * Computes the bounding box that contains all bodies' positions.
     * @param bodies Vector of pointers to Body objects.
     * @return A Quad that contains all bodies.
     */
    static Quad containing(const std::vector<Body*>& bodies);

    /**
     * @brief Finds the quadrant index for a given position within this quad.
     * Quadrants are numbered as follows:
     * 0: Top-left, 1: Top-right, 2: Bottom-left, 3: Bottom-right.
     * @param pos Position to find the quadrant for.
     * @return The index of the quadrant (0-3).
     */
    int findQuadrant(const Vector2D& pos) const;

    /**
     * @brief Creates a new Quad that represents a specific quadrant of this quad.
     * @param quadrant Quadrant index (0-3).
     * @return A new Quad representing the specified quadrant.
     */
    Quad intoQuadrant(int quadrant) const;

    /**
     * @brief Subdivides this quad into 4 smaller quads.
     * Each smaller quad represents one of the four quadrants.
     * @return An array of 4 Quads representing the subdivisions.
     */
    std::array<Quad, 4> subdivide() const;
};
