#pragma once

#include "Robot.h"
#include "ScriptEngine.h"

#include <string>
#include <vector>

// wx-free arena tick loop. ArenaPanel renders this; unit tests drive it directly.
class Simulation {
public:
    static constexpr float kWidth = 100.0f;
    static constexpr float kHeight = 100.0f;

    void reset(const std::string& script);
    void tick();

    const std::vector<Robot>& robots() const { return robots_; }
    std::vector<Robot>& robots() { return robots_; }
    const std::string& script() const { return script_; }
    int tickCount() const { return tickCount_; }
    bool hasLivingRobot() const;

private:
    std::vector<Robot> robots_;
    ScriptEngine engine_;
    std::string script_;
    int tickCount_ = 0;
};
