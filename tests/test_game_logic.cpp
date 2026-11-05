#include "Robot.h"
#include "ScriptEngine.h"
#include "Simulation.h"

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

namespace {

int g_failed = 0;
int g_passed = 0;

#define CHECK(cond)                                                          \
    do {                                                                     \
        if (cond) {                                                          \
            ++g_passed;                                                      \
        } else {                                                             \
            ++g_failed;                                                      \
            std::cerr << "FAIL " << __FILE__ << ":" << __LINE__ << "  "      \
                      << #cond << '\n';                                      \
        }                                                                    \
    } while (0)

#define CHECK_NEAR(actual, expected, eps)                                    \
    do {                                                                     \
        const double _a = static_cast<double>(actual);                       \
        const double _e = static_cast<double>(expected);                     \
        if (std::fabs(_a - _e) <= (eps)) {                                   \
            ++g_passed;                                                      \
        } else {                                                             \
            ++g_failed;                                                      \
            std::cerr << "FAIL " << __FILE__ << ":" << __LINE__              \
                      << "  " << #actual << " = " << _a                      \
                      << " expected " << _e << " +/- " << (eps) << '\n';     \
        }                                                                    \
    } while (0)

void testVec2() {
    const Vec2 a(0.0f, 0.0f);
    const Vec2 b(3.0f, 4.0f);
    CHECK_NEAR(a.distanceTo(b), 5.0f, 1e-5);
    CHECK_NEAR(b.length(), 5.0f, 1e-5);
    CHECK_NEAR(a.distanceTo(a), 0.0f, 1e-6);
}

void testRobotDefaultsAndMovement() {
    Robot bot("Alpha", Vec2(0.0f, 0.0f));
    CHECK(bot.name == "Alpha");
    CHECK_NEAR(bot.health, Robot::kMaxHealth, 1e-6);
    CHECK(!bot.shield);
    CHECK(bot.isAlive());

    bot.moveTo(Vec2(0.0f, 0.0f));
    CHECK_NEAR(bot.position.x, 0.0f, 1e-6);
    CHECK_NEAR(bot.position.y, 0.0f, 1e-6);

    bot.moveTo(Vec2(10.0f, 0.0f));
    CHECK_NEAR(bot.position.x, Robot::kMoveStep, 1e-5);
    CHECK_NEAR(bot.position.y, 0.0f, 1e-5);

    Robot diagonal("D", Vec2(0.0f, 0.0f));
    diagonal.moveTo(Vec2(3.0f, 4.0f));
    CHECK_NEAR(diagonal.position.distanceTo(Vec2(0.0f, 0.0f)), Robot::kMoveStep, 1e-5);
}

void testDamageAndShield() {
    Robot bot("Tank", Vec2(0.0f, 0.0f));
    bot.takeDamage(40.0f);
    CHECK_NEAR(bot.health, 60.0f, 1e-5);
    CHECK(bot.isAlive());

    bot.activateShield();
    CHECK(bot.shield);
    bot.takeDamage(20.0f);
    CHECK_NEAR(bot.health, 50.0f, 1e-5);

    bot.takeDamage(-5.0f);
    CHECK_NEAR(bot.health, 50.0f, 1e-5);

    bot.takeDamage(1000.0f);
    CHECK_NEAR(bot.health, 0.0f, 1e-5);
    CHECK(!bot.isAlive());
}

void testClampToArena() {
    Robot bot("Out", Vec2(-5.0f, 140.0f));
    bot.clampToArena(Simulation::kWidth, Simulation::kHeight);
    CHECK_NEAR(bot.position.x, 0.0f, 1e-6);
    CHECK_NEAR(bot.position.y, Simulation::kHeight, 1e-6);
}

void testScriptMoveAndShield() {
    Robot bot("Alpha", Vec2(0.0f, 0.0f));
    std::vector<Robot> robots;
    robots.push_back(bot);

    ScriptEngine engine;
    engine.loadScript("moveTo(10, 0)\nactivateShield()\n");
    engine.execute(robots[0], robots);

    CHECK_NEAR(robots[0].position.x, Robot::kMoveStep, 1e-5);
    CHECK(robots[0].shield);
}

void testScriptIgnoresCommentsWhitespaceAndJunk() {
    Robot bot("Alpha", Vec2(5.0f, 5.0f));
    std::vector<Robot> robots{bot};

    ScriptEngine engine;
    engine.loadScript(
        "# full line comment\n"
        "  // also a comment\n"
        "\n"
        "   moveTo( 5 , 5 )  \n"
        "not-a-command\n"
        "takeDamage(oops)\n");
    const float x = robots[0].position.x;
    const float y = robots[0].position.y;
    engine.execute(robots[0], robots);

    CHECK_NEAR(robots[0].position.x, x, 1e-5);
    CHECK_NEAR(robots[0].position.y, y, 1e-5);
    CHECK_NEAR(robots[0].health, Robot::kMaxHealth, 1e-5);
}

void testEmptyScriptIsNoOp() {
    Robot bot("Alpha", Vec2(3.0f, 7.0f));
    std::vector<Robot> robots{bot};
    ScriptEngine engine;
    engine.loadScript("");
    engine.execute(robots[0], robots);
    CHECK_NEAR(robots[0].position.x, 3.0f, 1e-6);
    CHECK_NEAR(robots[0].position.y, 7.0f, 1e-6);
}

void testTakeDamageCommand() {
    Robot bot("Alpha", Vec2(0.0f, 0.0f));
    std::vector<Robot> robots{bot};
    ScriptEngine engine;
    engine.loadScript("takeDamage(15)");
    engine.execute(robots[0], robots);
    CHECK_NEAR(robots[0].health, 85.0f, 1e-5);
}

void testMoveToNearestAndAttack() {
    std::vector<Robot> robots;
    robots.emplace_back("Alpha", Vec2(0.0f, 0.0f));
    robots.emplace_back("Beta", Vec2(5.0f, 0.0f));

    ScriptEngine engine;
    engine.loadScript("moveToNearest()\nattackNearest(10, 8)\n");
    engine.execute(robots[0], robots);

    CHECK_NEAR(robots[0].position.x, Robot::kMoveStep, 1e-5);
    CHECK_NEAR(robots[1].health, Robot::kMaxHealth - 8.0f, 1e-5);
    CHECK_NEAR(robots[0].health, Robot::kMaxHealth, 1e-5);
}

void testDeadRobotDoesNotAct() {
    std::vector<Robot> robots;
    robots.emplace_back("Alpha", Vec2(0.0f, 0.0f));
    robots.emplace_back("Beta", Vec2(3.0f, 0.0f));
    robots[0].health = 0.0f;

    ScriptEngine engine;
    engine.loadScript("attackNearest(10, 50)\nmoveToNearest()\n");
    engine.execute(robots[0], robots);

    CHECK_NEAR(robots[0].position.x, 0.0f, 1e-6);
    CHECK_NEAR(robots[1].health, Robot::kMaxHealth, 1e-5);
}

void testFindNearestSkipsSelfAndDead() {
    std::vector<Robot> robots;
    robots.emplace_back("Alpha", Vec2(0.0f, 0.0f));
    robots.emplace_back("Dead", Vec2(1.0f, 0.0f));
    robots.emplace_back("Beta", Vec2(20.0f, 0.0f));
    robots[1].health = 0.0f;

    Robot* nearest = ScriptEngine::findNearestAlive(robots[0], robots);
    CHECK(nearest == &robots[2]);
}

void testSimulationResetAndTick() {
    Simulation sim;
    sim.reset("moveTo(50, 50)\n");
    CHECK(sim.robots().size() == 2);
    CHECK(sim.robots()[0].name == "Alpha");
    CHECK(sim.robots()[1].name == "Beta");
    CHECK_NEAR(sim.robots()[0].position.x, 10.0f, 1e-5);
    CHECK_NEAR(sim.robots()[1].position.x, 90.0f, 1e-5);
    CHECK(sim.tickCount() == 0);

    sim.tick();
    CHECK(sim.tickCount() == 1);
    CHECK(sim.robots()[0].position.x > 10.0f);
    CHECK(sim.robots()[1].position.x < 90.0f);
    CHECK(sim.hasLivingRobot());
}

void testSimulationDuelEventuallyDealsDamage() {
    Simulation sim;
    sim.reset("moveToNearest()\nattackNearest(8, 4)\n");

    bool damaged = false;
    for (int i = 0; i < 200; ++i) {
        sim.tick();
        if (sim.robots()[0].health < Robot::kMaxHealth ||
            sim.robots()[1].health < Robot::kMaxHealth) {
            damaged = true;
            break;
        }
    }
    CHECK(damaged);
    CHECK(sim.robots()[0].position.distanceTo(sim.robots()[1].position) <= 8.0f + Robot::kMoveStep);
}

}  // namespace

int main() {
    testVec2();
    testRobotDefaultsAndMovement();
    testDamageAndShield();
    testClampToArena();
    testScriptMoveAndShield();
    testScriptIgnoresCommentsWhitespaceAndJunk();
    testEmptyScriptIsNoOp();
    testTakeDamageCommand();
    testMoveToNearestAndAttack();
    testDeadRobotDoesNotAct();
    testFindNearestSkipsSelfAndDead();
    testSimulationResetAndTick();
    testSimulationDuelEventuallyDealsDamage();

    std::cout << g_passed << " passed, " << g_failed << " failed\n";
    return g_failed == 0 ? 0 : 1;
}
