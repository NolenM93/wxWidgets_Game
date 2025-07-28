#pragma once
#include <string>
#include <cmath>
struct Vec2 // A simple 2D vector class for position and movement
{
	float x, y;
	Vec2(float x = 0, float y = 0) : x(x), y(y) {}
	// Overload the addition operator for vector addition
	float distanceTo(const Vec2& other) const {
		float dx = x - other.x;
		float dy = y - other.y;
		return std::sqrt(dx * dx + dy * dy);
	}
};

class Robot {
public:
	std::string name;// Name of the robot
	Vec2 position;// Current position of the robot
	float health;// Current health of the robot
	bool shield;// Whether the robot's shield is active

	Robot(std::string name, Vec2 pos);// Constructor to initialize the robot with a name and position

	void moveTo(const Vec2& target);// Move the robot towards a target position
	void takeDamage(float amount);// Apply damage to the robot, reducing its health
	void activateShield();// Activate the robot's shield, preventing damage for one turn
	bool isAlive() const;// Check if the robot is still alive (health > 0)
};
