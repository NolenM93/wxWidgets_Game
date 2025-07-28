#include "ScriptEngine.h"
#include <sstream>
#include <algorithm>
#include <cctype>

void ScriptEngine::loadScript(const std::string& code) // Load a script from a string
{
    script = code;
}

void ScriptEngine::execute(Robot& bot, const std::vector<Robot>& others) // Execute the loaded script for a given robot and other robots in the arena
{
    if (!bot.isAlive()) return;

    std::istringstream stream(script);
    std::string line;
    while (std::getline(stream, line)) {
        // Remove whitespace
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        if (line.find("moveTo(") == 0) {
            Vec2 target = parseMoveCommand(line);
            bot.moveTo(target);
        }
        else if (line.find("activateShield()") == 0) {
            bot.activateShield();
        }
        else if (line.find("takeDamage(") == 0) {
            size_t start = line.find("(") + 1;
            size_t end = line.find(")");
            float dmg = std::stof(line.substr(start, end - start));
            bot.takeDamage(dmg);
        }
    }
}

Vec2 ScriptEngine::parseMoveCommand(const std::string& line) // Parse a move command from the script
{
    size_t start = line.find("(") + 1;
    size_t comma = line.find(",", start);
    size_t end = line.find(")", comma);
    float x = std::stof(line.substr(start, comma - start));
    float y = std::stof(line.substr(comma + 1, end - comma - 1));
    return Vec2(x, y);
}


