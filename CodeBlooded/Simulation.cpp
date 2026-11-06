#include "Simulation.h"

void Simulation::reset(const std::string& script) {
    script_ = script;
    robots_.clear();
    robots_.emplace_back("Alpha", Vec2(10.0f, 10.0f));
    robots_.emplace_back("Beta", Vec2(90.0f, 90.0f));
    engine_.loadScript(script);
    tickCount_ = 0;
}

void Simulation::tick() {
    for (Robot& bot : robots_) {
        engine_.execute(bot, robots_);
        bot.clampToArena(kWidth, kHeight);
    }
    ++tickCount_;
}

bool Simulation::hasLivingRobot() const {
    for (const Robot& bot : robots_) {
        if (bot.isAlive()) {
            return true;
        }
    }
    return false;
}
