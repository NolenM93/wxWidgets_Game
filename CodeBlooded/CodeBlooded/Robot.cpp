#include "Robot.h"
#include <cmath>

Robot::Robot(std::string name, Vec2 pos)// Constructor to initialize the robot with a name and position
	: name(std::move(name)), position(pos), health(100.0f), shield(false) {
}

void Robot::moveTo(const Vec2& target) // Move the robot towards a target position
{
	float dx = target.x - position.x;
	float dy = target.y - position.y;
	float distance = std::sqrt(dx * dx + dy * dy);
	if (distance > 0.1f) {
		float step = 1.5f; // Move 1.5 units per tick
		position.x += step * (dx / distance);
		position.y += step * (dy / distance);
	}
}

void Robot::takeDamage(float amount) // Apply damage to the robot, reducing its health
{
	if (shield) amount *= 0.5f;
	health -= amount;
	if (health < 0) health = 0;
}

void Robot::activateShield() // Activate the robot's shield, preventing damage for one turn
{
	shield = true;
}

bool Robot::isAlive() const // Check if the robot is still alive (health > 0)
{
	return health > 0;
}