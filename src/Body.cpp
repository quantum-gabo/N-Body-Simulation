#include "../include/Body.h"

Body::Body(float mass, float radius, const Vector2D& pos, const Vector2D& vel, const Vector2D& acc)
    : mass(mass), radius(radius), pos(pos), vel(vel), acc(acc) {}

