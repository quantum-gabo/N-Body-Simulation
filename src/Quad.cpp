#include <vector>
#include "../include/Body.h"
#include "../include/Quad.h"
#include <limits>

Quad Quad::containing(const std::vector<Body*>& bodies) {
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();

    for (const auto& body : bodies) {
        minX = std::min(minX, body->pos.x);
        minY = std::min(minY, body->pos.y);
        maxX = std::max(maxX, body->pos.x);
        maxY = std::max(maxY, body->pos.y);
    }

    Vector2D center((minX + maxX) * 0.5f, (minY + maxY) * 0.5f);
    float size = std::max(maxX - minX, maxY - minY);
    return Quad(center, size);
}

int Quad::findQuadrant(const Vector2D& pos) const {
    // Special case for exact center point: assign to quadrant 0 by default
    return ((pos.y > center.y) << 1) | (pos.x > center.x);
}

Quad Quad::intoQuadrant(int quadrant) const {
    float newSize = size * 0.5f;
    float offsetX = ((quadrant & 1) - 0.5f) * newSize;
    float offsetY = (((quadrant >> 1) & 1) - 0.5f) * newSize;
    return Quad(Vector2D(center.x + offsetX, center.y + offsetY), newSize);
}

std::array<Quad, 4> Quad::subdivide() const {
    return { intoQuadrant(0), intoQuadrant(1), intoQuadrant(2), intoQuadrant(3) };
}
