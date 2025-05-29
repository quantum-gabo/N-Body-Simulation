#pragma once

#include "../include/Vector.h"
#include "../include/Quad.h"

/**
 * @brief Node struct representing a node in the quadtree for Barnes-Hut algorithm.
 * Each node contains information about its position, mass, children, and the bounding quad.
 * It also provides methods to check if the node is a leaf or empty.
 */
struct Node {
    int children = 0;         // Index of the first child node, 0 if no children
    int next = 0;             // Index of the next node in the list
    Vector2D pos = {0, 0};    // Position of the node (center of mass)
    float mass = 0.0f;        // Total mass of bodies in this node
    Quad quad;                // Bounding quad for this node

    /**
     * @brief Default constructor for Node.
     * Initializes children to 0, next to 0, pos to (0, 0), mass to 0.0f,
     * and quad to a default Quad centered at (0, 0) with size 1.0.
     */
    Node(int next, const Quad& quad) : next(next), quad(quad) {}

    /**
     * @brief Checks if this node is a leaf node.
     * A leaf node has no children, meaning it does not contain any further subdivisions.
     * @return True if the node is a leaf, false otherwise.
     */
    bool isLeaf() const { return children == 0; }

    /**
     * @brief Checks if this node is empty.
     * An empty node has no mass, meaning it does not contain any bodies.
     * @return True if the node is empty, false otherwise.
     */
    bool isEmpty() const { return mass == 0.0f; }
};
