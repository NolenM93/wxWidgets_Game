#include "ScriptEngine.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {

bool parseFloat(const std::string& text, float& out) {
    try {
        std::size_t idx = 0;
        out = std::stof(text, &idx);
        return idx > 0;
    } catch (const std::exception&) {
        return false;
    }
}

std::string argsInside(const std::string& line, const char* name) {
    const std::string prefix = std::string(name) + "(";
    if (line.compare(0, prefix.size(), prefix) != 0) {
        return {};
    }
    if (line.size() < prefix.size() + 1 || line.back() != ')') {
        return {};
    }
    return line.substr(prefix.size(), line.size() - prefix.size() - 1);
}

}  // namespace

void ScriptEngine::loadScript(const std::string& code) {
    script_ = code;
}

Robot* ScriptEngine::findNearestAlive(Robot& bot, std::vector<Robot>& others) {
    Robot* best = nullptr;
    float bestDist = 0.0f;
    for (Robot& other : others) {
        if (&other == &bot || !other.isAlive()) {
            continue;
        }
        const float dist = bot.position.distanceTo(other.position);
        if (!best || dist < bestDist) {
            best = &other;
            bestDist = dist;
        }
    }
    return best;
}

std::string ScriptEngine::stripWhitespace(std::string line) {
    line.erase(std::remove_if(line.begin(), line.end(),
                              [](unsigned char c) { return std::isspace(c) != 0; }),
               line.end());
    return line;
}

bool ScriptEngine::parseMoveCommand(const std::string& line, Vec2& out) {
    return parseTwoFloats(line, "moveTo", out.x, out.y);
}

bool ScriptEngine::parseOneFloat(const std::string& line, const char* name, float& out) {
    const std::string inner = argsInside(line, name);
    if (inner.empty()) {
        return false;
    }
    return parseFloat(inner, out);
}

bool ScriptEngine::parseTwoFloats(const std::string& line, const char* name, float& a, float& b) {
    const std::string inner = argsInside(line, name);
    const auto comma = inner.find(',');
    if (comma == std::string::npos) {
        return false;
    }
    return parseFloat(inner.substr(0, comma), a) && parseFloat(inner.substr(comma + 1), b);
}

void ScriptEngine::runLine(const std::string& rawLine, Robot& bot, std::vector<Robot>& others) {
    const std::string line = stripWhitespace(rawLine);
    if (line.empty() || line.front() == '#' || line.compare(0, 2, "//") == 0) {
        return;
    }

    Vec2 target;
    if (parseMoveCommand(line, target)) {
        bot.moveTo(target);
        return;
    }

    if (line == "moveToNearest()") {
        if (Robot* nearest = findNearestAlive(bot, others)) {
            bot.moveTo(nearest->position);
        }
        return;
    }

    if (line == "activateShield()") {
        bot.activateShield();
        return;
    }

    float amount = 0.0f;
    if (parseOneFloat(line, "takeDamage", amount)) {
        bot.takeDamage(amount);
        return;
    }

    float range = 0.0f;
    float damage = 0.0f;
    if (parseTwoFloats(line, "attackNearest", range, damage)) {
        if (Robot* nearest = findNearestAlive(bot, others)) {
            if (bot.position.distanceTo(nearest->position) <= range) {
                nearest->takeDamage(damage);
            }
        }
    }
}

void ScriptEngine::execute(Robot& bot, std::vector<Robot>& others) const {
    if (!bot.isAlive()) {
        return;
    }

    std::istringstream stream(script_);
    std::string line;
    while (std::getline(stream, line)) {
        runLine(line, bot, others);
        if (!bot.isAlive()) {
            return;
        }
    }
}
