#pragma once
#include "Robot.h"
#include <vector>
#include <string>

class ScriptEngine {
public:
	void loadScript(const std::string& code);// Load a script from a string
	void execute(Robot& bot, const std::vector<Robot>& others);// Execute the loaded script for a given robot and other robots in the arena

private:
	std::string script;// The loaded script code
	Vec2 parseMoveCommand(const std::string& line);// Parse a move command from the script
};



