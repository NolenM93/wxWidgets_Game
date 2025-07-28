#include "Robot.h"
#include <cmath>

Robot::Robot(std::string name, Vec2 pos)
	: name(std::move(name)), position(pos), health(100.0f), shield(false) {
}

void Robot::moveTo(const Vec2& target) {
	float dx = target.x - position.x;
	float dy = target.y - position.y;
	float distance = std::sqrt(dx * dx + dy * dy);
	if (distance > 0.1f) {
		float step = 1.5f; // Move 1.5 units per tick
		position.x += step * (dx / distance);
		position.y += step * (dy / distance);
	}
}

void Robot::takeDamage(float amount) {
	if (shield) amount *= 0.5f;
	health -= amount;
	if (health < 0) health = 0;
}

void Robot::activateShield() {
	shield = true;
}