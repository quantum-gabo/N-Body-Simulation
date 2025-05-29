#pragma once

#include "../include/Node.h"
#include "../include/Quad.h"
#include <vector>

/**
 * @brief Quadtree class for spatial partitioning in 2D space.
 * This class implements the Barnes-Hut algorithm for efficient force calculations
 * in N-body simulations. It allows for inserting bodies, propagating mass and
 * center of mass upwards, and computing gravitational acceleration at a given position.  
 */
class Quadtree {
public:
    float theta;                // Barnes-Hut opening angle
    float epsilon;              // Softening parameter to avoid singularities
    std::vector<Node> nodes;    // List of nodes in the quadtree
    std::vector<int> parents;   // List of parent nodes for each node

    static constexpr int ROOT = 0; // Index of the root node in the quadtree

    /**
     * @brief Default constructor for Quadtree.
     * Initializes the quadtree with a given theta and epsilon.
     * @param theta Barnes-Hut opening angle.
     * @param epsilon Softening parameter to avoid singularities.
     */
    Quadtree(float theta, float epsilon);

    /**
     * @brief Clears the quadtree, resetting it to an empty state.
     * @param rootQuad The bounding quad for the root node.
     */
    void clear(const Quad& rootQuad);

    /**
     * @brief Inserts a body with position and mass into the quadtree.
     * This method finds the appropriate leaf node for the body and inserts it,
     * subdividing nodes as necessary.
     * @param pos Position of the body to insert.
     * @param mass Mass of the body to insert.
     */
    void insert(const Vector2D& pos, float mass);

    /**
     * @brief Propagates mass and center of mass upwards from children to parents.
     * This method updates the parent nodes with the total mass and center of mass
     * based on their children nodes.
     */
    void propagate();

    /**
     * @brief Computes the gravitational acceleration at a given position using the quadtree.
     * This method traverses the quadtree recursively to accumulate the acceleration
     * contributions from all relevant nodes.
     * @param pos Position at which to compute the gravitational acceleration.
     * @return The computed gravitational acceleration as a Vector2D.
     */
    Vector2D computeAcceleration(const Vector2D& pos) const;

    /**
     * @brief Recursive helper function to compute gravitational acceleration.
     * This function traverses the quadtree recursively, accumulating the acceleration
     * contributions from nodes and their children.
     * @param nodeIndex Index of the current node in the quadtree.
     * @param pos Position at which to compute the gravitational acceleration.
     * @param acc Reference to a Vector2D to accumulate the acceleration.
     */
    void computeAccelerationRecursive(int nodeIndex, const Vector2D& pos, Vector2D& acc) const;

private:
    /**
     * @brief Subdivides a node into four children nodes.
     * This method creates four new child nodes for the given node and returns the index of the first child.
     * @param node Index of the node to subdivide.
     * @return Index of the first child node created.
     */
    int subdivide(int node);
};
