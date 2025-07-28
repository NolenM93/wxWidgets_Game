#pragma once
#include "Robot.h"
#include <vector>
#include <string>

class ScriptEngine {
public:
    void loadScript(const std::string& code);
    void execute(Robot& bot, const std::vector<Robot>& others);

private:
    std::string script;
    Vec2 parseMoveCommand(const std::string& line);
};



