#pragma once

#include <cmath>

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    Vec2() = default;
    Vec2(float x_, float y_) : x(x_), y(y_) {}

    float distanceTo(const Vec2& other) const {
        const float dx = x - other.x;
        const float dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    float length() const { return std::sqrt(x * x + y * y); }
};
