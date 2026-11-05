#pragma once

#include "Vec2.h"

#include <string>

class Robot {
public:
    static constexpr float kMaxHealth = 100.0f;
    static constexpr float kMoveStep = 1.5f;
    static constexpr float kArriveEpsilon = 0.1f;
    static constexpr float kShieldDamageFactor = 0.5f;

    std::string name;
    Vec2 position;
    float health = kMaxHealth;
    bool shield = false;

    Robot(std::string name, Vec2 pos);

    void moveTo(const Vec2& target);
    void takeDamage(float amount);
    void activateShield();
    void clampToArena(float width, float height);
    bool isAlive() const;
};
