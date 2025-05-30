#pragma once

#include <cmath>

struct Vector2 {
    float x;
    float y;

    Vector2() : x(0), y(0) {}
    Vector2(float x, float y) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2 normalized() const {
        float mag = magnitude();
        return (mag > 0) ? Vector2(x / mag, y / mag) : Vector2();
    }
};
