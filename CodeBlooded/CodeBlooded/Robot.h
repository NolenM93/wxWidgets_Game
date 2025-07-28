#pragma once
#include <string>
#include <cmath>
struct Vec2 {
	float x, y;
	Vec2(float x = 0, float y = 0) : x(x), y(y) {}

	float distanceTo(const Vec2& other) const {
		float dx = x - other.x;
		float dy = y - other.y;
		return std::sqrt(dx * dx + dy * dy);
	}
};

class Robot {
public:
	std::string name;
	Vec2 position;
	float health;
	bool shield;

	Robot(std::string name, Vec2 pos);

	void moveTo(const Vec2& target);
	void takeDamage(float amount);
	void activateShield();
	bool isAlive() const;
};
