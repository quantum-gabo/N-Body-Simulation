#include "../include/Quadtree.h"
#include "../include/Node.h"
#include "../include/Constants.h"
#include <cmath>

Quadtree::Quadtree(float theta, float epsilon)
    : theta(theta), epsilon(epsilon) {}

void Quadtree::clear(const Quad& rootQuad) {
    nodes.clear();
    parents.clear();
    // Start with one root node (index 0)
    nodes.emplace_back(0, rootQuad);
}

int Quadtree::subdivide(int node) {
    parents.push_back(node);
    int children = static_cast<int>(nodes.size());
    nodes[node].children = children;

    // Get four sub-quads from current node's quad
    auto quads = nodes[node].quad.subdivide();
    int nexts[] = { children + 1, children + 2, children + 3, nodes[node].next };

    // Create 4 child nodes with appropriate 'next' indices and quads
    for (int i = 0; i < 4; ++i) {
        nodes.emplace_back(nexts[i], quads[i]);
    }
    return children;
}

void Quadtree::insert(const Vector2D& pos, float mass) {
    int node = ROOT;

    // Descend tree to find appropriate leaf node
    while (!nodes[node].isLeaf()) {
        int quadrant = nodes[node].quad.findQuadrant(pos);
        node = nodes[node].children + quadrant;
    }

    // If leaf is empty, place mass here
    if (nodes[node].isEmpty()) {
        nodes[node].pos = pos;
        nodes[node].mass = mass;
        return;
    }

    // If same position, accumulate mass
    if (nodes[node].pos.x == pos.x && nodes[node].pos.y == pos.y) {
        nodes[node].mass += mass;
        return;
    }

    // Otherwise, subdivide until bodies are separated
    while (true) {
        int children = subdivide(node);
        int q1 = nodes[node].quad.findQuadrant(nodes[node].pos);
        int q2 = nodes[node].quad.findQuadrant(pos);

        // Move old body's data into correct child
        nodes[children + q1].pos = nodes[node].pos;
        nodes[children + q1].mass = nodes[node].mass;

        if (q1 == q2) {
            node = children + q1;
        } else {
            // Insert new body into correct child and finish
            nodes[children + q2].pos = pos;
            nodes[children + q2].mass = mass;
            return;
        }
    }
}

void Quadtree::propagate() {
    // Iterate parents in reverse order (bottom-up)
    for (int i = static_cast<int>(parents.size()) - 1; i >= 0; --i) {
        int node = parents[i];
        int c = nodes[node].children;

        Vector2D p(0.f, 0.f);
        float m = 0.f;
        // Sum mass and weighted positions of children
        for (int j = 0; j < 4; ++j) {
            p += nodes[c + j].pos * nodes[c + j].mass;
            m += nodes[c + j].mass;
        }

        nodes[node].mass = m;
        nodes[node].pos = p / m; // Center of mass
    }
}


Vector2D Quadtree::computeAcceleration(const Vector2D& pos) const {
    Vector2D acc(0.0f, 0.0f);
    computeAccelerationRecursive(ROOT, pos, acc);
    return acc;
}

void Quadtree::computeAccelerationRecursive(int nodeIndex, const Vector2D& pos, Vector2D& acc) const {
    const Node& node = nodes[nodeIndex];

    if (node.isEmpty()) return;

    Vector2D direction = node.pos - pos;
    float distSqr = direction.magSqr();
    float dist = std::sqrt(distSqr + epsilon);  // Softening included here

    // Use Barnes-Hut criterion or if leaf node
    if (node.isLeaf() || (node.quad.size / dist < theta)) {
        float invDist = 1.0f / dist;
        float invDist3 = invDist * invDist * invDist;
        acc += direction * (Constants::G * node.mass * invDist3);
    } else {
        // Traverse children nodes recursively
        for (int i = 0; i < 4; ++i) {
            computeAccelerationRecursive(node.children + i, pos, acc);
        }
    }
}
