/*
2D vector class for tracking the position, velocity, and acceleration of a particle.
*/
#pragma once
#include <cmath>

/**
 * @brief Vector2D class for representing 2D vectors.
 * This class provides basic vector operations such as addition, subtraction,
 * scalar multiplication, and magnitude calculation. It also supports operator overloading
 * for intuitive vector arithmetic.
 */
class Vector2D {
    public:
        float x, y; // Components of the vector

        /**
         * @brief Default constructor for Vector2D. 
         * Initializes the vector to (0, 0).
         * @param x X component of the vector.
         * @param y Y component of the vector.
         */
        Vector2D();

        /**
         * @brief Constructor for Vector2D with specified components.
         * Initializes the vector with given x and y values.
         * @param x X component of the vector.
         * @param y Y component of the vector.
         */
        Vector2D(float x, float y);

        /**
         * @brief Destructor for Vector2D.
         * Cleans up any resources if necessary (not needed in this case).
         */
        ~Vector2D();

        // Operator overloads for vector arithmetic 

        // Vector addtion operator
        inline Vector2D operator+(const Vector2D& v) const {
            return Vector2D(x + v.x, y + v.y);
        }

        // Vector subtraction operator
        inline Vector2D operator-(const Vector2D& v) const {
            return Vector2D(x - v.x, y - v.y);
        }

        // Scalar multiplication operator
        inline Vector2D operator/(float scalar) const
        {
            return Vector2D(x / scalar, y / scalar);
        }
        
        // Scalar multiplication operator
        inline Vector2D& operator+=(const Vector2D& v) {
            x += v.x;
            y += v.y;
            return *this;
        }

        // Scalar subtraction operator
        inline Vector2D& operator-=(const Vector2D& v) {
            x -= v.x;
            y -= v.y;
            return *this;
        }

        // Scalar multiplication operator
        inline Vector2D& operator*=(float scalar) {
            x *= scalar;
            y *= scalar;
            return *this;
        }

        // Comparison operators
        inline bool operator==(const Vector2D& v) const {
            return (x == v.x && y == v.y);
        }

        // Member functions

        // Computes the magnitude of the vector
        inline float mag() const {
            return sqrt(x * x + y * y);
        }

        // Computes the squared magnitude of the vector
        inline float magSqr() const {
            return x * x + y * y;
        }

        // Resets the vector to zero
        inline Vector2D& Zero() {
            x = 0.0f;
            y = 0.0f;
            return *this;
        }
};

// Non-member functions

// Scalar multiplication operator for Vector2D
inline Vector2D operator*(const Vector2D& vec, float scalar) {
    return Vector2D(vec.x * scalar, vec.y * scalar);
}
// Scalar multiplication operator for Vector2D (commutative)
inline Vector2D operator*(float scalar, const Vector2D& vec) {
    return Vector2D(vec.x * scalar, vec.y * scalar);
}

