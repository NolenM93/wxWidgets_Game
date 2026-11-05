#pragma once

#include "Robot.h"

#include <string>
#include <vector>

// Tiny line-oriented bot language. Each non-comment line is one command,
// and every command on the script runs once per simulation tick.
//
//   moveTo(x, y)
//   moveToNearest()
//   activateShield()
//   takeDamage(amount)
//   attackNearest(range, damage)
class ScriptEngine {
public:
    void loadScript(const std::string& code);
    const std::string& script() const { return script_; }

    void execute(Robot& bot, std::vector<Robot>& others) const;

    static Robot* findNearestAlive(Robot& bot, std::vector<Robot>& others);

private:
    std::string script_;

    static void runLine(const std::string& line, Robot& bot, std::vector<Robot>& others);
    static std::string stripWhitespace(std::string line);
    static bool parseMoveCommand(const std::string& line, Vec2& out);
    static bool parseOneFloat(const std::string& line, const char* name, float& out);
    static bool parseTwoFloats(const std::string& line, const char* name, float& a, float& b);
};
