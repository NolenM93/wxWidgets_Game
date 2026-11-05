#include "Robot.h"

#include <algorithm>
#include <utility>

Robot::Robot(std::string name, Vec2 pos)
    : name(std::move(name)), position(pos), health(kMaxHealth), shield(false) {}

void Robot::moveTo(const Vec2& target) {
    const float dx = target.x - position.x;
    const float dy = target.y - position.y;
    const float distance = std::sqrt(dx * dx + dy * dy);
    if (distance <= kArriveEpsilon) {
        return;
    }
    position.x += kMoveStep * (dx / distance);
    position.y += kMoveStep * (dy / distance);
}

void Robot::takeDamage(float amount) {
    if (amount < 0.0f) {
        amount = 0.0f;
    }
    if (shield) {
        amount *= kShieldDamageFactor;
    }
    health -= amount;
    if (health < 0.0f) {
        health = 0.0f;
    }
}

void Robot::activateShield() {
    shield = true;
}

void Robot::clampToArena(float width, float height) {
    position.x = std::max(0.0f, std::min(position.x, width));
    position.y = std::max(0.0f, std::min(position.y, height));
}

bool Robot::isAlive() const {
    return health > 0.0f;
}
