#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <iostream>
#include <cmath>


struct Vec2 {
    float x, y;
    Vec2(float x_=0, float y_=0) : x(x_), y(y_) {}
    Vec2 operator-(const Vec2& other) const { return Vec2(x - other.x, y - other.y); }
    float abs() const { return std::sqrt(x*x + y*y); }
};

struct AABB {
    Vec2 min;
    Vec2 max;
};

struct Object {
    Vec2 pos;
    AABB aabb;
};

struct Manifold {
    Object *A, *B;
    Vec2 normal;
    float penetration;
};

// Compare objects
bool AABBvsAABB( Manifold *m );

// Function to convert Vec2 to sf::Vector2f
sf::Vector2f toSF(const Vec2& v);

// Function to get dot product 
float Dot(const Vec2& a, const Vec2& b);

#endif // PHYSICS_HPP